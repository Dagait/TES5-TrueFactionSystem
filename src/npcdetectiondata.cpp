#include <chrono>
#include <unordered_map>

// Strukturdaten f�r die Erkennung
struct NPCDetectionData {
    int detectionCount;
    std::chrono::steady_clock::time_point lastDetectedTime;
};
