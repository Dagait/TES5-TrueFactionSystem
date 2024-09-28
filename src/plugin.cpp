#include "plugin.h"
#include "disguise.h"
#include "faction.h"

using namespace RE;
using namespace SKSE;

static EquipEventHandler g_equipEventHandler;

RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent *evn,
                                                         RE::BSTEventSource<RE::TESEquipEvent> *dispatcher) {
    if (!evn || !evn->actor) {
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::ConsoleLog::GetSingleton()->Print("Equipment change detected!");

    Actor *actor = skyrim_cast<Actor *>(evn->actor.get());

    if (actor && actor->IsPlayerRef()) {
        RE::ConsoleLog::GetSingleton()->Print("Equipment change detected!");
        UpdateDisguiseValue(actor);  // Update disguise value for player
    }

    return RE::BSEventNotifyControl::kContinue;
}

SKSEPluginLoad(const LoadInterface *skse) {
    SKSE::Init(skse);

    GetMessagingInterface()->RegisterListener([](MessagingInterface::Message *message) {
        if (message->type == MessagingInterface::kDataLoaded) {
            RE::ConsoleLog::GetSingleton()->Print("Loading in TrueFactionSystem...");

            auto equipEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (equipEventSource) {
                equipEventSource->AddEventSink(&g_equipEventHandler);
                RE::ConsoleLog::GetSingleton()->Print("TrueFactionSystem hook successful!");
            }

            Actor *player = PlayerCharacter::GetSingleton();
            if (player) {
                UpdateDisguiseValue(player);
            }
        }
    });

    return true;
}
