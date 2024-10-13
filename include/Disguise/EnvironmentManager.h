#pragma once
#include "Config.h"
#include "Datahandler/Datahandler.h"
#include "NpcDetectionData.h"


class EnvironmentManager {
public:
    static EnvironmentManager &GetInstance() {
        static EnvironmentManager instance;
        return instance;
    }
    /**
     * @brief Check if the player is in a dark area, such as an interior or shadowy region.
     *
     * @param player The player actor.
     * @return true If the player is in a dark area.
     * @return false If the player is not in a dark area.
     */
    bool IsPlayerInDarkArea(RE::Actor *player);
    bool IsPlayerNearLightSource(RE::Actor *player, float radius);
    /**
     * @brief Check if the current weather conditions are valid for detection.
     *
     * @param weather The current weather object.
     * @return true If the weather is valid for detection.
     * @return false If the weather reduces detection probability.
     */
    bool IsBadWeather(RE::TESWeather *currentWeather);
    /**
     * @brief Check if it is currently night time in the game.
     *
     * @return true If it is night time.
     * @return false If it is not night time.
     */
    bool IsNightTime();
    /**
     * @brief Check if the player is within the NPC's line of sight.
     *
     * @param npc The NPC whose line of sight is being checked.
     * @param player The player actor.
     * @return true If the player is in the NPC's line of sight.
     * @return false If the player is outside the NPC's line of sight.
     */
    bool IsInLineOfSight(RE::Actor *npc, RE::Actor *player);
    /**
     * @brief Check if the player is within the NPC's field of view.
     *
     * @param npc The NPC whose field of view is being checked.
     * @param player The player actor.
     * @param fieldOfViewDegrees The angle of the NPC's field of view.
     * @return true If the player is within the NPC's field of view.
     * @return false If the player is outside the field of view.
     */
    bool IsInFieldOfView(RE::Actor *npc, RE::Actor *player, float fieldOfViewDegrees = MAX_DETECTION_ANGLE);
    float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance);
    void CheckHoursPassed(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction);
    float GetEnvironmentalDetectionModifier(RE::Actor *player);

private:
    EnvironmentManager() = default;
};
