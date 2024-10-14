#pragma once
#include "Config.h"
#include "Faction.h"


namespace NPE {
    class DisguiseManager {
    public:
        static DisguiseManager &GetInstance() {
            static DisguiseManager instance;
            return instance;
        }
        /**
         * @brief Calculate the disguise value for a given actor and faction.
         *
         * @param actor The actor whose disguise value is to be calculated.
         * @param faction The faction that the actor is disguising as.
         */
        void CalculateDisguiseValue(RE::Actor *actor, RE::TESFaction *faction);
        float GetDisguiseValueForFaction(RE::TESFaction *faction);
        float GetDisguiseBonusValueForFaction(RE::TESFaction *faction);
        float GetRaceBonusValueForFaction(RE::TESFaction *faction);
        void AddArmorSetBonus(RE::Actor *actor);

    private:
        DisguiseManager() = default;
    };
}
