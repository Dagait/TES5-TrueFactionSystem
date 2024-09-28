#include "faction.h"

const std::vector<std::pair<std::string, RE::FormID>> factionArmorTags = {
    {"npeBanditFaction", 0x0001BCC0},      // Bandit faction
    {"npeImperialFaction", 0x0002BF9A},    // Imperial faction
    {"npeStormcloakFaction", 0x00028849},  // Stormcloak faction
};

std::vector<std::pair<std::string, RE::TESFaction *>> GetRelevantFactions() {
    // Hier fügen wir die relevanten Fraktionen hinzu
    std::vector<std::pair<std::string, RE::TESFaction *>> factions;

    // BanditFaction
    RE::TESFaction *banditFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0001BCC0);
    if (banditFaction) {
        factions.push_back({"BanditFaction", banditFaction});
    }

    // Imperial Legion
    RE::TESFaction *imperialFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002BF9A);
    if (imperialFaction) {
        factions.push_back({"ImperialFaction", imperialFaction});
    }

    // Stormcloaks
    RE::TESFaction *stormcloakFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00028849);
    if (stormcloakFaction) {
        factions.push_back({"StormcloakFaction", stormcloakFaction});
    }

    return factions;
}

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
