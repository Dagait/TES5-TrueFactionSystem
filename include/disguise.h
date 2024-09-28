#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

void CalculateDisguiseValue(Actor *actor, RE::TESFaction *faction);
void UpdateDisguiseValue(Actor *actor);
void CheckNPCDetection(Actor *actor);
float GetDetectionProbability(float disguiseValue);
float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance);
float GetDetectionProbability(float disguiseValue);
bool NPCRecognizesPlayer(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction);
void LoadDetectionData(SKSE::SerializationInterface *a_intfc);
void SaveDetectionData(SKSE::SerializationInterface *a_intfc);
bool IsFaceCovered(Actor *actor);