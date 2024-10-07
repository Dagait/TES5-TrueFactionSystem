#pragma once

#include "Race.h"
#include "Disguise.h"
#include "Faction.h"
#include "Combat.h"
#include "ArmorSlots.h"
#include "ArmorKeywordData.h"
#include "Papyrus.h"

#include "RE/B/BSFixedString.h"

#include "SKSE/Trampoline.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <thread>
#include <chrono>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

extern RE::TESDataHandler* g_dataHandler;
extern std::vector<RE::TESFaction*> g_allFactions;

// Class for handling equip events
class EquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
public:
    // Override method for processing equip events
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* evn,
                                                  RE::BSTEventSource<RE::TESEquipEvent>* dispatcher) override;
};

// Function to start a background task for periodically updating disguise values and NPC detection
void StartBackgroundTask(RE::Actor* player);

void SaveCallback(SKSE::SerializationInterface* a_intfc);

void LoadCallback(SKSE::SerializationInterface* a_intfc);