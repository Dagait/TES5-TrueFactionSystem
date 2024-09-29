#pragma once

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
