#include "Managers/EquipmentManager.h"
#include "Globals.h"


namespace NPE {
    RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent *evn,
                                                             RE::BSTEventSource<RE::TESEquipEvent> *dispatcher) {
        if (!evn || !evn->actor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        RE::Actor *actor = skyrim_cast<RE::Actor *>(evn->actor.get());

        if (actor && actor->IsPlayerRef()) {
            NPE::UpdateDisguiseValue(actor);  // Update disguise value for player
        }

        return RE::BSEventNotifyControl::kContinue;
    }
}