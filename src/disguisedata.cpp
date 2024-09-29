#include "disguisedata.h"

void PlayerDisguiseStatus::SetDisguiseValue(RE::TESFaction* faction, float value) {
    if (factionDisguiseMap.find(faction) != factionDisguiseMap.end()) {
        factionDisguiseMap[faction].disguiseValue = value;
    } else {
        factionDisguiseMap[faction] = {faction, value};
    }
}

float PlayerDisguiseStatus::GetDisguiseValue(RE::TESFaction* faction) const {
    auto it = factionDisguiseMap.find(faction);
    if (it != factionDisguiseMap.end()) {
        return it->second.disguiseValue;
    }
    return 0.0f;
}

void PlayerDisguiseStatus::RemoveDisguiseValue(RE::TESFaction* faction) { factionDisguiseMap.erase(faction); }

void PlayerDisguiseStatus::Clear() { factionDisguiseMap.clear(); }

void PlayerDisguiseStatus::Save(SKSE::SerializationInterface* a_intfc) {
    std::uint32_t size = factionDisguiseMap.size();
    a_intfc->WriteRecord('PDST', 1, &size, sizeof(size));

    for (const auto& [faction, data] : factionDisguiseMap) {
        RE::FormID factionID = faction->GetFormID();
        a_intfc->WriteRecordData(&factionID, sizeof(factionID));
        a_intfc->WriteRecordData(&data.disguiseValue, sizeof(data.disguiseValue));
    }
}

void PlayerDisguiseStatus::Load(SKSE::SerializationInterface* a_intfc) {
    std::uint32_t size;
    if (!a_intfc->ReadRecordData(&size, sizeof(size))) {
        return;
    }

    factionDisguiseMap.clear();
    for (std::uint32_t i = 0; i < size; ++i) {
        RE::FormID factionID;
        float disguiseValue;

        if (!a_intfc->ReadRecordData(&factionID, sizeof(factionID))) {
            return;
        }
        if (!a_intfc->ReadRecordData(&disguiseValue, sizeof(disguiseValue))) {
            return;
        }

        RE::TESFaction* faction = RE::TESForm::LookupByID<RE::TESFaction>(factionID);
        if (faction) {
            factionDisguiseMap[faction] = {faction, disguiseValue};
        }
    }
}
