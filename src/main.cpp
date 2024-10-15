#include "Main.h"
#include "Globals.h"


using namespace SKSE::stl;
using namespace SKSE;
using namespace RE;

std::chrono::steady_clock::time_point lastCheckTime;
std::chrono::steady_clock::time_point lastCheckDetectionTime;
std::chrono::steady_clock::time_point lastRaceCheckTime;

constexpr std::chrono::seconds CHECK_INTERVAL_SECONDS(2);
constexpr std::chrono::seconds DETECTION_INTERVAL_SECONDS(18);
constexpr std::chrono::seconds RACE_CHECK_INTERVAL_SECONDS(5);

static NPE::HitEventHandler g_hitEventHandler;

RE::TESDataHandler *g_dataHandler;
std::vector<RE::TESFaction *> g_allFactions;

void StartBackgroundTask(Actor *player) {
    std::thread([player]() {
        while (true) {
            if (player && player->IsPlayerRef()) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = now - lastCheckTime;
                auto elapsedDetection = now - lastCheckDetectionTime;
                auto elapsedRace = now - lastRaceCheckTime;

                if (elapsed >= CHECK_INTERVAL_SECONDS) {
                    NPE::disguiseManager.UpdateDisguiseValue(player);
                    NPE::CheckAndReAddPlayerToFaction(player);
                    lastCheckTime = now;
                }
                if (elapsedDetection >= DETECTION_INTERVAL_SECONDS) {
                    NPE::detectionManager.CheckNPCDetection(player);
                    lastCheckDetectionTime = now;
                }
                if (elapsedRace >= RACE_CHECK_INTERVAL_SECONDS) {
                    NPE::InitRaceDisguiseBonus();
                }
            }
            std::this_thread::sleep_for(CHECK_INTERVAL_SECONDS);
        }
    }).detach();
}


void SaveCallback(SKSE::SerializationInterface *a_intfc) {
    // SaveDetectionData(a_intfc);
    NPE::SaveArmorKeywordDataCallback(a_intfc);
}

void LoadCallback(SKSE::SerializationInterface *a_intfc) {
    // LoadDetectionData(a_intfc);
    NPE::LoadArmorKeywordDataCallback(a_intfc);
}

std::vector<RE::TESFaction *> ConvertBSTArrayToVector(const RE::BSTArray<RE::TESFaction *> &bstArray) {
    std::vector<RE::TESFaction *> vector;

    for (std::uint32_t i = 0; i < bstArray.size(); ++i) {
        vector.push_back(bstArray[i]);
    }

    return vector;
}

void InitializeGlobalData() {
    if (!g_dataHandler) {
        g_dataHandler = RE::TESDataHandler::GetSingleton();
    }
    const auto &bstFactions = g_dataHandler->GetFormArray<RE::TESFaction>();
    g_allFactions = ConvertBSTArrayToVector(bstFactions);
}

void InitializeLogging() {
    auto path = logger::log_directory();
    if (!path) {
        report_and_fail("Unable to lookup SKSE logs directory.");
    }
    *path /= PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
    log->set_level(spdlog::level::debug);

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}

extern "C" [[maybe_unused]] __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SKSE::GetPapyrusInterface()->Register(NPE::RegisterPapyrusFunctions);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            InitializeLogging();

            spdlog::info("Loading in TFS...");

            spdlog::info("Loading in all Factions...");
            InitializeGlobalData();

            auto equipEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (equipEventSource) {
                equipEventSource->AddEventSink(&NPE::equipEventHandler);
                spdlog::info("EquipEventHandler registered!");
            }

            auto hitEventSource = RE::ScriptEventSourceHolder::GetSingleton();
            if (hitEventSource) {
                hitEventSource->AddEventSink(&g_hitEventHandler);
                spdlog::info("HitEventHandler registered!");
            }

            Actor *player = PlayerCharacter::GetSingleton();
            if (player) {
                lastCheckTime = std::chrono::steady_clock::now();
                lastCheckDetectionTime = std::chrono::steady_clock::now();
                StartBackgroundTask(player);
            }

            SKSE::GetSerializationInterface()->SetSaveCallback(SaveCallback);
            SKSE::GetSerializationInterface()->SetLoadCallback(LoadCallback);

            spdlog::info("TFS successfully loaded!");
            spdlog::dump_backtrace();
            RE::ConsoleLog::GetSingleton()->Print("TFS successfully loaded!");
        }
    });

    return true;
}
