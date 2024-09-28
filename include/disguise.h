#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

float CalculateDisguiseValue(Actor *actor, bool isEquipped);
void UpdateDisguiseValue(Actor *actor, bool isEquipped);
void CheckNPCDetection(Actor *actor);
float GetDetectionProbability(float disguiseValue);
float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance);
float GetDetectionProbability(float disguiseValue);
bool NPCRecognizesPlayer(RE::Actor *npc, RE::Actor *player);
void LoadDetectionData(SKSE::SerializationInterface *a_intfc);
void SaveDetectionData(SKSE::SerializationInterface *a_intfc);