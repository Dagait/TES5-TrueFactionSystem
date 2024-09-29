#include "faction.h"

const std::vector<std::pair<std::string, RE::FormID>> factionArmorTags = {
    {"npeBanditFaction", 0x0001BCC0},
    {"npeImperialFaction", 0x0002BF9A}, 
    {"npeBladesFaction", 0x00072834},
    {"npeCompanionsFaction", 0x00048362}, 
    {"npeDawnguardFaction", 0x00014217}, 
    {"npeDawnstarFaction", 0x0002816E},
    {"npeFalkreathFaction", 0x00028170},  
    {"npeForswornFaction", 0x00043599},  
    {"npeMarkarthFaction", 0x0002816C},
    {"npeMorthalFaction", 0x0002816D},    
    {"npeNightingaleFaction", 0x000DEED6}, 
    {"npeRiftenFaction", 0x0002816B},
    {"npeSolitudeFaction", 0x00029DB0},  
    {"npeSolstheimFaction", 0x00018279},   
    {"npeStormcloaksFaction", 0x00028849},
    {"npeWhiterunFaction", 0x000267EA},   
    {"npeWindhelmFaction", 0x000267E3},      
    {"npeWinterholdFaction", 0x00014217}
};

static std::unordered_map<RE::FormID, std::string> factionFormIDToTagMap = {
    {0x0001BCC0, "npeBanditFaction"},
    {0x0002BF9A, "npeImperialFaction"},    {0x00072834, "npeBladesFaction"},
    {0x00048362, "npeCompanionsFaction"}, {0x00014217, "npeDawnguardFaction"},   {0x0002816E, "npeDawnstarFaction"},
    {0x00028170, "npeFalkreathFaction"},  {0x00043599, "npeForswornFaction"},    {0x0002816C, "npeMarkarthFaction"},
    {0x0002816D, "npeMorthalFaction"},    {0x000DEED6, "npeNightingaleFaction"}, {0x0002816B, "npeRiftenFaction"},
    {0x00029DB0, "npeSolitudeFaction"},   {0x00018279, "npeSolstheimFaction"},   {0x00028849, "npeStormcloaksFaction"},
    {0x000267EA, "npeWhiterunFaction"},   {0x000267E3, "npeWindhelmFaction"},    {0x00014217, "npeWinterholdFaction"}};

const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> allArmorSlots = {
    RE::BGSBipedObjectForm::BipedObjectSlot::kHead,     RE::BGSBipedObjectForm::BipedObjectSlot::kBody,
    RE::BGSBipedObjectForm::BipedObjectSlot::kHands,    RE::BGSBipedObjectForm::BipedObjectSlot::kFeet,
    RE::BGSBipedObjectForm::BipedObjectSlot::kForearms, RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet,
    RE::BGSBipedObjectForm::BipedObjectSlot::kHair};

std::vector<std::pair<std::string, RE::TESFaction *>> GetRelevantFactions() {
    std::vector<std::pair<std::string, RE::TESFaction *>> factions;

    RE::TESFaction *banditFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0001BCC0);
    if (banditFaction) {
        factions.push_back({"BanditFaction", banditFaction});
    }

    RE::TESFaction *imperialFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002BF9A);
    if (imperialFaction) {
        factions.push_back({"ImperialFaction", imperialFaction});
    }

    RE::TESFaction *stormcloakFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00028849);
    if (stormcloakFaction) {
        factions.push_back({"StormcloakFaction", stormcloakFaction});
    }

    RE::TESFaction *bladesFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00072834);
    if (bladesFaction) {
        factions.push_back({"BladesFaction", bladesFaction});
    }

    RE::TESFaction *companionsFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00048362);
    if (companionsFaction) {
        factions.push_back({"CompanionsFaction", companionsFaction});
    }

    RE::TESFaction *dawnguardFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00014217);
    if (dawnguardFaction) {
        factions.push_back({"DawnguardFaction", dawnguardFaction});
    }

    RE::TESFaction *dawnstarFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002816E);
    if (dawnstarFaction) {
        factions.push_back({"DawnstarFaction", dawnstarFaction});
    }

    RE::TESFaction *falkreathFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00028170);
    if (falkreathFaction) {
        factions.push_back({"FalkreathFaction", falkreathFaction});
    }

    RE::TESFaction *forswornFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00043599);
    if (forswornFaction) {
        factions.push_back({"ForswornFaction", forswornFaction});
    }

    RE::TESFaction *markarthFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002816C);
    if (markarthFaction) {
        factions.push_back({"MarkarthFaction", markarthFaction});
    }

    RE::TESFaction *morthalFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002816D);
    if (morthalFaction) {
        factions.push_back({"MorthalFaction", morthalFaction});
    }

    RE::TESFaction *nightingaleFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x000DEED6);
    if (nightingaleFaction) {
        factions.push_back({"NightingaleFaction", nightingaleFaction});
    }

    RE::TESFaction *riftenFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x0002816B);
    if (riftenFaction) {
        factions.push_back({"RiftenFaction", riftenFaction});
    }

    RE::TESFaction *solitudeFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00029DB0);
    if (solitudeFaction) {
        factions.push_back({"SolitudeFaction", solitudeFaction});
    }

    RE::TESFaction *solstheimFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00018279);
    if (solstheimFaction) {
        factions.push_back({"SolstheimFaction", solstheimFaction});
    }

    RE::TESFaction *whiterunFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x000267EA);
    if (whiterunFaction) {
        factions.push_back({"WhiterunFaction", whiterunFaction});
    }

    RE::TESFaction *windhelmFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x000267E3);
    if (windhelmFaction) {
        factions.push_back({"WindhelmFaction", windhelmFaction});
    }

    RE::TESFaction *winterholdFaction = RE::TESForm::LookupByID<RE::TESFaction>(0x00014217);
    if (winterholdFaction) {
        factions.push_back({"WinterholdFaction", winterholdFaction});
    }

    return factions;
}

std::string GetTagForFaction(RE::TESFaction *faction) {
    if (faction) {
        auto it = factionFormIDToTagMap.find(faction->GetFormID());
        if (it != factionFormIDToTagMap.end()) {
            return it->second;
        }
    }
    return "";
}

RE::TESFaction *GetFactionByFormID(RE::FormID *formID) { return nullptr; }

std::vector<RE::TESFaction *> GetFactionsByArmorTags(RE::Actor *actor) {
    if (!actor) {
        return {};
    }

    std::set<RE::TESFaction *> factionsSet;  // Verwende ein Set, um Duplikate zu vermeiden

    for (auto slot : allArmorSlots) {
        RE::TESObjectARMO *wornArmor = actor->GetWornArmor(slot);
        if (wornArmor) {
            for (const auto &[tag, factionID] : factionArmorTags) {
                if (wornArmor->HasKeywordString(tag)) {
                    RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
                    if (faction) {
                        factionsSet.insert(faction);
                    }
                }
            }
        }
    }

    return std::vector<RE::TESFaction *>(factionsSet.begin(), factionsSet.end());
}

RE::TESFaction *GetFactionByArmorTag(RE::Actor *actor) {
    if (!actor) {
        return nullptr;
    }

    std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> armorSlots = {
        RE::BGSBipedObjectForm::BipedObjectSlot::kHead, RE::BGSBipedObjectForm::BipedObjectSlot::kBody,
        RE::BGSBipedObjectForm::BipedObjectSlot::kHands, RE::BGSBipedObjectForm::BipedObjectSlot::kFeet,
        RE::BGSBipedObjectForm::BipedObjectSlot::kForearms, RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet,
        RE::BGSBipedObjectForm::BipedObjectSlot::kHair
    };

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
