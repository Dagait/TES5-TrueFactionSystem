#include "Disguise.h"


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

constexpr float DETECTION_RADIUS = 450.0f;

// Global map to store NPCs, which have recognized the player (Disguised Value the player)
std::unordered_map<RE::FormID, NPCDetectionData> recognizedNPCs;
constexpr float TIME_TO_LOSE_DETECTION = 2.0f;  // 2 hours

PlayerDisguiseStatus playerDisguiseStatus;

float GetDisguiseValueForFaction(RE::TESFaction *faction) { return playerDisguiseStatus.GetDisguiseValue(faction); }

float GetDisguiseBonusValueForFaction(RE::TESFaction *faction) { return playerDisguiseStatus.GetBonusValue(faction); }

float GetRaceBonusValueForFaction(RE::TESFaction *faction) { return playerDisguiseStatus.GetRaceBonusValue(faction); }

void AddArmorSetBonus(RE::Actor *actor) {
    for (const auto &[factionTag, factionID] : factionArmorKeywords) {
        int matchingArmorPieces = 0;
        const int totalArmorSlots = 5;

        for (const auto &slot : armorSlotsSlot) {
            RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
            if (armor && armor->HasKeywordString(factionTag)) {
                matchingArmorPieces++;
            }
        }

        float bonusAmount = 0.0f;

        if (matchingArmorPieces >= 4) {
            bonusAmount = 20.0f;
        } else if (matchingArmorPieces >= 3) {
            bonusAmount = 10.0f;
        } else if (matchingArmorPieces >= 2) {
            bonusAmount = 5.0f;
        }

        // If any bonus is applied, increase the actor's ArmorRating
        if (bonusAmount > 0.0f) {
            RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
            playerDisguiseStatus.SetBonusValue(faction, bonusAmount);
        }
    }
}

void CalculateDisguiseValue(Actor *actor, RE::TESFaction *faction) {
    float disguiseValue = 0.0f;
    constexpr const char *COVERED_FACE_TAG = "npeCoveredFace";

    std::string factionTag = GetTagForFaction(faction);
    if (factionTag.empty()) {
        return;
    }

    if (IsPlayerInCorrectRace(factionTag)) {
        disguiseValue += playerDisguiseStatus.GetRaceBonusValue(faction);
    }
    disguiseValue += playerDisguiseStatus.GetBonusValue(faction);

    for (const auto &slot : armorSlotsSlot) {
        // Get Keyword linked to the factionFormID from factionFormIDToTagMap {{factionFormID, factionKeyword},...}
        auto it = factionFormIDToTagMap.find(faction->GetFormID());
        if (it == factionFormIDToTagMap.end()) {
            continue;
        }
        std::string factionKeyword = it->second;

        RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
        if (armor) {
            if (armor->HasKeywordString(COVERED_FACE_TAG) && armor->HasKeywordString(factionKeyword)) {
                playerDisguiseStatus.SetDisguiseValue(faction, 100);
                AddArmorSetBonus(actor);
                return;
            }

            if (armor->HasKeywordString(factionTag)) {
                disguiseValue += slot.weight;
            }
        }
    }

    disguiseValue = std::clamp(disguiseValue, 0.0f, 100.0f);

    playerDisguiseStatus.SetDisguiseValue(faction, disguiseValue);
    AddArmorSetBonus(actor);
}

float GetDetectionProbability(float disguiseValue) { return abs(100.0f - disguiseValue); }

float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
    float distanceFactor = 1.0f - (distance / maxDistance);  // Scaled by 1 (near) to 0 (far)
    return detectionProbability * distanceFactor;
}

bool IsInLineOfSight(RE::Actor *npc, RE::Actor *player) {
    if (!npc || !player) {
        return false;
    }

    bool hasLineOfSight = false;
    return npc->HasLineOfSight(player->AsReference(), hasLineOfSight);
}

bool IsBadWeather(RE::TESWeather *currentWeather) {
    // Check if the weather is rainy, foggy, or cloudy (bad for visibility)
    if (currentWeather) {
        if (currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kRainy ||
            currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kCloudy ||
            currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kSnow) {
            return true;
        }
    }
    return false;
}

bool IsInFieldOfView(RE::Actor *npc, RE::Actor *player, float fieldOfViewDegrees) {
    if (!npc || !player) {
        return false;
    }

    float distance = npc->GetPosition().GetDistance(player->GetPosition());
    if (distance > DETECTION_RADIUS) {
        return false;
    }

    float npcRotationZ = npc->data.angle.z;
    float npcRotationZInRadians = npcRotationZ;

    RE::NiPoint3 npcForward(std::cos(npcRotationZInRadians), std::sin(npcRotationZInRadians), 0.0f);

    RE::NiPoint3 npcToPlayer = player->GetPosition() - npc->GetPosition();
    float length = npcToPlayer.Length();
    if (length > 0.0f) {
        npcToPlayer /= length;
    }

    float dotProduct = npcForward.Dot(npcToPlayer);
    dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
    float fovFactor = std::cos((fieldOfViewDegrees / 2.0f) * (M_PI / 180.0f));

    if (dotProduct >= fovFactor) {
        // Fully inside the FOV
        return true;
    } else {
        // Smooth falloff for detection outside the FOV
        float falloffFactor = std::max(0.0f, (dotProduct + 1.0f) / (1.0f - fovFactor));
        return falloffFactor > 0.0f;
    }
}

