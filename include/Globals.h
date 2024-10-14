#pragma once
#include "Managers/NPCAlertnessManager.h"
#include "Managers/DetectionManager.h"
#include "Managers/DisguiseManager.h"
#include "Managers/EnvironmentManager.h"
#include "Managers/EquipmentManager.h"

#include "DisguiseData.h"
#include "NpcDetectionData.h"
#include "ArmorKeywordData.h"


namespace NPE {
    class DetectionManager;
    class DisguiseManager;
    class EnvironmentManager;
    class NPCAlertnessManager;
    class EquipEventHandler;

    extern DetectionManager& detectionManager;
    extern DisguiseManager& disguiseManager;
    extern EnvironmentManager& environmentManager;
    extern NPCAlertnessManager& npcAlertnessManager;
    extern EquipEventHandler& equipEventHandler;

    extern std::vector<std::pair<std::string, RE::FormID>> factionArmorKeywords;
    extern const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> allArmorSlots;
    extern std::unordered_map<RE::FormID, std::string> factionFormIDToTagMap;

    extern std::unordered_map<RE::FormID, NPCDetectionData> recognizedNPCs;
    extern PlayerDisguiseStatus playerDisguiseStatus;
    extern std::vector<ArmorKeywordData> savedArmorKeywordAssociations;
    extern RE::TESDataHandler* g_dataHandler;

    extern std::unordered_map<std::string, std::unordered_map<std::string, int>> factionRaceData;

    extern const std::vector<ArmorSlot> armorSlotsSlot;
}