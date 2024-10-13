#include "Combat.h"

constexpr float REJOIN_COOLDOWN = 2.0f;

std::unordered_map<RE::TESFaction*, float> factionCooldowns;


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

void StartCombat(RE::Actor* npc, RE::Actor* player, RE::TESFaction* npcFaction) {
    // TODO: Rework
    if (!npc || !player) {
        return;
    }

    if (npcFaction) {
        player->AddToFaction(npcFaction, -1);
    }
}

void CheckAndReAddPlayerToFaction(RE::Actor* player) {
    auto now = RE::Calendar::GetSingleton()->GetHoursPassed();
    auto factions = GetRelevantFactions();

    for (const auto& [factionName, faction] : factions) {
        float disguiseValue = playerDisguiseStatus.GetDisguiseValue(faction);

        if (factionCooldowns.find(faction) != factionCooldowns.end()) {
            auto timeSinceRemoved = now - factionCooldowns[faction];
            if (timeSinceRemoved < REJOIN_COOLDOWN) {
                continue;
            } else if (timeSinceRemoved >= REJOIN_COOLDOWN && disguiseValue >= 5.0f && !player->IsInFaction(faction)) {
                player->AddToFaction(faction, 1);
                factionCooldowns.erase(faction);
            }
        }
    }
}

RE::BSEventNotifyControl HitEventHandler::ProcessEvent(const RE::TESHitEvent* evn,
                                                       RE::BSTEventSource<RE::TESHitEvent>* dispatcher) {
    if (!evn || !evn->target || !evn->cause) {
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::Actor* target = skyrim_cast<RE::Actor*>(evn->target.get());
    RE::Actor* aggressor = skyrim_cast<RE::Actor*>(evn->cause.get());

    if (target && target->IsPlayerRef() && aggressor) {
        RE::TESFaction* faction = GetFactionByActor(aggressor);

        if (faction && target->IsInFaction(faction)) {
            target->AddToFaction(faction, -1);
            factionCooldowns[faction] = RE::Calendar::GetSingleton()->GetHoursPassed();
        }
    } else if (aggressor && aggressor->IsPlayerRef() && target) {
        RE::TESFaction* faction = GetFactionByActor(target);

        if (faction && aggressor->IsInFaction(faction)) {
            aggressor->AddToFaction(faction, -1);
            factionCooldowns[faction] = RE::Calendar::GetSingleton()->GetHoursPassed();
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}
