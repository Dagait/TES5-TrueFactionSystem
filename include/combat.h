#pragma once
#include "RE/C/Calendar.h"
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

/**
 * @brief Event handler for handling hit events in the game.
 *
 * This class handles TESHitEvent, which occurs when an actor is hit.
 */
class HitEventHandler : public RE::BSTEventSink<RE::TESHitEvent> {
public:
    /**
     * @brief Processes the hit event when a TESHitEvent is fired.
     *
     * @param evn The hit event containing information about the hit.
     * @param dispatcher The event dispatcher responsible for sending the event.
     * @return RE::BSEventNotifyControl The result of event processing (continue or stop).
     */
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* evn,
                                                  RE::BSTEventSource<RE::TESHitEvent>* dispatcher) override;
};

/**
 * @brief Retrieves the faction of a given actor.
 *
 * @param actor The actor whose faction is being retrieved.
 * @return RE::TESFaction* The faction the actor belongs to, or nullptr if none.
 */
RE::TESFaction* GetFactionByActor(RE::Actor* actor);

/**
 * @brief Checks and re-adds the player to a faction if certain conditions are met.
 *
 * This function checks if the player should be re-added to a faction after a cooldown period.
 *
 * @param player The player actor.
 */
void CheckAndReAddPlayerToFaction(RE::Actor* player);

/**
 * @brief Initiates combat between an NPC and the player.
 *
 * @param npc The non-player character (NPC) initiating combat.
 * @param player The player character.
 * @param npcFaction The faction the NPC belongs to.
 */
void StartCombat(RE::Actor* npc, RE::Actor* player, RE::TESFaction* npcFaction);
