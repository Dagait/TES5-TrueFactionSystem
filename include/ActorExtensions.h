#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"


class NPEActor {
public:
    explicit NPEActor(RE::Actor* a_actor);

    void StartCombatWith(RE::Actor* target);
    void SetHostile(RE::Actor* target);

private:
    RE::Actor* actor;
};