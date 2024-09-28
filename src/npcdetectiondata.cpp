#include <chrono>
#include <unordered_map>

// Strukturdaten für die Erkennung
struct NPCDetectionData {
    int detectionCount;
    std::chrono::steady_clock::time_point lastDetectedTime;
};
