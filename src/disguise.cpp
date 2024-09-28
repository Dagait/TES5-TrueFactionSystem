#include "disguise.h"
#include "faction.h"
#include "npcdetectiondata.h"

#include <cmath>
#include <unordered_map>
#include <chrono>


constexpr float DETECTION_RADIUS = 250.0f;

constexpr float CHEST_WEIGHT = 70.0f;
constexpr float HELMET_WEIGHT = 15.0f;
constexpr float GLOVES_WEIGHT = 4.0f;
constexpr float FOREARMS_WEIGHT = 8.0f;
constexpr float SHOES_WEIGHT = 5.0f;
constexpr float CIRCLET_WEIGHT = 1.0f;

constexpr RE::ActorValue kDisguiseValue = static_cast<RE::ActorValue>(1);
// Global map to store NPCs, which have recognized the player (Disguised Value the player)
std::unordered_map<RE::FormID, NPCDetectionData> recognizedNPCs;
constexpr std::chrono::minutes TIME_TO_LOSE_DETECTION(120);  // 2 hours



float CalculateDisguiseValue(Actor *actor, bool isEquipped) {
    float disguiseValue = abs(actor->AsActorValueOwner()->GetActorValue(kDisguiseValue));

    // Also check, if the armor belongs to a faction
    RE::TESFaction *faction = GetFactionByArmorTag(actor);

    // TODO: Differ between equip and unequip
    if (isEquipped) {
        // Add the weight of the armor to the disguise value
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody) && faction) {
            disguiseValue += CHEST_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead) && faction) {
            disguiseValue += HELMET_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHands) && faction) {
            disguiseValue += GLOVES_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kForearms) && faction) {
            disguiseValue += FOREARMS_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet) && faction) {
            disguiseValue += CIRCLET_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kFeet) && faction) {
            disguiseValue += SHOES_WEIGHT;
        }
    } else {
        // Subtract the weight of the armor from the disguise value
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody) && faction) {
            disguiseValue -= CHEST_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead) && faction) {
            disguiseValue -= HELMET_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHands) && faction) {
            disguiseValue -= GLOVES_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kForearms) && faction) {
            disguiseValue -= FOREARMS_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet) && faction) {
            disguiseValue -= CIRCLET_WEIGHT;
        }
        if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kFeet) && faction) {
            disguiseValue -= SHOES_WEIGHT;
        }
    }

    return disguiseValue > 100 ? 100 : abs(disguiseValue);
}

float GetDetectionProbability(float disguiseValue) {
    return abs(100.0f - disguiseValue);
}

float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
    float distanceFactor = 1.0f - (distance / maxDistance);  // Scaled by 1 (near) to 0 (far)
    return detectionProbability * distanceFactor;
}

bool NPCRecognizesPlayer(RE::Actor *npc, RE::Actor *player) {
    float playerDisguiseValue = abs(player->AsActorValueOwner()->GetActorValue(kDisguiseValue));
    float distance = abs(npc->GetPosition().GetDistance(player->GetPosition()));

    float recognitionProbability = (100.0f - playerDisguiseValue) / 100.0f;
    recognitionProbability *= (distance / DETECTION_RADIUS);

    auto npcID = npc->GetFormID();
    auto now = std::chrono::steady_clock::now();

    if (recognizedNPCs.find(npcID) != recognizedNPCs.end()) {
        NPCDetectionData &detectionData = recognizedNPCs[npcID];

        auto timeSinceLastDetected = now - detectionData.lastDetectedTime;

        if (timeSinceLastDetected < TIME_TO_LOSE_DETECTION) {
            // If the NPC has detected the player before, increase the recognition probability
            recognitionProbability += 0.5f;
        } else {
            // If time has passed since the last detection, reset the detection count
            recognizedNPCs.erase(npcID);
        }
    }

    // Zufälliger Wert für die Erkennung
    float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return randomValue < recognitionProbability;
}

/**
 * Check if NPCs detect the player based the player's disguise value
 * @param player The player actor
*/
void CheckNPCDetection(RE::Actor* player) {
    RE::TESObjectCELL* currentCell = player->GetParentCell();
    if (!currentCell) {
        RE::ConsoleLog::GetSingleton()->Print("Player is not in a cell!");
        return;
    }

    float disguiseValue = player->AsActorValueOwner()->GetActorValue(kDisguiseValue);
    bool playerDetected = false;

    auto factions = GetRelevantFactions();
    auto now = std::chrono::steady_clock::now();

    for (auto &[factionName, faction] : factions) {
        currentCell->ForEachReferenceInRange(player->GetPosition(), DETECTION_RADIUS, [&](RE::TESObjectREFR &ref) {
            RE::Actor *npc = skyrim_cast<RE::Actor *>(&ref);
            if (npc && npc != player && npc->IsInFaction(faction)) {  // Check if NPC is in the relevant faction
                RE::ConsoleLog::GetSingleton()
                    ->Print(("NPC " + std::to_string(npc->GetFormID()) + " from " + factionName + " is near!").c_str());
                float distance = abs(player->GetPosition().GetDistance(npc->GetPosition()));

                // Calculate the detection probability based on the player's disguise value
                float detectionProbability = GetDetectionProbability(disguiseValue);
                detectionProbability = AdjustProbabilityByDistance(detectionProbability, distance, DETECTION_RADIUS);

                if (NPCRecognizesPlayer(npc, player)) {
                    playerDetected = true;

                    // NPC has recognized the player, add to map
                    recognizedNPCs[npc->GetFormID()] = {1, now};

                    player->AddToFaction(faction, -1);  // Remove from faction if detected
                    return BSContainer::ForEachResult::kStop;
                }
            }
            return BSContainer::ForEachResult::kContinue;
        });

        if (playerDetected) {
            break;
        }
    }
}


void UpdateDisguiseValue(Actor *actor, bool isEquipped) {
    // Calculate the disguise value based on equipped armor
    float disguiseValue = CalculateDisguiseValue(actor, isEquipped);

    // Set the actor's custom disguise value
    actor->AsActorValueOwner()->SetActorValue(kDisguiseValue, disguiseValue);

    RE::ConsoleLog::GetSingleton()->Print(("Current Disguise Value: " + std::to_string(disguiseValue)).c_str());

    // Get the faction corresponding to the armor worn by the player
    RE::TESFaction *faction = GetFactionByArmorTag(actor);

    if (faction && !actor->IsInFaction(faction)) {
        // Add the player to the faction if they are wearing faction armor
        actor->AddToFaction(faction, 1);
        RE::ConsoleLog::GetSingleton()->Print("Player added to faction!");
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