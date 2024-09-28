#pragma once
#include <chrono>

struct NPCDetectionData {
    int detectionCount;
    std::chrono::steady_clock::time_point lastDetectedTime;
};