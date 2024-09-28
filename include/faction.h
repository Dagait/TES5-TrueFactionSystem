#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

RE::TESFaction *GetFactionByArmorTag(RE::Actor *actor);
void ModifyFactionDetection(Actor *actor, float disguiseValue, RE::TESFaction *faction);
