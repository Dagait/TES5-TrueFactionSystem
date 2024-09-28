#include <chrono>
#include <unordered_map>

struct NPCDetectionData {
    int detectionCount;
    std::chrono::steady_clock::time_point lastDetectedTime;
};
