#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "Main.h"
#include "Disguise.h"
#include "Misc/Utils.h"


namespace NPE {
    bool IsPlayerInCorrectRace(RE::BSFixedString keyword);

    void InitRaceDisguiseBonus();

    void LoadJsonData();

    int RaceValueForFaction(const std::string &race, const std::string &faction);
}