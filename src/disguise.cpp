#include "disguise.h"

#include "faction.h"

constexpr RE::ActorValue kDisguiseValue = static_cast<RE::ActorValue>(0);


float CalculateDisguiseValue(Actor *actor) {
    float disguiseValue = 0.0f;

    // List of armor weights (Need to be adjusted)
    constexpr float chestWeight = 70.0f;
    constexpr float helmetWeight = 15.0f;
    constexpr float glovesWeight = 4.0f;
    constexpr float forearmsWeight = 8.0f;
    constexpr float shoesWeight = 5.0f;
    constexpr float circletWeight = 1.0f;

    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody)) {
        disguiseValue += chestWeight;
    }
    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHead)) {
        disguiseValue += helmetWeight;
    }
    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kHands)) {
        disguiseValue += glovesWeight;
    }
    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kForearms)) {
        disguiseValue += forearmsWeight;
    }
    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet)) {
        disguiseValue += circletWeight;
    }
    if (actor->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kFeet)) {
        disguiseValue += shoesWeight;
    }

    return disguiseValue;
}

float GetDetectionProbability(float disguiseValue) {
    return 100.0f - disguiseValue;
}

bool IsPlayerDetected(float disguiseValue) {
    float detectionProbability = GetDetectionProbability(disguiseValue);

    // Generate a random value between 0 and 100
    float randomValue = static_cast<float>(rand() % 101);
    return randomValue <= detectionProbability;
}

float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
    float distanceFactor = 1.0f - (distance / maxDistance);  // Scaled by 1 (near) to 0 (far)
    return detectionProbability * distanceFactor;
}

bool CheckNPCDetection(RE::Actor* player, float disguiseValue) {
    RE::TESObjectCELL* currentCell = player->GetParentCell();
    if (!currentCell) {
        return false;
    }

    float detectionRadius = 500.0f;

    bool playerDetected = false;

    currentCell->ForEachReferenceInRange(player->GetPosition(), detectionRadius, [&](RE::TESObjectREFR& ref) {
        RE::Actor* npc = skyrim_cast<RE::Actor*>(&ref);
        if (npc && npc != player) {
            float distance = player->GetPosition().GetDistance(npc->GetPosition());

            // Calculate the detection probability based on the player's disguise value
            float detectionProbability = GetDetectionProbability(disguiseValue);
            detectionProbability = AdjustProbabilityByDistance(detectionProbability, distance, detectionRadius);

            if (IsPlayerDetected(detectionProbability)) {
                RE::ConsoleLog::GetSingleton()->Print(
                    ("NPC " + std::to_string(npc->GetFormID()) + " detected the player!").c_str());
                playerDetected = true;
                return BSContainer::ForEachResult::kStop;
            }
        }

        return BSContainer::ForEachResult::kContinue;
    });

    return playerDetected;
}


void UpdateDisguiseValue(Actor *actor) {
    // Calculate the disguise value based on equipped armor
    float disguiseValue = CalculateDisguiseValue(actor);

    // Set the actor's custom disguise value
    actor->AsActorValueOwner()->SetActorValue(kDisguiseValue, disguiseValue);

    RE::ConsoleLog::GetSingleton()->Print(("Current Disguise Value: " + std::to_string(disguiseValue)).c_str());

    // Get the faction corresponding to the armor worn by the player
    RE::TESFaction *faction = GetFactionByArmorTag(actor);
    if (faction) {
        // Modify the faction detection based on the disguise value
        ModifyFactionDetection(actor, disguiseValue, faction);
    } else {
        RE::ConsoleLog::GetSingleton()->Print("No faction found!");
    }

    bool playerDetected = CheckNPCDetection(actor, disguiseValue);
    if (playerDetected) {
        // Player is detected by NPCs, modify behavior or remove from faction if necessary
        RE::ConsoleLog::GetSingleton()->Print("Player detected by nearby NPCs!");

        // Example: Remove player from faction when detected
        if (faction) {
            actor->AddToFaction(faction, -1);  // Remove from faction if detected
            RE::ConsoleLog::GetSingleton()->Print("Player removed from faction due to detection.");
        }
    } else {
        RE::ConsoleLog::GetSingleton()->Print("Player remains undetected.");
    }
}