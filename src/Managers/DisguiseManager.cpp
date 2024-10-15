#include "Managers/DisguiseManager.h"
#include "Globals.h"


namespace NPE {


    void DisguiseManager::UpdateDisguiseValue(RE::Actor *actor) {
        constexpr const char *COVERED_FACE_TAG = "npeCoveredFace";
        std::unordered_map<RE::TESFaction *, float> factionDisguiseValues;

        // Gather the factions by armor tags
        std::vector<RE::TESFaction *> factions = GetFactionsByArmorTags(actor);
        if (factions.empty()) {
            playerDisguiseStatus.Clear();
            RemovePlayerFromAllFactions(actor);
            return;
        }

        this->ClearArmorDisguiseValues(actor);

        // Iterate through armor slots and calculate values per faction
        for (const auto &slot : armorSlotsSlot) {
            RE::TESObjectARMO *armor = actor->GetWornArmor(slot.slot);
            if (!armor) {
                continue;
            }

            for (RE::TESFaction *faction : factions) {
                if (!faction) {
                    continue;
                }

                std::string factionTag = GetTagForFaction(faction);
                if (factionTag.empty()) {
                    continue;
                }

                // Check if the armor has a keyword for the faction
                if (armor->HasKeywordString(factionTag)) {
                    // Add armor piece value to the faction
                    factionDisguiseValues[faction] += slot.weight;

                    // Check if the armor covers the face and add a bonus
                    if (armor->HasKeywordString(COVERED_FACE_TAG)) {
                        factionDisguiseValues[faction] += 12;
                    }
                }
            }
        }

        // Process each faction's disguise value
        for (auto &[faction, disguiseValue] : factionDisguiseValues) {
            disguiseValue += playerDisguiseStatus.GetRaceBonusValue(faction);
            disguiseValue += playerDisguiseStatus.GetBonusValue(faction);
            disguiseValue = std::clamp(disguiseValue, 0.0f, 100.0f);

            playerDisguiseStatus.SetDisguiseValue(faction, disguiseValue);

            // Add or remove the actor from factions based on disguise value
            if (!actor->IsInFaction(faction) && disguiseValue > 5.0f) {
                actor->AddToFaction(faction, 1);
            } else if (disguiseValue <= 5.0f) {
                actor->AddToFaction(faction, -1);
                playerDisguiseStatus.RemoveDisguiseValue(faction);
            }
        }

        detectionManager.CheckNPCDetection(actor);
    }


    void DisguiseManager::ClearArmorDisguiseValues(RE::Actor *actor) {
        for (const auto &[factionTag, factionID] : factionArmorKeywords) {
            RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
            if (faction) {
                playerDisguiseStatus.SetBonusValue(faction, 0.0f);
                playerDisguiseStatus.SetDisguiseValue(faction, 0.0f);
            }
        }
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