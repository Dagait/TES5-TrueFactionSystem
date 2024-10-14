#include "Managers/DisguiseManager.h"
#include "Globals.h"


namespace NPE {
    void DisguiseManager::CalculateDisguiseValue(RE::Actor *actor, RE::TESFaction *faction) {
        float disguiseValue = 0.0f;
        constexpr const char *COVERED_FACE_TAG = "npeCoveredFace";

        std::string factionTag = GetTagForFaction(faction);
        if (factionTag.empty()) {
            return;
        }

        disguiseValue += playerDisguiseStatus.GetRaceBonusValue(faction);
        disguiseValue += playerDisguiseStatus.GetBonusValue(faction);

        for (const auto &slot : armorSlotsSlot) {
            RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
            if (!armor) {
                continue;
            }

            // Get Keyword linked to the factionFormID from factionFormIDToTagMap {{factionFormID, factionKeyword},...}
            auto it = factionFormIDToTagMap.find(faction->GetFormID());
            if (it == factionFormIDToTagMap.end()) {
                continue;
            }
            std::string factionKeyword = it->second;

            if (armor->HasKeywordString(COVERED_FACE_TAG) && armor->HasKeywordString(factionKeyword)) {
                disguiseValue += 100;
            }

            if (armor->HasKeywordString(factionTag)) {
                disguiseValue += slot.weight;
            }
        }

        disguiseValue = std::clamp(disguiseValue, 0.0f, 100.0f);

        playerDisguiseStatus.SetDisguiseValue(faction, disguiseValue);
        AddArmorSetBonus(actor);
    }

    float DisguiseManager::GetDisguiseValueForFaction(RE::TESFaction *faction) {
        return playerDisguiseStatus.GetDisguiseValue(faction);
    }

    float DisguiseManager::GetRaceBonusValueForFaction(RE::TESFaction *faction) {
        return playerDisguiseStatus.GetRaceBonusValue(faction);
    }

    float DisguiseManager::GetDisguiseBonusValueForFaction(RE::TESFaction *faction) {
        return playerDisguiseStatus.GetBonusValue(faction);
    }

    void DisguiseManager::AddArmorSetBonus(RE::Actor *actor) {
        for (const auto &[factionTag, factionID] : factionArmorKeywords) {
            int matchingArmorPieces = 0;
            const int totalArmorSlots = 5;

            for (const auto &slot : armorSlotsSlot) {
                RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
                if (armor && armor->HasKeywordString(factionTag)) {
                    matchingArmorPieces++;
                }
            }

            float bonusAmount = 0.0f;

            if (matchingArmorPieces >= 4) {
                bonusAmount = 20.0f;
            } else if (matchingArmorPieces >= 3) {
                bonusAmount = 10.0f;
            } else if (matchingArmorPieces >= 2) {
                bonusAmount = 5.0f;
            }

            // If any bonus is applied, increase the actor's ArmorRating
            if (bonusAmount > 0.0f) {
                RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
                playerDisguiseStatus.SetBonusValue(faction, bonusAmount);
            }
        }
    }
}