#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "Main.h"
#include "DisguiseData.h"

extern RE::TESDataHandler* g_dataHandler;
extern PlayerDisguiseStatus playerDisguiseStatus;

extern std::unordered_map<std::string, std::unordered_map<std::string, int>> factionRaceData;
extern std::unordered_map<std::string, std::unordered_map<std::string, int>> raceValueData;

bool IsPlayerInCorrectRace(RE::BSFixedString keyword);

void InitRaceDisguiseBonus();

void LoadJsonData();

int GetDisguiseValueForFaction(const std::string &race, const std::string &faction);