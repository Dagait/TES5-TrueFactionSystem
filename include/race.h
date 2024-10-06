#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "main.h"
#include "disguise_data.h"

extern RE::TESDataHandler* g_dataHandler;
extern PlayerDisguiseStatus playerDisguiseStatus;

bool IsPlayerInCorrectRace(RE::BSFixedString keyword);

void InitRaceDisguiseBonus();