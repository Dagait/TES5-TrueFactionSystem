#include "main.h"


using namespace SKSE;
using namespace RE;

std::chrono::steady_clock::time_point lastCheckTime;
std::vector<ArmorKeywordData> savedArmorKeywordAssociations;

constexpr std::chrono::seconds CHECK_INTERVAL_SECONDS(2);

static EquipEventHandler g_equipEventHandler;
static HitEventHandler g_hitEventHandler;

RE::BSEventNotifyControl EquipEventHandler::ProcessEvent(const RE::TESEquipEvent *evn,
                                                         RE::BSTEventSource<RE::TESEquipEvent> *dispatcher) {
    if (!evn || !evn->actor) {
        return RE::BSEventNotifyControl::kContinue;
    }

    Actor *actor = skyrim_cast<Actor*>(evn->actor.get());

    if (actor && actor->IsPlayerRef()) {
        UpdateDisguiseValue(actor);  // Update disguise value for player
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
                    UpdateDisguiseValue(player);
                    CheckNPCDetection(player);   
                    CheckAndReAddPlayerToFaction(player);
                    lastCheckTime = now;
                }
            }
            std::this_thread::sleep_for(CHECK_INTERVAL_SECONDS);
        }
    }).detach();
}

bool PapyrusAddKeywordToArmor(RE::StaticFunctionTag *, RE::TESObjectARMO *armor, RE::BGSKeyword *keyword) {
    return AddKeywordToArmor(armor, keyword);
}

bool PapyrusRemoveKeywordFromArmor(RE::StaticFunctionTag *, RE::TESObjectARMO *armor, RE::BGSKeyword *keyword) {
    return RemoveKeywordFromArmor(armor, keyword);
}

RE::BGSKeyword *PapyrusGetKeywordByEditorID(RE::StaticFunctionTag *, RE::BSFixedString keyword) {
    return GetKeywordByEditorID(keyword);
}

std::vector<RE::TESFaction *> PapyrusGetFactionsForActor(RE::StaticFunctionTag *, RE::Actor *actor) {
    return GetFactionsForActor(actor);
}

float PapyrusGetDisguiseValueForFaction(RE::StaticFunctionTag *, RE::TESFaction *faction) {
    return GetDisguiseValueForFaction(faction);
}

RE::BSFixedString PapyrusGetFactionEditorID(RE::StaticFunctionTag *, RE::TESFaction *faction) {
    return GetFactionEditorID(faction);
}

float PapyrusGetDisguiseBonusValueForFaction(RE::StaticFunctionTag *, RE::TESFaction *faction) {
    return GetDisguiseBonusValueForFaction(faction);
}


// Function to bind the Papyrus function
bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine *vm) {
    vm->RegisterFunction("AddKeywordToArmor", "npeTFS_NativeFunctions", PapyrusAddKeywordToArmor);
    vm->RegisterFunction("RemoveKeywordFromArmor", "npeTFS_NativeFunctions", PapyrusRemoveKeywordFromArmor);
    vm->RegisterFunction("GetKeywordByEditorID", "npeTFS_NativeFunctions", PapyrusGetKeywordByEditorID);
    vm->RegisterFunction("GetFactionsForActor", "npeTFS_NativeFunctions", PapyrusGetFactionsForActor);
    vm->RegisterFunction("GetDisguiseValueForFaction", "npeTFS_NativeFunctions", PapyrusGetDisguiseValueForFaction);
    vm->RegisterFunction("GetDisguiseBonusValueForFaction", "npeTFS_NativeFunctions", PapyrusGetDisguiseBonusValueForFaction);
    vm->RegisterFunction("GetFactionEditorID", "npeTFS_NativeFunctions", PapyrusGetFactionEditorID);
    return true;
}

void SaveCallback(SKSE::SerializationInterface *a_intfc) {
    // SaveDetectionData(a_intfc);
    SaveArmorKeywordDataCallback(a_intfc);
}

void LoadCallback(SKSE::SerializationInterface *a_intfc) {
    // LoadDetectionData(a_intfc);
    LoadArmorKeywordDataCallback(a_intfc);
}




extern "C" [[maybe_unused]] __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    SKSE::GetPapyrusInterface()->Register(RegisterPapyrusFunctions);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            RE::ConsoleLog::GetSingleton()->Print("Loading in TFS...");

            auto equipEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (equipEventSource) {
                equipEventSource->AddEventSink(&g_equipEventHandler);
                RE::ConsoleLog::GetSingleton()->Print("TFS: EquipEventHandler registered!");
            }

            auto hitEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (hitEventSource) {
                hitEventSource->AddEventSink(&g_hitEventHandler);
                RE::ConsoleLog::GetSingleton()->Print("TFS: HitEventHandler registered!");
            }

            Actor *player = PlayerCharacter::GetSingleton();
            if (player) {
                lastCheckTime = std::chrono::steady_clock::now();
                StartBackgroundTask(player);
            }

            SKSE::GetSerializationInterface()->SetSaveCallback(SaveCallback);
            SKSE::GetSerializationInterface()->SetLoadCallback(LoadCallback);
        }
    });

    return true;
}
