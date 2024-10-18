#pragma once
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


namespace NPE {
    constexpr float TIME_TO_LOSE_DETECTION = 2.0f;

    constexpr float DETECTION_RADIUS = 400.0f;

    constexpr float MAX_DETECTION_ANGLE = 90.0f;
    constexpr float NPC_LEVEL_THRESHOLD = 20.0f;
    constexpr float ADD_TO_FACTION_THRESHOLD = 15.0f;


    constexpr std::chrono::seconds CHECK_INTERVAL_SECONDS(2);

    constexpr std::chrono::seconds UPDATE_DISGUISE_INTERVAL_SECONDS(2);
    constexpr std::chrono::seconds DETECTION_INTERVAL_SECONDS(18);
    constexpr std::chrono::seconds RACE_CHECK_INTERVAL_SECONDS(5);
}