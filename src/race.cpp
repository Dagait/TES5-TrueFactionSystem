#include "Race.h"
// Get this from: https://github.com/nlohmann/json
#include "nlohmann/json.hpp"


const float raceFactionBonus = 15.0f;

const std::string raceFactionFilePath = "tfs_definitions/race_faction.json";
const std::string raceValuesFilePath = "tfs_definitions/race_values.json";

std::unordered_map<std::string, std::unordered_map<std::string, int>> factionRaceData;
std::unordered_map<std::string, std::unordered_map<std::string, int>> raceValueData;

void LoadJsonData() {
    std::string dllPath = GetCurrentDLLPath();
    dllPath = dllPath.substr(0, dllPath.find_last_of("\\/")) + "\\";

    std::ifstream raceFactionFile(dllPath + raceFactionFilePath);
    std::ifstream raceValuesFile(dllPath + raceValuesFilePath);

    if (!raceFactionFile.is_open() || !raceValuesFile.is_open()) {
        spdlog::error("Failed to open one of the JSON files.");
        return;
    }

    nlohmann::json raceFactionJson;
    nlohmann::json raceValuesJson;

    raceFactionFile >> raceFactionJson;
    raceValuesFile >> raceValuesJson;

    // Parse the race-faction correlation data
    for (auto &[faction, raceList] : raceFactionJson["factions"].items()) {
        std::unordered_map<std::string, int> raceMap;
        for (auto &[fitType, races] : raceList.items()) {
            int value = 0;

            if (fitType == "best_fit")
                value = 20;
            else if (fitType == "possible")
                value = 10;
            else if (fitType == "unlikely")
                value = 0;
            else if (fitType == "impossible")
                value = -20;

            for (auto &race : races) {
                raceMap[race] = value;
            }
        }
        factionRaceData[faction] = raceMap;
    }

    for (auto &[race, fitMap] : raceValuesJson["races"].items()) {
        std::unordered_map<std::string, int> fitValues;
        fitValues["best_fit"] = fitMap["best_fit"];
        fitValues["possible"] = fitMap["possible"];
        fitValues["unlikely"] = fitMap["unlikely"];
        fitValues["impossible"] = fitMap["impossible"];

        raceValueData[race] = fitValues;
    }

    spdlog::info("Read in JSON data.");
    spdlog::info("factionRaceData size: {}", factionRaceData.size());
    spdlog::info("raceValueData size: {}", raceValueData.size());
}

int RaceValueForFaction(const std::string &race, const std::string &faction) {
    // Check if faction exists
    if (factionRaceData.find(faction) == factionRaceData.end()) {
        spdlog::error("Faction not found.");
        return 0;
    }

    // Check if the race exists within that faction's data
    auto &raceMap = factionRaceData[faction];
    if (raceMap.find(race) != raceMap.end()) {
        return raceMap[race];
    } else {
        spdlog::error("Race not found in faction.");
        return 0;
    }
}

RE::TESRace *GetPlayerRace() { 
	RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
    if (player) {
        return player->GetRace();
    }
    return nullptr;
}

void InitRaceDisguiseBonus() {
    if (factionRaceData.empty() || raceValueData.empty()) {
        LoadJsonData();
    }

    // Get the player's race
    RE::TESRace *playerRace = GetPlayerRace();
    if (!playerRace) {
        spdlog::error("Player race not found.");
        return;
    }

    // Retrieve the 'npe' keyword from the player's race
    std::string raceIdentifier;

    // Get the race's keywords
    RE::BGSKeywordForm *keywordForm = static_cast<RE::BGSKeywordForm *>(playerRace);
    if (keywordForm) {
        bool foundNpeKeyword = false;
        for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
            RE::BGSKeyword *keyword = keywordForm->keywords[i];
            if (keyword) {
                std::string keywordEditorID = keyword->GetFormEditorID();
                if (keywordEditorID.rfind("npe", 0) == 0) {  // keyword starts with 'npe'
                    raceIdentifier = keywordEditorID;
                    foundNpeKeyword = true;
                    spdlog::info("Found 'npe' keyword for player's race: {}", raceIdentifier);
                    break;  // Exit loop after finding the keyword
                }
            }
        }
        if (!foundNpeKeyword) {
            spdlog::error("No 'npe' keyword found for player's race.");
            return;
        }
    } else {
        spdlog::error("Player's race does not have any keywords.");
        return;
    }

    // Loop through all factions to initialize the race bonus value
    for (const auto &[tag, factionID] : factionArmorKeywords) {
        RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
        if (!faction) {
            spdlog::error("Faction not found for tag '{}'", tag);
            continue;
        }

        // Fetch the disguise bonus for the player's race in this faction
        auto factionIt = factionRaceData.find(tag);
        if (factionIt != factionRaceData.end()) {
            const auto &raceMap = factionIt->second;
            auto raceIt = raceMap.find(raceIdentifier);
            if (raceIt != raceMap.end()) {
                int raceFactionBonus = raceIt->second;
                spdlog::info("Setting race bonus for faction '{}' (ID: {}): {}", tag, factionID, raceFactionBonus);
                playerDisguiseStatus.SetRaceBonusValue(faction, raceFactionBonus);
            } else {
                spdlog::warn("Player's race '{}' not found in faction '{}'", raceIdentifier, tag);
                // Optionally, set a default value or handle as needed
                playerDisguiseStatus.SetRaceBonusValue(faction, 0);
            }
        } else {
            spdlog::error("Faction '{}' not found in factionRaceData.", tag);
        }
    }
}


bool IsPlayerInCorrectRace(RE::BSFixedString keyword) { 
    RE::TESRace *playerRace = GetPlayerRace(); 
    RE::BGSKeyword **raceKeywords = playerRace->keywords;
    for (uint32_t i = 0; i < playerRace->numKeywords; i++) {
        if (raceKeywords[i]->GetFormEditorID() == keyword) {
            return true;
        }
    }
    return false;
}