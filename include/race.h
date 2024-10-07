#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "Main.h"
#include "DisguiseData.h"

extern RE::TESDataHandler* g_dataHandler;
extern PlayerDisguiseStatus playerDisguiseStatus;

bool IsPlayerInCorrectRace(RE::BSFixedString keyword);

void InitRaceDisguiseBonus();