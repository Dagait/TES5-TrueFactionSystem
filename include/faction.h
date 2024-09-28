#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

RE::TESFaction *GetFactionByArmorTag(RE::Actor *actor);
std::vector<std::pair<std::string, RE::TESFaction *>> GetRelevantFactions();
