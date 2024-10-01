Scriptname npeTFS_NativeFunctions extends ObjectReference

; -------- NATIVE SKSE FUNCTIONS --------
Bool Function AddKeywordToArmor(Armor akArmor, Keyword akKeyword) global Native
Bool Function RemoveKeywordFromArmor(Armor akArmor, Keyword akKeyword) global Native
Keyword Function GetKeywordByEditorID(string akKeywordName) global Native
Faction[] Function GetFactionsForActor(Actor akActor) global Native
float Function GetDisguiseValueForFaction(Faction akFaction) global Native
string Function GetFactionEditorID(Faction akFaction) global Native
