#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

class EquipEventHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
public:
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent *evn,
                                                  RE::BSTEventSource<RE::TESEquipEvent> *dispatcher) override;
};
