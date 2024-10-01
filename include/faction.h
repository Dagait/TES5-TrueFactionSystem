#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"


using namespace RE;

// Faction utility functions
/**
 * @brief Get the faction based on the actor's worn armor tags.
 *
 * @param actor The actor whose armor is being evaluated.
 * @return TESFaction* The corresponding faction if found, nullptr otherwise.
 */
TESFaction* GetFactionByArmorTag(Actor* actor);

/**
 * @brief Get a list of factions relevant to the current game context.
 *
 * @return std::vector<std::pair<std::string, TESFaction*>> A list of factions with their corresponding string tags.
 */
std::vector<std::pair<std::string, TESFaction*>> GetRelevantFactions();

/**
 * @brief Get the tag associated with a given faction.
 *
 * @param faction The faction whose tag is being retrieved.
 * @return std::string The tag corresponding to the faction, or an empty string if not found.
 */
std::string GetTagForFaction(TESFaction* faction);

/**
 * @brief Get a list of factions based on the armor tags worn by the actor.
 *
 * @param actor The actor whose armor is being checked for faction tags.
 * @return std::vector<TESFaction*> A list of factions based on the actor's worn armor.
 */
std::vector<TESFaction*> GetFactionsByArmorTags(Actor* actor);


/**
 * 
 */
std::vector<RE::TESFaction*> GetFactionsForActor(RE::Actor* actor);


/**
 *
 */
RE::TESFaction* GetFactionByEditorID(RE::BSFixedString factionName);


/**
 *
 */
RE::BSFixedString GetFactionEditorID(RE::TESFaction* faction);