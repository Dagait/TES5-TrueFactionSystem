#include "ArmorKeywordData.h"


void SaveArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc) {
    a_intfc->OpenRecord('AKWD', 1);
    std::uint32_t numAssociations = savedArmorKeywordAssociations.size();
    a_intfc->WriteRecordData(&numAssociations, sizeof(numAssociations));

    for (const auto& association : savedArmorKeywordAssociations) {
        a_intfc->WriteRecordData(&association, sizeof(association));
    }
}

void LoadArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc) {
    std::uint32_t type;
    std::uint32_t version;
    std::uint32_t length;

    while (a_intfc->GetNextRecordInfo(type, version, length)) {
        if (type == 'AKWD') {
            std::uint32_t numAssociations;
            a_intfc->ReadRecordData(&numAssociations, sizeof(numAssociations));

            savedArmorKeywordAssociations.clear();
            for (std::uint32_t i = 0; i < numAssociations; ++i) {
                ArmorKeywordData data;
                a_intfc->ReadRecordData(&data, sizeof(data));
                savedArmorKeywordAssociations.push_back(data);
            }

            // Re-apply the keywords to the armors
            for (const auto& association : savedArmorKeywordAssociations) {
                RE::TESObjectARMO* armor = RE::TESForm::LookupByID<RE::TESObjectARMO>(association.armorID);
                RE::BGSKeyword* keyword = RE::TESForm::LookupByID<RE::BGSKeyword>(association.keywordID);

                if (armor && keyword) {
                    AddKeywordToArmor(armor, keyword);
                }
            }
        }
    }
}


void RevertArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc) {
    savedArmorKeywordAssociations.clear();
}
