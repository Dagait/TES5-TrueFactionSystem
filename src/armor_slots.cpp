#include "armor_slots.h"

// Define the armor slots with corresponding weights
const std::vector<ArmorSlot> armorSlots = {{RE::BGSBipedObjectForm::BipedObjectSlot::kBody, CHEST_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kHands, GLOVES_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kForearms, FOREARMS_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet, CIRCLET_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kFeet, SHOES_WEIGHT},
                                           {RE::BGSBipedObjectForm::BipedObjectSlot::kHead, HELMET_WEIGHT}};

// Define all possible armor slots for checking
const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> allArmorSlots = {
    RE::BGSBipedObjectForm::BipedObjectSlot::kHead,     RE::BGSBipedObjectForm::BipedObjectSlot::kBody,
    RE::BGSBipedObjectForm::BipedObjectSlot::kHands,    RE::BGSBipedObjectForm::BipedObjectSlot::kFeet,
    RE::BGSBipedObjectForm::BipedObjectSlot::kForearms, RE::BGSBipedObjectForm::BipedObjectSlot::kCirclet,
    RE::BGSBipedObjectForm::BipedObjectSlot::kHair};

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

    // Add the new keyword
    newKeywords[newNumKeywords - 1] = keyword;

    // Replace the old keyword array
    keywordForm->keywords = newKeywords;
    keywordForm->numKeywords = newNumKeywords;

    return true;
}

RE::BGSKeyword* GetKeywordByEditorID(RE::BSFixedString keywordName) {
    auto dataHandler = RE::TESDataHandler::GetSingleton();
    if (!dataHandler) {
        return nullptr;
    }
    const auto& allKeywords = dataHandler->GetFormArray<RE::BGSKeyword>();

    for (RE::BGSKeyword *currentKeyword : allKeywords) {
        if (currentKeyword) {
            if (strcmp(keywordName.c_str(), currentKeyword->GetFormEditorID()) == 0) {
                return currentKeyword;
            }
        }
    }

    return nullptr;
}