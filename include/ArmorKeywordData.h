#pragma once
#include "ArmorSlots.h"

using namespace RE;


struct ArmorKeywordData {
    RE::FormID armorID;
    RE::FormID keywordID;
};

extern std::vector<ArmorKeywordData> savedArmorKeywordAssociations;

/**
* 
*/
void SaveArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc);

/**
 *
 */
void LoadArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc);

/**
 * For new game?
 */
void RevertArmorKeywordDataCallback(SKSE::SerializationInterface* a_intfc);
