#include "plugin.h"
#include "disguise.h"
#include "faction.h"
#include "combat.h"

#include "SKSE/Trampoline.h"
#include <thread>
#include <chrono>

using namespace SKSE;
using namespace RE;

std::chrono::steady_clock::time_point lastCheckTime;

constexpr std::chrono::seconds CHECK_INTERVAL_SECONDS(5);

static EquipEventHandler g_equipEventHandler;
static HitEventHandler g_hitEventHandler;

RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent *evn,
                                                         RE::BSTEventSource<RE::TESEquipEvent> *dispatcher) {
    if (!evn || !evn->actor) {
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::ConsoleLog::GetSingleton()->Print("Equipment change detected!");

    Actor *actor = skyrim_cast<Actor *>(evn->actor.get());
    bool isEquipped = evn->equipped;

    if (actor && actor->IsPlayerRef()) {
        RE::ConsoleLog::GetSingleton()->Print("Equipment change detected!");
        UpdateDisguiseValue(actor, isEquipped);  // Update disguise value for player
    }

    return RE::BSEventNotifyControl::kContinue;
}

void StartBackgroundTask(Actor *player) {
    std::thread([player]() {
        while (true) {
            if (player && player->IsPlayerRef()) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = now - lastCheckTime;

                if (elapsed >= CHECK_INTERVAL_SECONDS) {
                    RE::ConsoleLog::GetSingleton()->Print("Checking NPC detection...");
                    CheckNPCDetection(player);
                    CheckAndReAddPlayerToFaction(player);
                    lastCheckTime = now;
                }
            }
            std::this_thread::sleep_for(CHECK_INTERVAL_SECONDS);
        }
    }).detach();
}


SKSEPluginLoad(const LoadInterface *skse) {
    SKSE::Init(skse);

    SKSE::GetMessagingInterface()->RegisterListener([](MessagingInterface::Message *message) {
        if (message->type == MessagingInterface::kDataLoaded) {
            RE::ConsoleLog::GetSingleton()->Print("Loading in TrueFactionSystem...");

            auto equipEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (equipEventSource) {
                equipEventSource->AddEventSink(&g_equipEventHandler);
                RE::ConsoleLog::GetSingleton()->Print("EquipEventHandler registered!");
            }

            auto hitEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (hitEventSource) {
                hitEventSource->AddEventSink(&g_hitEventHandler);
                RE::ConsoleLog::GetSingleton()->Print("HitEventHandler registered!");
            }

            Actor *player = PlayerCharacter::GetSingleton();
            if (player) {
                lastCheckTime = std::chrono::steady_clock::now();
                StartBackgroundTask(player);
            }
        }
    });

    SKSE::GetSerializationInterface()->SetSaveCallback(
        [](SKSE::SerializationInterface *a_intfc) { SaveDetectionData(a_intfc); });

    SKSE::GetSerializationInterface()->SetLoadCallback(
        [](SKSE::SerializationInterface *a_intfc) { LoadDetectionData(a_intfc); });

    return true;
}
