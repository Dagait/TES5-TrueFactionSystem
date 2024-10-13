#include "Disguise/NPCAlertnessManager.h"


void NPCAlertnessManager::UpdateNPCAlertness(RE::Actor *npc, bool recognizesPlayer) {
    NPEActor npeActor(npc);
    NPCAlertness currentAlertness = npeActor.GetAlertness();

    if (recognizesPlayer) {
        if (currentAlertness == NPCAlertness::RELAXED) {
            npeActor.SetAlertness(NPCAlertness::SUSPICIOUS);
        } else if (currentAlertness == NPCAlertness::SUSPICIOUS) {
            npeActor.SetAlertness(NPCAlertness::ALERTED);
        }
    } else {
        if (currentAlertness == NPCAlertness::ALERTED) {
            npeActor.SetAlertness(NPCAlertness::SUSPICIOUS);
        } else if (currentAlertness == NPCAlertness::SUSPICIOUS) {
            npeActor.SetAlertness(NPCAlertness::RELAXED);
        }
    }
}

float NPCAlertnessManager::GetNPCDetectionModifier(RE::Actor *npc) {
    NPEActor npeActor(npc);
    NPCAlertness alertness = npeActor.GetAlertness();
    switch (alertness) {
        case NPCAlertness::RELAXED:
            return 0.5f;
        case NPCAlertness::SUSPICIOUS:
            return 1.0f;
        case NPCAlertness::ALERTED:
            return 2.0f;
        default:
            return 1.0f;
    }
}

void NPCAlertnessManager::AdjustDetectionProbabilityBasedOnAlertness(NPCAlertness alertness,
                                                                     float &detectionProbability) {
    // detectionProbability *= GetNPCDetectionModifier(alertness);
}