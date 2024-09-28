#include "combat.h"
#include "faction.h"


RE::TESFaction* GetFactionByActor(RE::Actor* actor) {
    if (!actor) {
        return nullptr;
    }

    auto factions = GetRelevantFactions();

    for (const auto& [factionName, faction] : factions) {
        if (actor->IsInFaction(faction)) {
            return faction;
        }
    }

    return nullptr;
}


RE::BSEventNotifyControl HitEventHandler::ProcessEvent(const RE::TESHitEvent* evn,
                                                       RE::BSTEventSource<RE::TESHitEvent>* dispatcher) {
    if (!evn || !evn->target || !evn->cause) {
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::Actor* target = skyrim_cast<RE::Actor*>(evn->target.get());
    RE::Actor* aggressor = skyrim_cast<RE::Actor*>(evn->cause.get());

    if (target && target->IsPlayerRef() && aggressor) {
        RE::ConsoleLog::GetSingleton()->Print("Player was hit by an NPC!");
        RE::TESFaction* faction = GetFactionByActor(aggressor);

        if (faction && target->IsInFaction(faction)) {
            target->AddToFaction(faction, -1);
            RE::ConsoleLog::GetSingleton()->Print("Player removed from faction due to aggression.");
        }
    } else if (aggressor && aggressor->IsPlayerRef() && target) {
        RE::ConsoleLog::GetSingleton()->Print("Player hit an NPC!");
        RE::TESFaction* faction = GetFactionByActor(target);

        if (faction && aggressor->IsInFaction(faction)) {
            aggressor->AddToFaction(faction, -1);
            RE::ConsoleLog::GetSingleton()->Print("Player removed from faction due to aggression.");
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}
