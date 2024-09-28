#pragma once
#include <unordered_map>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

struct DisguiseData {
    RE::TESFaction* faction;
    float disguiseValue;
};

struct PlayerDisguiseStatus {
    std::unordered_map<RE::TESFaction*, DisguiseData> factionDisguiseMap;
    void SetDisguiseValue(RE::TESFaction* faction, float value);
    float GetDisguiseValue(RE::TESFaction* faction) const;
    void RemoveDisguiseValue(RE::TESFaction* faction);

    void Save(SKSE::SerializationInterface* a_intfc);
    void Load(SKSE::SerializationInterface* a_intfc);
    void Clear();
};