bool IsNightTime() {
    RE::Calendar *calendar = RE::Calendar::GetSingleton();
    float currentHour = calendar->GetHour();
    return (currentHour >= 20.0f || currentHour <= 6.0f);
}

bool IsPlayerNearLightSource(RE::Actor *player, float radius) {
    if (!player) {
        return false;
    }

    RE::TESObjectCELL *cell = player->GetParentCell();
    if (!cell) {
        return false;
    }

    NiPoint3 playerPos = player->GetPosition();

    cell->ForEachReferenceInRange(playerPos, radius, [&](RE::TESObjectREFR &ref) -> BSContainer::ForEachResult {
        RE::TESObjectLIGH *lightSource = ref.As<RE::TESObjectLIGH>();
        if (lightSource && lightSource->data.radius > 0) {
            return BSContainer::ForEachResult::kStop;
        }
        return BSContainer::ForEachResult::kContinue;
    });

    return false;
}

bool IsPlayerInDarkArea(RE::Actor *player) {
    RE::TESObjectCELL *cell = player->GetParentCell();
    if (!cell) {
        return false;
    }

    if (cell->IsInteriorCell()) {
        RE::INTERIOR_DATA *lightingData = cell->GetLighting();
        float lightingLevel = lightingData->ambient.red +
                              lightingData->ambient.green +
                              lightingData->ambient.blue;

        return lightingLevel < 150.0f;
    } else {
        // If exterior, check the current weather and light levels from the Sky singleton
        RE::Sky *sky = RE::Sky::GetSingleton();
        if (!sky) {
            return false;
        }

        RE::TESWeather *currentWeather = sky->currentWeather;
        if (IsBadWeather(currentWeather)) {
            return true;
        }

        if (!IsPlayerNearLightSource(player)) {
            return true;
        }

        float currentTime = sky->currentGameHour;
        if (IsNightTime()) {
            return true;
        }
    }

    return false;
}


void CheckHoursPassed(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction) {
    RE::FormID npcID = npc->GetFormID();
    float currentInGameHours = RE::Calendar::GetSingleton()->GetHoursPassed();

    if (recognizedNPCs.find(npcID) != recognizedNPCs.end()) {
        NPCDetectionData &detectionData = recognizedNPCs[npcID];

        float timeSinceLastDetected = currentInGameHours - detectionData.lastDetectedTime;

        if (timeSinceLastDetected > TIME_TO_LOSE_DETECTION) {
            recognizedNPCs.erase(npcID);
            if (playerDisguiseStatus.GetDisguiseValue(faction) > 5.0f) {
                player->AddToFaction(faction, 1);
            }
        }
    }
}

bool NPCRecognizesPlayer(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction) {
    float playerDisguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);
    float distance = abs(npc->GetPosition().GetDistance(player->GetPosition()));
    if (distance > DETECTION_RADIUS) {
        return false;
    }

    float recognitionProbability = (100.0f - playerDisguiseValue) / 100.0f;

    float distanceFactor = 1.0f / (1.0f + std::exp((distance - DETECTION_RADIUS) * 0.1f));
    recognitionProbability *= distanceFactor;

    // Level check for NPCs vs player
    int npcLevel = npc->GetLevel();
    int playerLevel = player->GetLevel();
    int levelDifference = abs(npcLevel - playerLevel);

    float levelFactor = 1.0f;
    levelFactor += 0.02f * std::log(1 + levelDifference);

    recognitionProbability *= levelFactor;
    // End level check

    if (IsPlayerInDarkArea(player)) {
        recognitionProbability *= 0.3f;
    } else {
        recognitionProbability += 0.2f;
    }

    RE::FormID npcID = npc->GetFormID();
    float currentInGameHours = RE::Calendar::GetSingleton()->GetHoursPassed();

    if (recognizedNPCs.find(npcID) != recognizedNPCs.end()) {
        NPCDetectionData &detectionData = recognizedNPCs[npcID];

        float timeSinceLastDetected = currentInGameHours - detectionData.lastDetectedTime;

        if (timeSinceLastDetected < TIME_TO_LOSE_DETECTION) {
            // If the NPC has detected the player before, increase the recognition probability
            recognitionProbability += 0.5f;
        } else {
            // If time has passed since the last detection, reset the detection count
            recognizedNPCs.erase(npcID);
        }
    }

    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float randomValue = dist(gen);
    recognitionProbability = std::clamp(recognitionProbability, 0.0f, 1.0f);
    return randomValue <= recognitionProbability;
}

/**
 * Check if NPCs detect the player based the player's disguise value
 * @param **player** The player actor
 */
