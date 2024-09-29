#include "disguise.h"
#include "faction.h"
#include "combat.h"
#include "npc_detection_data.h"
#include "disguise_data.h"
#include "armor_slots.h"

#include <cmath>
#include <random>
#include <unordered_map>
#include <future>
#include <vector>
#include <thread>


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

constexpr float DETECTION_RADIUS = 300.0f;

// Global map to store NPCs, which have recognized the player (Disguised Value the player)
std::unordered_map<RE::FormID, NPCDetectionData> recognizedNPCs;
constexpr float TIME_TO_LOSE_DETECTION = 2.0f;  // 2 hours

PlayerDisguiseStatus playerDisguiseStatus;

bool IsFaceCovered(Actor *actor) {
    constexpr const char *COVERED_FACE_TAG = "npeCoveredFace";

    for (auto slot : allArmorSlots) {
        RE::TESObjectARMO *armor = actor->GetWornArmor(slot);
        if (armor && armor->HasKeywordString(COVERED_FACE_TAG)) {
            return true;
        }
    }

    return false;
}


void CalculateDisguiseValue(Actor *actor, RE::TESFaction *faction) {
    float disguiseValue = 0.0f;

    std::string factionTag = GetTagForFaction(faction);
    if (factionTag.empty()) {
        return;
    }

    for (const auto &slot : armorSlots) {
        RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
        if (armor) {
            if (armor->HasKeywordString(factionTag)) {
                disguiseValue += slot.weight;
            }
        }
    }

    disguiseValue = std::clamp(disguiseValue, 0.0f, 100.0f);

    playerDisguiseStatus.SetDisguiseValue(faction, disguiseValue);
}

float GetDetectionProbability(float disguiseValue) {
    return abs(100.0f - disguiseValue);
}

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

bool IsValidWeather(RE::TESWeather *weather) {
    // TODO: Check if the current weather is the same as the weather we are looking for
    return false;
}

bool IsInFieldOfView(RE::Actor *npc, RE::Actor *player, float fieldOfViewDegrees = 90.0f) {
    if (!npc || !player) {
        return false;
    }

    float distance = npc->GetPosition().GetDistance(player->GetPosition());
    if (distance > DETECTION_RADIUS) {
        return false;
    }

    float npcRotationZ = npc->data.angle.z;
    float npcRotationZInRadians = npcRotationZ;
    // This vector points into the wrong direction???
    RE::NiPoint3 npcForward(std::cos(npcRotationZInRadians), std::sin(npcRotationZInRadians), 0.0f);

    RE::NiPoint3 npcToPlayer = player->GetPosition() - npc->GetPosition();
    float length = npcToPlayer.Length();
    if (length > 0.0f) {
        npcToPlayer /= length;
    }

    float dotProduct = npcForward.Dot(npcToPlayer);
    dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);

    float angle = std::acos(dotProduct) * (180.0f / M_PI);

    return angle <= (fieldOfViewDegrees / 2.0f);
}

bool IsNightTime() {
    RE::Calendar *calendar = RE::Calendar::GetSingleton();
    float currentHour = calendar->GetHour();
    return (currentHour >= 20.0f || currentHour <= 6.0f);
}

bool IsPlayerInDarkArea(RE::Actor *player) {
    // TODO: Implement this function to check if the player is in a dark area
    RE::TESObjectCELL *cell = player->GetParentCell();
    if (cell && cell->IsInteriorCell()) {
        return true;
    }
    // TODO: Check for weather and light sources
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

    float distanceFactor = 1.0f / std::pow((distance / DETECTION_RADIUS), 1.2f);
    recognitionProbability *= distanceFactor;

    // Level check for NPCs vs player
    int npcLevel = npc->GetLevel();
    int playerLevel = player->GetLevel();
    int levelDifference = abs(npcLevel - playerLevel);

    float levelFactor = 1.0f;
    if (levelDifference >= 0) {
        levelFactor += 0.08f * levelDifference;
    }

    recognitionProbability *= levelFactor;
    // End level check

    if (IsNightTime()) {
        // If it is night time and the player is not in a dark area, reduce the recognition probability
        recognitionProbability *= 0.2f;
    }
    if (!IsPlayerInDarkArea(player)) {
        // If the player is not in a dark area, reduce the recognition probability
        recognitionProbability *= 0.3f;
    }
    if (IsValidWeather(RE::Sky::GetSingleton()->currentWeather)) {
        // If the weather is not valid, reduce the recognition probability
        recognitionProbability *= 0.2f;
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
        RE::Actor *npc = skyrim_cast<RE::Actor*>(&ref);
        /*
        * Check if the reference is an actor and not the player
         * The Player also needs to be in the NPC's line of sight and field of view
        */
        if (npc && npc != player) {
            detectionFutures.push_back(std::async(std::launch::async, [&, npc]() {
                // Iterate through factions to check disguise values
                for (const auto &[factionName, faction] : GetRelevantFactions()) {
                    float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);

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
                            // NPC detected the player
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

    for (RE::TESFaction *faction : factions) {
        if (!faction) {
            continue;
        }

        CalculateDisguiseValue(actor, faction);

        float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);

        if (!actor->IsInFaction(faction)) {
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