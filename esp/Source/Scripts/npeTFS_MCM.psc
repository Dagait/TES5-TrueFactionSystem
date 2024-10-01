Scriptname npeTFS_MCM extends SKI_ConfigBase

Import npeTFS_NativeFunctions

; -------- PRIVATE VARS --------
Armor[] wornArmors
int[] _wornArmorMenuOIDs
int selectedArmorIndex = -1
int wornArmorCount = 0

int selectedKeywordIndex = 0

string[] availableKeywordNames
Int[] availableKeywordFormIDs

int _keywordDropdownOID
int _addKeywordTextOptionOID
int _removeKeywordTextOptionOID

string playerInformationPageName = "Player Information"
string armorKeywordSettingPageName = "Armor-Keyword Setting"

Event OnConfigInit()
    ; Initialize worn armors
    wornArmors = GetWornArmors(Game.GetPlayer())
    _wornArmorMenuOIDs = new int[100]
    InitCustomKeywords()

    ; Count how many valid armors the player is wearing
    wornArmorCount = 0
    int index = 0
    while index < wornArmors.Length
        if wornArmors[index] != None
            wornArmorCount += 1
        endif
        index += 1
    endWhile

    ; Define pages for the MCM
    Pages = new string[2]
    Pages[0] = playerInformationPageName
    Pages[1] = armorKeywordSettingPageName
endEvent

Function InitWornArmor()
    wornArmors = GetWornArmors(Game.GetPlayer())
    _wornArmorMenuOIDs = new int[100]
endFunction

Function InitCustomKeywords()
    availableKeywordNames = new string[19]
    availableKeywordFormIDs = new int[19]

    ; Get all my custom keywords by name, because the formid changes with the load order...SAD

    availableKeywordNames[0] = "Bandit Faction"
    availableKeywordFormIDs[0] = GetKeywordByEditorID("npeBanditFaction").GetFormID()

    availableKeywordNames[1] = "Blades Faction"
    availableKeywordFormIDs[1] = GetKeywordByEditorID("npeBladesFaction").GetFormID()

    availableKeywordNames[2] = "Imperial Faction"
    availableKeywordFormIDs[2] = GetKeywordByEditorID("npeImperialFaction").GetFormID()

    availableKeywordNames[3] = "Companions Faction"
    availableKeywordFormIDs[3] = GetKeywordByEditorID("npeCompanionsFaction").GetFormID()

    availableKeywordNames[4] = "Dawnstar Faction"
    availableKeywordFormIDs[4] = GetKeywordByEditorID("npeDawnstarFaction").GetFormID()

    availableKeywordNames[5] = "Falkreath Faction"
    availableKeywordFormIDs[5] = GetKeywordByEditorID("npeFalkreathFaction").GetFormID()

    availableKeywordNames[6] = "Forsworn Faction"
    availableKeywordFormIDs[6] = GetKeywordByEditorID("npeForswornFaction").GetFormID()

    availableKeywordNames[7] = "Markarth Faction"
    availableKeywordFormIDs[7] = GetKeywordByEditorID("npeMarkarthFaction").GetFormID()

    availableKeywordNames[8] = "Morthal Faction"
    availableKeywordFormIDs[8] = GetKeywordByEditorID("npeMorthalFaction").GetFormID()

    availableKeywordNames[9] = "Nightingale Faction"
    availableKeywordFormIDs[9] = GetKeywordByEditorID("npeNightingaleFaction").GetFormID()

    availableKeywordNames[10] = "Riften Faction"
    availableKeywordFormIDs[10] = GetKeywordByEditorID("npeRiftenFaction").GetFormID()

    availableKeywordNames[11] = "SilverHand Faction"
    availableKeywordFormIDs[11] = GetKeywordByEditorID("npeSilverHandFaction").GetFormID()

    availableKeywordNames[12] = "Solitude Faction"
    availableKeywordFormIDs[12] = GetKeywordByEditorID("npeSolitudeFaction").GetFormID()

    availableKeywordNames[13] = "Stormcloak Faction"
    availableKeywordFormIDs[13] = GetKeywordByEditorID("npeStormcloakFaction").GetFormID()

    availableKeywordNames[14] = "Thalmor Faction"
    availableKeywordFormIDs[14] = GetKeywordByEditorID("npeThalmorFaction").GetFormID()

    availableKeywordNames[15] = "Whiterun Faction"
    availableKeywordFormIDs[15] = GetKeywordByEditorID("npeWhiterunFaction").GetFormID()

    availableKeywordNames[16] = "Windhelm Faction"
    availableKeywordFormIDs[16] = GetKeywordByEditorID("npeWindhelmFaction").GetFormID()

    availableKeywordNames[17] = "Winterhold Faction"
    availableKeywordFormIDs[17] = GetKeywordByEditorID("npeWinterholdFaction").GetFormID()

    availableKeywordNames[18] = "Full Covered Face"
    availableKeywordFormIDs[18] = GetKeywordByEditorID("npeCoveredFace").GetFormID()
