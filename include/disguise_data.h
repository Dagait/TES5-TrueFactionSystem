#pragma once
#include <unordered_map>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

/**
 * @brief Struct that holds the disguise data for a specific faction.
 *
 * This contains the faction and the player's disguise value associated with it.
 */
struct DisguiseData {
    RE::TESFaction* faction;    /// Pointer to the faction associated with the disguise.
    float disguiseValue;        /// The player's disguise value for the faction.
    float bonusValue = 0.0f;    /// Bonus for armor sets
};

/**
 * @brief Class to manage player's disguise status across different factions.
 *
 * This class provides methods to set, get, remove, and manage the player's disguise values for multiple factions.
 */
struct PlayerDisguiseStatus {
    /// Map that holds the disguise data for each faction.
    std::unordered_map<RE::TESFaction*, DisguiseData> factionDisguiseMap;

    /**
     * @brief Sets the disguise value for a given faction.
     *
     * @param faction The faction to set the disguise value for.
     * @param value The disguise value to assign.
     */
    void SetDisguiseValue(RE::TESFaction* faction, float value);

    /**
     * @brief Gets the disguise value for a given faction.
     *
     * @param faction The faction to retrieve the disguise value from.
     * @return float The disguise value if it exists, otherwise 0.0f.
     */
    float GetDisguiseValue(RE::TESFaction* faction) const;

    /**
     * @brief Sets the bonus value for a given faction.
     *
     * @param faction The faction to set the bonus value for.
     * @param bonus The bonus value to assign.
     */
    void SetBonusValue(RE::TESFaction* faction, float bonus);

    /**
     * @brief Gets the bonus value for a given faction.
     *
     * @param faction The faction to retrieve the bonus value from.
     * @return float The bonus value if it exists, otherwise 0.0f.
     */
    float GetBonusValue(RE::TESFaction* faction) const;

    /**
     * @brief Removes the disguise value for a given faction.
     *
     * @param faction The faction to remove the disguise value from.
     */
    void RemoveDisguiseValue(RE::TESFaction* faction);

    /**
     * @brief Clears all disguise values.
     *
     * Resets the player's disguise status by clearing all entries in the map.
     */
    void Clear();

    /**
     * @brief Saves the player's disguise status to a file.
     *
     * @param a_intfc The SKSE serialization interface used to write the data.
     */
    void Save(SKSE::SerializationInterface* a_intfc);

    /**
     * @brief Loads the player's disguise status from a file.
     *
     * @param a_intfc The SKSE serialization interface used to read the data.
     */
    void Load(SKSE::SerializationInterface* a_intfc);
};
