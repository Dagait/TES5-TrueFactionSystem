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

    // Loop through the factionArmorKeywords to check each faction
    for (const auto &[tag, factionID] : factionArmorKeywords) {
        if (IsPlayerInCorrectRace(tag)) {
            RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);

            // Get the player's race as a string
            RE::TESRace *playerRace = GetPlayerRace();
            if (!playerRace) return;

            std::string raceEditorID = playerRace->GetFormEditorID();

            // Fetch the disguise bonus for the race in this faction
            auto factionIt = factionRaceData.find(tag);
            if (factionIt != factionRaceData.end()) {
                auto raceIt = factionIt->second.find(raceEditorID);
                if (raceIt != factionIt->second.end()) {
                    int raceFactionBonus = raceIt->second;
                    playerDisguiseStatus.SetRaceBonusValue(faction, raceFactionBonus);
                }
            }
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