#include "faction.h"

const std::vector<std::pair<std::string, RE::FormID>> factionArmorTags = {
    {"npeBanditArmor", 0x0001BCC0},      // Bandit faction
    {"npeImperialArmor", 0x0001C21C},    // Imperial faction
    {"npeStormcloakArmor", 0x0001C21E},  // Stormcloak faction
};

RE::TESFaction *GetFactionByArmorTag(RE::Actor *actor) {
    if (!actor) {
        return nullptr;
    }

    std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> armorSlots = {
        RE::BGSBipedObjectForm::BipedObjectSlot::kHead, RE::BGSBipedObjectForm::BipedObjectSlot::kBody,
        RE::BGSBipedObjectForm::BipedObjectSlot::kHands, RE::BGSBipedObjectForm::BipedObjectSlot::kFeet};

    for (auto &slot : armorSlots) {
        RE::TESObjectARMO *wornArmor = actor->GetWornArmor(slot);
        if (wornArmor) {
            for (const auto &[tag, factionID] : factionArmorTags) {
                if (wornArmor->HasKeywordString(tag)) {
                    return RE::TESForm::LookupByID<RE::TESFaction>(factionID);
                }
            }
        }
    }

    return nullptr;
}

void ModifyFactionDetection(Actor *actor, float disguiseValue, RE::TESFaction *faction) {
    if (faction && actor->IsInFaction(faction) && disguiseValue <= 60) {
        actor->AddToFaction(faction, -1);
    } else if (faction && !actor->IsInFaction(faction) && disguiseValue > 60) {
        actor->AddToFaction(faction, 1);
    }
}
