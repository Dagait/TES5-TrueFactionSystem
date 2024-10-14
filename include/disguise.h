#pragma once
#include "Faction.h"
#include "Combat.h"
#include "Actor/NPEActor.h"

#include "Config.h"


namespace NPE {
    // Disguise related functions

    /**
     * @brief Update the disguise value based on the actor's equipment.
     *
     * @param actor The actor whose disguise is being updated.
     */
    void UpdateDisguiseValue(RE::Actor* actor);

    // Detection and probability functions
    /**
     * @brief Calculate the probability of detection based on disguise value.
     *
     * @param disguiseValue The disguise value of the actor.
     * @return float The probability of being detected.
     */
    float GetDetectionProbability(float disguiseValue);

    /**
     * @brief Adjust the detection probability based on distance from NPC.
     *
     * @param detectionProbability The initial detection probability.
     * @param distance The distance between the actor and the NPC.
     * @param maxDistance The maximum detection range.
     * @return float The adjusted detection probability.
     */
    float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance);

    // Data serialization functions
    /**
     * @brief Save the current NPC detection data for serialization.
     *
     * @param a_intfc The serialization interface.
     */
    void SaveDetectionData(SKSE::SerializationInterface* a_intfc);

    /**
     * @brief Load NPC detection data from serialized data.
     *
     * @param a_intfc The serialization interface.
     */
    void LoadDetectionData(SKSE::SerializationInterface* a_intfc);

    /**
     * @brief Check if the player is within the NPC's line of sight.
     *
     * @param npc The NPC whose line of sight is being checked.
     * @param player The player actor.
     * @return true If the player is in the NPC's line of sight.
     * @return false If the player is outside the NPC's line of sight.
     */
    bool IsInLineOfSight(RE::Actor* npc, RE::Actor* player);

    /**
     * @brief Remove the player from all relevant factions.
     *
     * @param player The player actor.
     */
    void RemovePlayerFromAllFactions(RE::Actor* player);

    /**
     *
     */
    float GetDisguiseValueForFaction(RE::TESFaction* faction);

    /**
     *
     */
    float GetDisguiseBonusValueForFaction(RE::TESFaction* faction);

    /**
     * @brief Check if the current weather conditions are valid for detection.
     *
     * @param weather The current weather object.
     * @return true If the weather is valid for detection.
     * @return false If the weather reduces detection probability.
     */
    bool IsBadWeather(RE::TESWeather* currentWeather);

    /**
     *
     */
    void AddArmorSetBonus(RE::Actor* actor);

    /**
     *
     */
    float GetRaceBonusValueForFaction(RE::TESFaction* faction);

    /**
     *
     */
    void ClearArmorDisguiseValues(RE::Actor* actor);
}