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

void UpdateDisguiseValue(Actor *actor) {
    float disguiseValue = CalculateDisguiseValue(actor);
    actor->AsActorValueOwner()->SetActorValue(kDisguiseValue, disguiseValue);

    RE::ConsoleLog::GetSingleton()->Print(("Current Disguise Value: " + std::to_string(disguiseValue)).c_str());
    RE::TESFaction *faction = GetFactionByArmorTag(actor);
    if (faction) {
        ModifyFactionDetection(actor, disguiseValue, faction);
    } else {
        RE::ConsoleLog::GetSingleton()->Print("No faction found!");
    }
}
