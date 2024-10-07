#include "Race.h"


RE::TESRace *GetPlayerRace() { 
	RE::PlayerCharacter *player = RE::PlayerCharacter::GetSingleton();
    if (player) {
        return player->GetRace();
    }
    return nullptr;
}

void InitRaceDisguiseBonus() { 
    // Compare agains the factionArmorKeywords { { "npeKeyword", 0x000XXXXX }, .... }
    for (const auto &[tag, factionID] : factionArmorKeywords) {
        if (IsPlayerInCorrectRace(tag)) {
            RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
            playerDisguiseStatus.SetRaceBonusValue(faction, 20);
        }
    }
}

bool IsPlayerInCorrectRace(RE::BSFixedString keyword) { 
    RE::TESRace *playerRace = GetPlayerRace(); 
    RE::BGSKeyword **raceKeywords = playerRace->keywords;
    for (uint32_t i = 0; i < playerRace->numKeywords; i++) {
        if (raceKeywords[i]->GetFormEditorID() == keyword) {
            return true;
        }
    }
    return false;
}