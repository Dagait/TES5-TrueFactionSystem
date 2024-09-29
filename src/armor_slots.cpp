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
