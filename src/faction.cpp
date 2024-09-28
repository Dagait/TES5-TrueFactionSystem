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
    /* TODO: Implement NPC faction detection logic:
     * 1. The higher the disguise value, the less likely NPCs will detect the player as an enemy.
     *    - NPCs should detect the player within a smaller detection radius if the disguise value is high.
     *
     * 2. Add the player to the relevant faction if they are wearing faction armor,
     *    even if they are only wearing one piece of armor.
     *
     * 3. Probability-based detection:
     *    - Example: If the player has a disguise value of 80, the NPC will only notice that
     *      the player is not really part of the faction when the player gets too close.
     *    - The probability of detection increases based on the player's proximity to the NPC,
     *      and this should be dynamically calculated based on the player's disguise value.
     *
     * 4. Random removal from faction:
     *    - If the player gets too close and is detected, they should be randomly removed from the faction
     *      (e.g., a 20% chance of being detected and removed).
     *
     * 5. Optional improvements:
     *    - Implement a mechanic where the disguise value decreases based on the player's actions (e.g., aggressive
     * actions or loud behavior).
     *    - If the disguise value is high, the player could walk past faction NPCs without arousing suspicion.
     *    - Add visual or audio cues to warn the player when they are about to be detected.
     */

    // Step 1: Modify faction detection based on disguise value
    if (faction) {
        if (disguiseValue <= 60) {
            // If the player's disguise value is too low, remove them from the faction
            if (actor->IsInFaction(faction)) {
                actor->AddToFaction(faction, -1);
                RE::ConsoleLog::GetSingleton()->Print("Player removed from faction due to low disguise.");
            }
        } else {
            // If the player's disguise value is high enough, add them to the faction
            if (!actor->IsInFaction(faction)) {
                actor->AddToFaction(faction, 1);
                RE::ConsoleLog::GetSingleton()->Print("Player added to faction due to high disguise.");
            }
        }
    }
}
