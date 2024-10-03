Scriptname npeTFS_NativeFunctions extends ObjectReference

; -------- NATIVE SKSE FUNCTIONS --------
Bool Function AddKeywordToArmor(Armor akArmor, Keyword akKeyword) global Native
Bool Function RemoveKeywordFromArmor(Armor akArmor, Keyword akKeyword) global Native
Keyword Function GetKeywordByEditorID(string akKeywordName) global Native
Faction[] Function GetFactionsForActor(Actor akActor) global Native
Faction[] Function GetAllFactions() global Native
float Function GetDisguiseValueForFaction(Faction akFaction) global Native
float Function GetDisguiseBonusValueForFaction(Faction akFaction) global Native
string Function GetFactionEditorID(Faction akFaction) global Native
Keyword Function HandleAddFactionFromMCM(Faction akFaction) global Native
string[] Function GetAssignedKeywords() global Native
Faction[] Function GetAssignedFactions() global Native