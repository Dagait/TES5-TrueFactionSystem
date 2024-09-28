#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

using namespace RE;

float CalculateDisguiseValue(Actor *actor);
void UpdateDisguiseValue(Actor *actor);
bool CheckNPCDetection(Actor *actor, float disguiseValue);
float GetDetectionProbability(float disguiseValue);
bool IsPlayerDetected(float disguiseValue);
float AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance);