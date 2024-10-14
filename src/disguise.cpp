#include "Disguise.h"
#include "Globals.h"


namespace NPE {
    float GetDisguiseValueForFaction(RE::TESFaction *faction) { return playerDisguiseStatus.GetDisguiseValue(faction); }

    float GetDisguiseBonusValueForFaction(RE::TESFaction *faction) {
        return playerDisguiseStatus.GetBonusValue(faction);
    }

    float GetRaceBonusValueForFaction(RE::TESFaction *faction) {
        return playerDisguiseStatus.GetRaceBonusValue(faction);
    }

    void AddArmorSetBonus(RE::Actor *actor) {
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

    float GetDetectionProbability(float disguiseValue) { return abs(100.0f - disguiseValue); }

    float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
        float distanceFactor = 1.0f - (distance / maxDistance);  // Scaled by 1 (near) to 0 (far)
        return detectionProbability * distanceFactor;
    }

    void RemovePlayerFromAllFactions(RE::Actor *player) {
        auto factions = GetRelevantFactions();
        for (auto &[factionName, faction] : factions) {
            if (player->IsInFaction(faction)) {
                player->AddToFaction(faction, -1);
            }
        }
    }

    void UpdateDisguiseValue(RE::Actor *actor) {
        std::vector<RE::TESFaction *> factions = GetFactionsByArmorTags(actor);
        if (factions.empty()) {
            playerDisguiseStatus.Clear();
            RemovePlayerFromAllFactions(actor);
            return;
        }

        ClearArmorDisguiseValues(actor);

        for (RE::TESFaction *faction : factions) {
            if (!faction) {
                continue;
            }

            disguiseManager.CalculateDisguiseValue(actor, faction);

            float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);

            if (!actor->IsInFaction(faction) && disguiseValue > 5.0f) {
                actor->AddToFaction(faction, 1);
            } else {
                if (disguiseValue <= 5.0f) {
                    actor->AddToFaction(faction, -1);
                    playerDisguiseStatus.RemoveDisguiseValue(faction);
                }
            }
        }

        detectionManager.CheckNPCDetection(actor);
    }

    void SaveDetectionData(SKSE::SerializationInterface *a_intfc) {
        for (auto &[npcID, detectionData] : recognizedNPCs) {
            a_intfc->WriteRecord('NPCD', 1, &npcID, sizeof(npcID));
            a_intfc->WriteRecordData(&detectionData, sizeof(detectionData));
        }
    }

    void ClearArmorDisguiseValues(RE::Actor *actor) {
        for (const auto &[factionTag, factionID] : factionArmorKeywords) {
            RE::TESFaction *faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
            if (faction) {
                playerDisguiseStatus.SetBonusValue(faction, 0.0f);
                playerDisguiseStatus.SetDisguiseValue(faction, 0.0f);
            }
        }
    }

    void LoadDetectionData(SKSE::SerializationInterface *a_intfc) {
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;

        while (a_intfc->GetNextRecordInfo(type, version, length)) {
            RE::FormID npcID;
            NPCDetectionData detectionData;

            if (a_intfc->ReadRecordData(&npcID, sizeof(npcID))) {
                if (length == sizeof(detectionData)) {
                    if (a_intfc->ReadRecordData(&detectionData, sizeof(detectionData))) {
                        recognizedNPCs[npcID] = detectionData;
                    }
                } else {
                    RE::ConsoleLog::GetSingleton()->Print("Record length mismatch!");
                }
            }
        }
    }
}