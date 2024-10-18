#include "Managers/DetectionManager.h"
#include "Globals.h"


namespace NPE {
    /**
     * Check if NPCs detect the player based the player's disguise value
     * @param **player** The player actor
     */
    void DetectionManager::CheckNPCDetection(RE::Actor *player) {
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
                        float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction) +
                                              playerDisguiseStatus.GetBonusValue(faction);

                        if (disguiseValue > 0.0f && npc->IsInFaction(faction)) {
                            CheckHoursPassed(npc, player, faction);

                            bool isInLineOfSight = environmentManager.IsInLineOfSight(npc, player);
                            bool isInFieldOfView = environmentManager.IsInFieldOfView(npc, player);

                            if (!isInFieldOfView) {
                                return false;
                            }

                            float distance = std::abs(player->GetPosition().GetDistance(npc->GetPosition()));

                            // Calculate detection probability
                            float detectionProbability = GetDetectionProbability(disguiseValue);
                            detectionProbability =
                                AdjustProbabilityByDistance(detectionProbability, distance, detectionRadius);

                            if (this->NPCRecognizesPlayer(npc, player, faction) ||
                                this->DetectCrimeWhileDisguised(npc, player)) {
                                this->StartCombat(npc, player, faction);
                                recognizedNPCs[npc->GetFormID()] = {npc->GetFormID(), currentInGameHours};
                                return true;  // NPC detected the player
                            }
                        }
                    }
                    return false;  // NPC did not detect the player
                }));
            }

            return RE::BSContainer::ForEachResult::kContinue;  // Continue with the next reference
        });

        for (auto &future : detectionFutures) {
            if (future.get()) {
                playerDetected = true;
                break;
            }
        }

        if (playerDetected) {
            return;
        }
    }

    bool DetectionManager::NPCRecognizesPlayer(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction) {
        float playerDisguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);
        float distance = abs(npc->GetPosition().GetDistance(player->GetPosition()));

        if (distance > DETECTION_RADIUS) {
            return false;
        }

        // Calculate the base recognition probability based on disguise and distance
        float recognitionProbability = (DETECTION_RADIUS - distance) / DETECTION_RADIUS;
        recognitionProbability *= (100.0f - playerDisguiseValue) / 100.0f;

        // Adjust by distance factor to scale probability
        float distanceFactor = 1.0f / (1.0f + std::exp((distance - DETECTION_RADIUS) * 0.1f));
        recognitionProbability *= distanceFactor;

        // Level difference affects detection
        int npcLevel = npc->GetLevel();
        int playerLevel = player->GetLevel();
        int levelDifference = abs(npcLevel - playerLevel);

        float levelFactor = 1.0f + 0.02f * std::log(1 + levelDifference);
        recognitionProbability *= levelFactor;

        // Add any environmental modifiers for detection (lighting, sneaking, etc.)
        recognitionProbability += environmentManager.GetEnvironmentalDetectionModifier(player);

        // Retrieve previous detection time for this NPC
        RE::FormID npcID = npc->GetFormID();
        float currentInGameHours = RE::Calendar::GetSingleton()->GetHoursPassed();

        if (recognizedNPCs.find(npcID) != recognizedNPCs.end()) {
            NPCDetectionData &detectionData = recognizedNPCs[npcID];

            float timeSinceLastDetected = currentInGameHours - detectionData.lastDetectedTime;

            if (timeSinceLastDetected < TIME_TO_LOSE_DETECTION) {
                recognitionProbability += 0.25f;  // NPC detected player recently, so boost detection chance
            } else {
                recognizedNPCs.erase(npcID);  // Forget if too much time has passed
            }
        }

        if (recognitionProbability >= 1.0f) {
            return true;
        }

        recognitionProbability = std::clamp(recognitionProbability, 0.0f, 1.0f);
        const float suspicionThreshold = 0.7f;

        // If recognitionProbability is high but not a guaranteed detection
        if (recognitionProbability >= suspicionThreshold) {
            this->TriggerSuspiciousIdle(npc);
        }

        // Detection check using rng
        if (recognitionProbability >= 0.72f) {
            static thread_local std::mt19937 gen(std::random_device{}());
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);

            float randomValue = dist(gen);
            if (randomValue <= recognitionProbability) {
                return true;
            }
        }

        return false;
    }

    void DetectionManager::TriggerSuspiciousIdle(RE::Actor *npc) {
        static RE::FormID idleFormID = 0x000977ED;  // StudyIdle for now
    
        RE::TESIdleForm *idleForm = RE::TESForm::LookupByID<RE::TESIdleForm>(idleFormID);
        if (idleForm && npc) {
            RE::stl::zstring startState = "StudyIdle";
            RE::stl::zstring endState = "StudyIdle";
            npc->PlayAnimation(startState, endState);
        } else {
            spdlog::error("Failed to play animation.");
        }
    }

    bool DetectionManager::DetectCrimeWhileDisguised(RE::Actor* npc, RE::Actor* player) {
        int detectionLevel = player->RequestDetectionLevel(npc);
        bool isLockpicking = player->IsLockpick();
        // bool isStealing = player->PickUpObject();

        if (detectionLevel > 1 && isLockpicking) {
            if (!player->IsSneaking()) {
                return true;
            }
        }
        return false;
    }

    float DetectionManager::GetDetectionProbability(float disguiseValue) { return abs(100.0f - disguiseValue); }

    void DetectionManager::CheckHoursPassed(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction) {
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

    float DetectionManager::AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
        float distanceFactor = 1.0f / (1.0f + std::exp((distance - maxDistance) * 0.1f));
        return detectionProbability * distanceFactor;
    }

    void DetectionManager::StartCombat(RE::Actor *npc, RE::Actor *player, RE::TESFaction *npcFaction) {
        // TODO: Rework
        if (!npc || !player) {
            return;
        }

        if (npcFaction) {
            player->AddToFaction(npcFaction, -1);
        }
    }
}