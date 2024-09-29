#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "RE/C/Calendar.h"

using namespace RE;

class HitEventHandler : public RE::BSTEventSink<RE::TESHitEvent> {
public:
    virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* evn,
                                                  RE::BSTEventSource<RE::TESHitEvent>* dispatcher) override;
};
RE::TESFaction* GetFactionByActor(RE::Actor* actor);
void CheckAndReAddPlayerToFaction(RE::Actor* player);
void StartCombat(RE::Actor* npc, RE::Actor* player, RE::TESFaction* npcFaction);