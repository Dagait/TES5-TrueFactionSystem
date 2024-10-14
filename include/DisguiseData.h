#pragma once

namespace NPE {
    /**
     * @brief Struct that holds the disguise data for a specific faction.
     *
     * This contains the faction and the player's disguise value associated with it.
     */
    struct DisguiseData {
        RE::TESFaction* faction;         /// Pointer to the faction associated with the disguise.
        float disguiseValue;             /// The player's disguise value for the faction.
        float bonusValue = 0.0f;         /// Bonus for armor sets
        float raceDisguiseBonus = 0.0f;  /// Bonus for the disguise if the player is a certain race
    };

    /**
     * @brief Class to manage player's disguise status across different factions.
     */
    class PlayerDisguiseStatus {
    public:
        void SetDisguiseValue(RE::TESFaction* faction, float value);
        float GetDisguiseValue(RE::TESFaction* faction) const;

        void SetBonusValue(RE::TESFaction* faction, float bonus);
        float GetBonusValue(RE::TESFaction* faction) const;

        void SetRaceBonusValue(RE::TESFaction* faction, float bonus);
        float GetRaceBonusValue(RE::TESFaction* faction) const;

        void RemoveDisguiseValue(RE::TESFaction* faction);
        void Clear();

        void Save(SKSE::SerializationInterface* a_intfc);
        void Load(SKSE::SerializationInterface* a_intfc);

    private:
        std::unordered_map<RE::TESFaction*, DisguiseData> factionDisguiseMap;
    };

}