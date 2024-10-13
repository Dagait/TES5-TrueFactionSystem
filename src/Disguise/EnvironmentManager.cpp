#include "Disguise/EnvironmentManager.h"


bool EnvironmentManager::IsPlayerInDarkArea(RE::Actor *player) {
    RE::TESObjectCELL *cell = player->GetParentCell();
    if (!cell) {
        return false;
    }

    if (cell->IsInteriorCell()) {
        RE::INTERIOR_DATA *lightingData = cell->GetLighting();
        float lightingLevel = lightingData->ambient.red + lightingData->ambient.green + lightingData->ambient.blue;

        return lightingLevel < 150.0f;
    } else {
        // If exterior, check the current weather and light levels from the Sky singleton
        RE::Sky *sky = RE::Sky::GetSingleton();
        if (!sky) {
            return false;
        }

        RE::TESWeather *currentWeather = sky->currentWeather;
        if (IsBadWeather(currentWeather)) {
            return true;
        }

        if (!IsPlayerNearLightSource(player)) {
            return true;
        }

        float currentTime = sky->currentGameHour;
        if (IsNightTime()) {
            return true;
        }
    }

    return false;
}


bool EnvironmentManager::IsPlayerNearLightSource(RE::Actor *player, float radius) {
    if (!player) {
        return false;
    }

    RE::TESObjectCELL *cell = player->GetParentCell();
    if (!cell) {
        return false;
    }

    RE::NiPoint3 playerPos = player->GetPosition();

    cell->ForEachReferenceInRange(playerPos, radius, [&](RE::TESObjectREFR &ref) -> RE::BSContainer::ForEachResult {
        RE::TESObjectLIGH *lightSource = ref.As<RE::TESObjectLIGH>();
        if (lightSource && lightSource->data.radius > 0) {
            return RE::BSContainer::ForEachResult::kStop;
        }
        return RE::BSContainer::ForEachResult::kContinue;
    });

    return false;
}

bool EnvironmentManager::IsInLineOfSight(RE::Actor *npc, RE::Actor *player) {
    if (!npc || !player) {
        return false;
    }

    bool hasLineOfSight = false;
    return npc->HasLineOfSight(player->AsReference(), hasLineOfSight);
}

bool EnvironmentManager::IsNightTime() {
    RE::Sky *sky = RE::Sky::GetSingleton();
    if (!sky) {
        return false;
    }

    float currentTime = sky->currentGameHour;
    return currentTime < 6.0f || currentTime > 18.0f;
}

bool EnvironmentManager::IsInFieldOfView(RE::Actor *npc, RE::Actor *player, float fieldOfViewDegrees) {
    if (!npc || !player) {
        return false;
    }

    float distance = npc->GetPosition().GetDistance(player->GetPosition());
    if (distance > DETECTION_RADIUS) {
        return false;
    }

    float npcRotationZ = npc->data.angle.z;
    float npcRotationZInRadians = npcRotationZ;

    RE::NiPoint3 npcForward(std::cos(npcRotationZInRadians), std::sin(npcRotationZInRadians), 0.0f);

    RE::NiPoint3 npcToPlayer = player->GetPosition() - npc->GetPosition();
    float length = npcToPlayer.Length();
    if (length > 0.0f) {
        npcToPlayer /= length;
    }

    float dotProduct = npcForward.Dot(npcToPlayer);
    dotProduct = std::clamp(dotProduct, -1.0f, 1.0f);
    float fovFactor = std::cos((fieldOfViewDegrees / 2.0f) * (M_PI / 180.0f));

    if (dotProduct >= fovFactor) {
        return true;
    } else {
        // Smooth falloff for detection outside the FOV
        float falloffFactor = std::max(0.0f, (dotProduct + 1.0f) / (1.0f - fovFactor));
        return falloffFactor > 0.0f;
    }
}

bool EnvironmentManager::IsBadWeather(RE::TESWeather *currentWeather) {
    // Check if the weather is rainy, foggy, or cloudy (bad for visibility)
    if (currentWeather) {
        if (currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kRainy ||
            currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kCloudy ||
            currentWeather->data.flags & RE::TESWeather::WeatherDataFlag::kSnow) {
            return true;
        }
    }
    return false;
}

float EnvironmentManager::AdjustProbabilityByDistance(float detectionProbability, float distance, float maxDistance) {
    float distanceFactor = 1.0f - (distance / maxDistance);  // Scaled by 1 (near) to 0 (far)
    return detectionProbability * distanceFactor;
}

void EnvironmentManager::CheckHoursPassed(RE::Actor *npc, RE::Actor *player, RE::TESFaction *faction) {
    RE::FormID npcID = npc->GetFormID();
    float currentInGameHours = RE::Calendar::GetSingleton()->GetHoursPassed();

    if (recognizedNPCs.find(npcID) != recognizedNPCs.end()) {
        NPCDetectionData &detectionData = recognizedNPCs[npcID];

        float timeSinceLastDetected = currentInGameHours - detectionData.lastDetectedTime;

        if (timeSinceLastDetected > TIME_TO_LOSE_DETECTION) {
            recognizedNPCs.erase(npcID);
            if (playerDisguiseStatus.GetDisguiseValue(faction) > 5.0f) {
                player->AddToFaction(faction, 1);
            }
        }
    }
}

float EnvironmentManager::GetEnvironmentalDetectionModifier(RE::Actor *player) {
    if (this->IsPlayerInDarkArea(player)) {
        return 0.5f;  // Reduce detection in dark areas
    }
    if (this->IsPlayerNearLightSource(player, 5.0f)) {
        return 1.5f;  // Increase detection near light sources
    }
    return 1.0f;
}
