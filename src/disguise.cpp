#include "disguise.h"

#include "faction.h"

constexpr RE::ActorValue kDisguiseValue = static_cast<RE::ActorValue>(0);  // Custom value for disguise


float CalculateDisguiseValue(Actor *actor) {
    float disguiseValue = 0.0f;

    // List of armor weights
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

bool CheckNPCDetection(Actor *actor, float disguiseValue) {
    // Set the detection probability based on the player's disguise value
    float detectionProbability = 100.0f - disguiseValue;  // Inverse of disguise value
    float detectionRadius = 2000.0f;                      // Maximum detection radius around the player

    // Get the player's current cell
    RE::TESObjectCELL *playerCell = actor->GetParentCell();
    if (!playerCell) return false;

    // Iterate through all references in the player's current cell
    auto &references = playerCell->GetRuntimeData().references;
    for (auto &refHandle : references) {
        RE::TESObjectREFR *reference = refHandle.get();
        if (!reference) continue;

        // Check if the reference is an Actor
        RE::Actor *npc = reference->As<RE::Actor>();
        if (!npc || npc == actor) continue;  // Skip if it's the player or not an NPC

        // Get the distance between the player and the NPC
        float distance = npc->GetPosition().GetDistance(actor->GetPosition());

        // Only check NPCs within the detection radius
        if (distance < detectionRadius) {
            // Higher proximity increases detection probability
            float proximityFactor = (detectionRadius - distance) / detectionRadius;
            float finalProbability = detectionProbability * proximityFactor;

            // Simulate detection based on probability
            float randomChance = static_cast<float>(rand() % 100);
            if (randomChance < finalProbability) {
                RE::ConsoleLog::GetSingleton()->Print(
                    ("Player detected by NPC at distance: " + std::to_string(distance)).c_str());
                return true;  // Player is detected by an NPC
            }
        }
    }

    // Player is not detected by any NPCs within the radius
    return false;
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

    // **New part: Check for NPC detection in the player's vicinity**
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