void CheckNPCDetection(RE::Actor *player) {
    RE::TESObjectCELL *currentCell = player->GetParentCell();
    if (!currentCell) {
        // Player is not in a cell (e.g. in the main menu), return
        return;
    }

    bool playerDetected = false;
    float currentInGameHours = RE::Calendar::GetSingleton()->GetHoursPassed();
    float detectionRadius = DETECTION_RADIUS;

    std::vector<std::future<bool>> detectionFutures;

    // Look for NPCs in the player's detection range
    currentCell->ForEachReferenceInRange(player->GetPosition(), detectionRadius, [&](RE::TESObjectREFR &ref) {
        RE::Actor *npc = skyrim_cast<RE::Actor *>(&ref);
        /*
         * Check if the reference is an actor and not the player
         * The Player also needs to be in the NPC's line of sight and field of view
         */
        if (npc && npc != player) {
            detectionFutures.push_back(std::async(std::launch::async, [&, npc]() {
                // Iterate through factions to check disguise values
                for (const auto &[factionName, faction] : GetRelevantFactions()) {
                    float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction) + playerDisguiseStatus.GetBonusValue(faction);

                    if (disguiseValue > 0.0f && npc->IsInFaction(faction)) {
                        CheckHoursPassed(npc, player, faction);

                        bool isInLineOfSight = IsInLineOfSight(npc, player);
                        bool isInFieldOfView = IsInFieldOfView(npc, player);

                        if (!isInLineOfSight && !isInFieldOfView) {
                            return false;
                        }

                        float distance = std::abs(player->GetPosition().GetDistance(npc->GetPosition()));

                        // Calculate detection probability
                        float detectionProbability = GetDetectionProbability(disguiseValue);
                        detectionProbability =
                            AdjustProbabilityByDistance(detectionProbability, distance, detectionRadius);

                        if (NPCRecognizesPlayer(npc, player, faction)) {
                            StartCombat(npc, player, faction);
                            recognizedNPCs[npc->GetFormID()] = {npc->GetFormID(), currentInGameHours};
                            return true;  // NPC detected the player
                        }
                    }
                }
                return false;  // NPC did not detect the player
            }));
        }

        return BSContainer::ForEachResult::kContinue;  // Continue with the next reference
    });

    for (auto &future : detectionFutures) {
        if (future.get()) {
            playerDetected = true;
            break;  // Stop checking if the player is detected
        }
    }

    if (playerDetected) {
        return;
    }
}

void RemovePlayerFromAllFactions(Actor *player) {
    auto factions = GetRelevantFactions();
    for (auto &[factionName, faction] : factions) {
        if (player->IsInFaction(faction)) {
            player->AddToFaction(faction, -1);
        }
    }
}

void UpdateDisguiseValue(Actor *actor) {
    std::vector<RE::TESFaction *> factions = GetFactionsByArmorTags(actor);
    if (factions.empty()) {
        playerDisguiseStatus.Clear();
        RemovePlayerFromAllFactions(actor);
        return;
    }

    ClearArmorDisguiseValues(actor);

    for (RE::TESFaction *faction : factions) {
        if (!faction) {
            continue;
        }

        CalculateDisguiseValue(actor, faction);

        float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);

        if (!actor->IsInFaction(faction) && disguiseValue > 5.0f) {
            actor->AddToFaction(faction, 1);
        } else {
            if (disguiseValue <= 5.0f) {
                actor->AddToFaction(faction, -1);
                playerDisguiseStatus.RemoveDisguiseValue(faction);
            }
        }
    }

    CheckNPCDetection(actor);
}

void SaveDetectionData(SKSE::SerializationInterface *a_intfc) {
    for (auto &[npcID, detectionData] : recognizedNPCs) {
        a_intfc->WriteRecord('NPCD', 1, &npcID, sizeof(npcID));
        a_intfc->WriteRecordData(&detectionData, sizeof(detectionData));
    }
}

void ClearArmorDisguiseValues(RE::Actor *actor) {
    for (const auto &[factionTag, factionID] : factionArmorKeywords) {
        RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
        if (faction) {
            playerDisguiseStatus.SetBonusValue(faction, 0.0f);
            playerDisguiseStatus.SetDisguiseValue(faction, 0.0f);
        }
    }
}

void LoadDetectionData(SKSE::SerializationInterface *a_intfc) {
    std::uint32_t type;
    std::uint32_t version;
    std::uint32_t length;

    while (a_intfc->GetNextRecordInfo(type, version, length)) {
        RE::FormID npcID;
        NPCDetectionData detectionData;

        if (a_intfc->ReadRecordData(&npcID, sizeof(npcID))) {
            if (length == sizeof(detectionData)) {
                if (a_intfc->ReadRecordData(&detectionData, sizeof(detectionData))) {
                    recognizedNPCs[npcID] = detectionData;
                }
            } else {
                RE::ConsoleLog::GetSingleton()->Print("Record length mismatch!");
            }
        }
    }
}