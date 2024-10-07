#pragma once
#include "Faction.h"
#include "Combat.h"
#include "NpcDetectionData.h"
#include "DisguiseData.h"
#include "ArmorSlots.h"

#include <cmath>
#include <random>
#include <unordered_map>
#include <future>
#include <vector>
#include <thread>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

extern std::unordered_map<RE::FormID, NPCDetectionData> recognizedNPCs;
extern PlayerDisguiseStatus playerDisguiseStatus;

// Disguise related functions
/**
 * @brief Calculate the disguise value for a given actor and faction.
 *
 * @param actor The actor whose disguise value is to be calculated.
 * @param faction The faction that the actor is disguising as.
 */
void CalculateDisguiseValue(Actor* actor, TESFaction* faction);

/**
 * @brief Update the disguise value based on the actor's equipment.
 *
 * @param actor The actor whose disguise is being updated.
 */
void UpdateDisguiseValue(Actor* actor);

/**
 * @brief Check if the player is detected by NPCs based on disguise value.
 *
 * @param actor The actor (usually the player) being detected by NPCs.
 */
void CheckNPCDetection(Actor* actor);

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

/**
 * @brief Check if an NPC recognizes the player based on faction and disguise.
 *
 * @param npc The NPC checking for the player.
 * @param player The player actor.
 * @param faction The faction being checked for disguise.
 * @return true If the NPC recognizes the player.
 * @return false If the NPC does not recognize the player.
 */
bool NPCRecognizesPlayer(Actor* npc, Actor* player, TESFaction* faction);

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

// Field of view and line of sight functions
/**
 * @brief Check if the player is within the NPC's field of view.
 *
 * @param npc The NPC whose field of view is being checked.
 * @param player The player actor.
 * @param fieldOfViewDegrees The angle of the NPC's field of view.
 * @return true If the player is within the NPC's field of view.
 * @return false If the player is outside the field of view.
 */
bool IsInFieldOfView(Actor* npc, Actor* player, float fieldOfViewDegrees = 90.0f);

/**
 * @brief Check if the player is within the NPC's line of sight.
 *
 * @param npc The NPC whose line of sight is being checked.
 * @param player The player actor.
 * @return true If the player is in the NPC's line of sight.
 * @return false If the player is outside the NPC's line of sight.
 */
bool IsInLineOfSight(Actor* npc, Actor* player);

/**
 * @brief Check if it is currently night time in the game.
 *
 * @return true If it is night time.
 * @return false If it is not night time.
 */
bool IsNightTime();

/**
 * @brief Check if the player is in a dark area, such as an interior or shadowy region.
 *
 * @param player The player actor.
 * @return true If the player is in a dark area.
 * @return false If the player is not in a dark area.
 */
bool IsPlayerInDarkArea(Actor* player);

/**
 * @brief Remove the player from all relevant factions.
 *
 * @param player The player actor.
 */
void RemovePlayerFromAllFactions(Actor* player);

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
bool IsPlayerNearLightSource(RE::Actor* player, float radius = 100.0f);

/**
 *
 */
void AddArmorSetBonus(RE::Actor* actor);

/**
 *
 */
float GetRaceBonusValueForFaction(RE::TESFaction* faction);