#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "Main.h"
#include "DisguiseData.h"
#include "Disguise.h"
#include "Misc/Utils.h"

extern RE::TESDataHandler* g_dataHandler;

extern std::unordered_map<std::string, std::unordered_map<std::string, int>> factionRaceData;
extern std::unordered_map<std::string, std::unordered_map<std::string, int>> raceValueData;

bool IsPlayerInCorrectRace(RE::BSFixedString keyword);

void InitRaceDisguiseBonus();

void LoadJsonData();

int RaceValueForFaction(const std::string &race, const std::string &faction);