endFunction

; Function to retrieve keywords associated with a given armor
Keyword[] Function GetArmorKeywords(Armor akArmor)
    if akArmor == None
        return None
    endif

    ; Retrieve the keywords associated with the armor
    Keyword[] keywords = akArmor.GetKeywords()
    return keywords
EndFunction

; Function to retrieve all the armor items the player is currently wearing
Armor[] Function GetWornArmors(Actor target)
    Armor[] wornArmorForms = new Armor[30] ; Max size to handle most scenarios
    int index
    int slotsChecked
    slotsChecked += 0x00100000 ; Ignore reserved slots
    slotsChecked += 0x00200000 ; Ignore reserved slot
    slotsChecked += 0x80000000 ; Ignore FX slot

    int thisSlot = 0x01
    while (thisSlot < 0x80000000)
        if (Math.LogicalAnd(slotsChecked, thisSlot) != thisSlot)
            Armor thisArmor = target.GetWornForm(thisSlot) as Armor
            if (thisArmor)
                wornArmorForms[index] = thisArmor
                index += 1
                slotsChecked += thisArmor.GetSlotMask()
            else
                slotsChecked += thisSlot
            endif
        endif
        thisSlot *= 2
    endWhile
    return wornArmorForms
EndFunction

Function ArmorKeywordPage()
    ; Left panel: list all worn armors
    AddHeaderOption("Worn Armor | Form ID")
    int index = 0
    while index < wornArmorCount
        if wornArmors[index]
            ; Display the names of the worn armors on the left panel
            _wornArmorMenuOIDs[index] = AddTextOption("Worn: " + wornArmors[index].GetName(), wornArmors[index].GetFormID())
        endif
        index += 1
    endWhile

    ; Right panel: display keywords for the selected armor if one is selected
    if selectedArmorIndex != -1 && wornArmors[selectedArmorIndex]
        Armor selectedArmor = wornArmors[selectedArmorIndex]

        AddEmptyOption()
        AddHeaderOption("Selected Armor Keywords")

        ; Retrieve and display keywords for the selected armor
        Keyword[] keywords = GetArmorKeywords(selectedArmor)
        if keywords.Length > 0
            index = 0
            while index < keywords.Length
                AddTextOption("Keyword: " + keywords[index].GetString(), keywords[index].GetFormID())
                index += 1
            endWhile
        else
            AddTextOption("No Keywords Found", 0)
        endif

        ; Dropdown for selecting a new keyword to add
        AddEmptyOption()
        AddHeaderOption("Add Keyword to Armor")
        _keywordDropdownOID = AddMenuOption("Select Keyword", availableKeywordNames[selectedKeywordIndex])

        ; Button to add the selected keyword to the armor
        _addKeywordTextOptionOID = AddTextOption("Add Selected Keyword", "")

        ; Button to add the selected keyword to the armor
        AddEmptyOption()
        _removeKeywordTextOptionOID = AddTextOption("Remove Selected Keyword", "")
    else
        AddEmptyOption()
        AddHeaderOption("No Armor Selected")
    endif
