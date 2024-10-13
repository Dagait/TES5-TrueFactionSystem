#include "ArmorSlots.h"


const std::vector<ArmorSlot> armorSlotsSlot = {{RE::BGSBipedObjectForm::BipedObjectSlot::kBody, CHEST_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kHands, GLOVES_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kForearms, FOREARMS_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet, CIRCLET_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kFeet, SHOES_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kHead, HELMET_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kHair, HAIR_WEIGHT}
};

bool AddKeywordToArmor(RE::TESObjectARMO* armor, RE::BGSKeyword* keyword) {
    if (!armor || !keyword) {
        return false;
    }

    RE::BGSKeywordForm* keywordForm = armor->As<RE::BGSKeywordForm>();
    if (!keywordForm) {
        return false;
    }

    // Check if the keyword is already in the list
    for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
        if (keywordForm->keywords[i] == keyword) {
            return false;  // Keyword already exists, don't add it again
        }
    }

    // Expand the keyword array and add the new keyword
    uint32_t newNumKeywords = keywordForm->numKeywords + 1;
    RE::BGSKeyword** newKeywords = new RE::BGSKeyword*[newNumKeywords];

    // Copy over existing keywords
    for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
        newKeywords[i] = keywordForm->keywords[i];
    }

    newKeywords[newNumKeywords - 1] = keyword;

    keywordForm->keywords = newKeywords;
    keywordForm->numKeywords = newNumKeywords;

    return true;
}

bool RemoveKeywordFromArmor(RE::TESObjectARMO* armor, RE::BGSKeyword* keyword) {
    if (!armor || !keyword) {
        return false;
    }

    RE::BGSKeywordForm* keywordForm = armor->As<RE::BGSKeywordForm>();
    if (!keywordForm || keywordForm->numKeywords == 0) {
        return false;
    }

    // Find the index of the keyword to be removed
    int32_t indexToRemove = -1;
    for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
        if (keywordForm->keywords[i] == keyword) {
            indexToRemove = i;
            break;
        }
    }

    // If keyword not found, return false
    if (indexToRemove == -1) {
        return false;
    }

    // If there's only one keyword, just clear the array
    if (keywordForm->numKeywords == 1) {
        keywordForm->keywords = nullptr;
        keywordForm->numKeywords = 0;
        return true;
    }

    uint32_t newNumKeywords = keywordForm->numKeywords - 1;
    RE::BGSKeyword** newKeywords = new RE::BGSKeyword*[newNumKeywords];

    // Copy over the keywords except for the one to remove
    for (uint32_t i = 0, j = 0; i < keywordForm->numKeywords; i++) {
        if (i != indexToRemove) {
            newKeywords[j++] = keywordForm->keywords[i];
        }
    }

    keywordForm->keywords = newKeywords;
    keywordForm->numKeywords = newNumKeywords;

    return true;
}


RE::BGSKeyword* GetKeywordByEditorID(RE::BSFixedString keywordEditorID) {
    if (!g_dataHandler) {
        return nullptr;
    }
    const auto& allKeywords = g_dataHandler->GetFormArray<RE::BGSKeyword>();

    for (RE::BGSKeyword *keyword : allKeywords) {
        if (keyword && strcmp(keywordEditorID.c_str(), keyword->GetFormEditorID()) == 0) {
            return keyword;
        }
    }

    return nullptr;
}