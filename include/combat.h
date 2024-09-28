#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

class HitEventHandler : public RE::BSTEventSink<RE::TESHitEvent> {
public:
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* evn,
                                                  RE::BSTEventSource<RE::TESHitEvent>* dispatcher) override;
};
RE::TESFaction* GetFactionByActor(RE::Actor* actor);