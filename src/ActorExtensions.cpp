# include "ActorExtensions.h"



NPEActor::NPEActor(RE::Actor* a_actor) : actor(a_actor) {
    if (!actor) {
        throw std::invalid_argument("Actor cannot be null");
    }
}

void NPEActor::StartCombatWith(RE::Actor* target) {
}

// Method to set the target as hostile
void NPEActor::SetHostile(RE::Actor* target) {
}
