#pragma once
#include "ArmorSlots.h"


namespace NPE {
    struct ArmorKeywordData {
        RE::FormID armorID;
        RE::FormID keywordID;
    };

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
}
