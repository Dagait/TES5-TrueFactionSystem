#pragma once
#include <vector>

#include "RE/B/BGSBipedObjectForm.h"

/**
 * @brief Constant values for the weight of different armor pieces.
 *
 * These values represent how much each type of armor contributes to the disguise.
 */
constexpr float CHEST_WEIGHT = 30.0f;    /// Weight for chest armor.
constexpr float HELMET_WEIGHT = 12.0f;   /// Weight for helmet.
constexpr float GLOVES_WEIGHT = 4.0f;    /// Weight for gloves.
constexpr float FOREARMS_WEIGHT = 8.0f;  /// Weight for forearm armor.
constexpr float SHOES_WEIGHT = 5.0f;     /// Weight for shoes/boots.
constexpr float CIRCLET_WEIGHT = 1.0f;   /// Weight for circlets.

/**
 * @brief Struct to represent an armor slot and its corresponding weight.
 *
 * This structure holds the armor slot type and the weight contribution to disguise.
 */
struct ArmorSlot {
    RE::BGSBipedObjectForm::BipedObjectSlot slot;  /// The specific armor slot.
    float weight;                                  /// The disguise weight value for the slot.
};

/**
 * @brief Vector containing all relevant armor slots and their corresponding weights.
 *
 * This vector is used to iterate over armor slots and apply the respective disguise weights.
 */
extern const std::vector<ArmorSlot> armorSlots;

/**
 * @brief Vector containing all armor slot types.
 *
 * This vector includes all armor slots that can be used to determine what armor pieces are worn by the player or NPCs.
 */
extern const std::vector<RE::BGSBipedObjectForm::BipedObjectSlot> allArmorSlots;
