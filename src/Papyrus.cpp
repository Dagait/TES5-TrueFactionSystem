#include "Papyrus.h"


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

RE::BGSKeyword *PapyrusHandleAddFactionFromMCM(RE::StaticFunctionTag *, RE::TESFaction *faction) {
    return HandleAddFactionFromMCM(faction);
}

std::vector<std::string> PapyrusGetAssignedKeywords(RE::StaticFunctionTag *) { return GetAssignedKeywords(); }

std::vector<RE::TESFaction *> PapyrusGetAssignedFactions(RE::StaticFunctionTag *) { return GetAssignedFactions(); }

std::vector<RE::TESFaction *> PapyrusGetAllFactions(RE::StaticFunctionTag *) { return GetAllFactions(); }

bool PapyrusRemoveFactionKeywordAssignment(RE::StaticFunctionTag *, RE::BSFixedString keyword,
                                           RE::TESFaction *faction) {
    return RemoveFactionKeywordAssignment(keyword, faction);
}

float PapyrusGetRaceBonusValue(RE::StaticFunctionTag *, RE::TESFaction *faction) {
    return GetRaceBonusValueForFaction(faction);
}


// Function to bind the Papyrus function
bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine *vm) {
    vm->RegisterFunction("AddKeywordToArmor", "npeTFS_NativeFunctions", PapyrusAddKeywordToArmor);
    vm->RegisterFunction("RemoveKeywordFromArmor", "npeTFS_NativeFunctions", PapyrusRemoveKeywordFromArmor);
    vm->RegisterFunction("GetKeywordByEditorID", "npeTFS_NativeFunctions", PapyrusGetKeywordByEditorID);
    vm->RegisterFunction("GetFactionsForActor", "npeTFS_NativeFunctions", PapyrusGetFactionsForActor);
    vm->RegisterFunction("GetDisguiseValueForFaction", "npeTFS_NativeFunctions", PapyrusGetDisguiseValueForFaction);
    vm->RegisterFunction("GetDisguiseBonusValueForFaction", "npeTFS_NativeFunctions",
                         PapyrusGetDisguiseBonusValueForFaction);
    vm->RegisterFunction("GetFactionEditorID", "npeTFS_NativeFunctions", PapyrusGetFactionEditorID);
    vm->RegisterFunction("HandleAddFactionFromMCM", "npeTFS_NativeFunctions", PapyrusHandleAddFactionFromMCM);
    vm->RegisterFunction("GetAllFactions", "npeTFS_NativeFunctions", PapyrusGetAllFactions);
    vm->RegisterFunction("GetAssignedKeywords", "npeTFS_NativeFunctions", PapyrusGetAssignedKeywords);
    vm->RegisterFunction("GetAssignedFactions", "npeTFS_NativeFunctions", PapyrusGetAssignedFactions);
    vm->RegisterFunction("RemoveFactionKeywordAssignment", "npeTFS_NativeFunctions",
                         PapyrusRemoveFactionKeywordAssignment);
    vm->RegisterFunction("GetRaceBonusValueForFaction", "npeTFS_NativeFunctions", PapyrusGetRaceBonusValue);
    return true;
}