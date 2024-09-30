#pragma once

#include "disguise.h"
#include "faction.h"
#include "combat.h"
#include "armor_slots.h"
#include "armor_keyword_data.h"

#include "RE/B/BSFixedString.h"

#include "SKSE/Trampoline.h"
#include <thread>
#include <chrono>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

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