endFunction

Function PlayerInformationPage()
    AddHeaderOption("Player Factions | Disguise Values")

    ; Get the player actor
    Actor player = Game.GetPlayer()

    ; Retrieve the factions the player is currently in
    Faction[] playerFactions = GetFactionsForActor(player)

    if playerFactions.Length > 0
        int index = 0
        while index < playerFactions.Length
            Faction currentFaction = playerFactions[index]

            string factionEditorID = GetFactionEditorID(currentFaction)
            float disguiseValue = GetDisguiseValueForFaction(currentFaction)
            ; Display the faction name (via Faction ID, because most factions does not have a name) and disguise value
            ; Factions EditorID is an empty string??
            AddTextOption("Faction: " + factionEditorID, disguiseValue)
            index += 1
        endWhile
    else
        AddTextOption("No Factions Found", 0)
    endif
endFunction

; This event handles resetting and updating the page
Event OnPageReset(string a_page)
    InitWornArmor()
    if (a_page == playerInformationPageName)
        PlayerInformationPage()
    elseIf (a_page == armorKeywordSettingPageName)
        ArmorKeywordPage()
    endIf
EndEvent

Event OnOptionMenuOpen(int a_option)
    if a_option == _keywordDropdownOID
        ; Populate the menu with available keywords (factions)
        SetMenuDialogOptions(availableKeywordNames)
        SetMenuDialogStartIndex(selectedKeywordIndex)
        SetMenuDialogDefaultIndex(0)
    endif
EndEvent

; This event handles the armor selection and dynamically updates the keywords panel
Event OnOptionSelect(int a_option)
    int index = 0
    bool breakLoop = false
    while !breakLoop && index < wornArmorCount
        if a_option == _wornArmorMenuOIDs[index] && wornArmors[index]
            selectedArmorIndex = index

            ; Dynamically update the page to show keywords by forcing a page reset
            ForcePageReset() ; This forces the page to refresh and re-trigger OnPageReset()
            breakLoop = true
        endif
        index += 1
    endWhile

    ; Handle the Add Keyword button
    if a_option == _addKeywordTextOptionOID && selectedArmorIndex != -1 && wornArmors[selectedArmorIndex]
        ; Retrieve the selected keyword from the list
        Keyword newKeyword = Game.GetFormFromFile(availableKeywordFormIDs[selectedKeywordIndex], "TrueFactionSystem.esp") as Keyword
        Debug.Notification(""+availableKeywordFormIDs[selectedKeywordIndex])
        if AddKeywordToArmor(wornArmors[selectedArmorIndex], newKeyword)
            Debug.Notification("Keyword successfully added to: " + wornArmors[selectedArmorIndex].GetName())
        else
            Debug.Notification("Failed to add keyword.")
        endif

        ; Refresh the menu to show the updated keywords list
        ForcePageReset()
    elseif a_option == _removeKeywordTextOptionOID && selectedArmorIndex != -1 && wornArmors[selectedArmorIndex]
        Keyword newKeyword = Game.GetFormFromFile(availableKeywordFormIDs[selectedKeywordIndex], "TrueFactionSystem.esp") as Keyword
        Debug.Notification(""+availableKeywordFormIDs[selectedKeywordIndex])
        if RemoveKeywordFromArmor(wornArmors[selectedArmorIndex], newKeyword)
            Debug.Notification("Keyword successfully removed from: " + wornArmors[selectedArmorIndex].GetName())
        else
            Debug.Notification("Failed to remove keyword.")
        endif

        ; Refresh the menu to show the updated keywords list
        ForcePageReset()
    endif
EndEvent

; This event handles the selection from the keyword dropdown
Event OnOptionMenuAccept(int a_option, int a_index)
    if a_option == _keywordDropdownOID
        selectedKeywordIndex = a_index
        Debug.Notification("Selected keyword: " + availableKeywordNames[selectedKeywordIndex])

        ; Refresh the menu to show the updated keyword selection
        ForcePageReset()
    endif
EndEvent
