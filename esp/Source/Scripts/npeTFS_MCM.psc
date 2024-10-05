Scriptname npeTFS_MCM extends SKI_ConfigBase

Import npeTFS_NativeFunctions

; -------- PRIVATE VARS --------
Armor[] wornArmors
Faction[] availableFactions
int[] _wornArmorMenuOIDs
int[] _availableFactionsMenuOIDs
int[] _factionKeywordAssignementsOIDs
int selectedArmorIndex = -1
int selectedFactionIndex = -1
int selectedKeywordFactionIndex = -1
int wornArmorCount = 0

int selectedKeywordIndex = 0

string[] availableKeywordNames
Int[] availableKeywordFormIDs

int maxFactions = 50 ; Because papyrus does not like dynamic arrays, I have to limit it
int currentFactionCount = 0 ; max factions are 50

int _keywordDropdownOID
int _addKeywordTextOptionOID
int _removeKeywordTextOptionOID
int _resetModTextOptionOID
int _addFactionOptionOID
int _removeFactionKeywordAssignementOID

string[] assignedKeywordsManage
Faction[] assignedFactionsManage

string playerInformationPageName = "Player Information"
string armorKeywordSettingPageName = "Armor-Keyword Setting"
string factionManagementPageName = "Assign Factions as Disguise"
string factionOverviewPageName = "Disguise Faction Overview"
string modSettingsPageName = "Settings"

Event OnConfigInit()
    LoadCustomContent("skyui/TrueFactionSystem/TFS.dds")
    wornArmors = GetWornArmors(Game.GetPlayer())
    availableFactions = GetAllFactions()
    _wornArmorMenuOIDs = new int[100]
    _availableFactionsMenuOIDs = new int[100]
    _factionKeywordAssignementsOIDs = new int[100]

    InitCustomKeywords()
    InitAvailableFactions()

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
    Pages = new string[5]
    Pages[0] = playerInformationPageName
    Pages[1] = armorKeywordSettingPageName
    Pages[2] = factionManagementPageName
    Pages[3] = factionOverviewPageName
    Pages[4] = modSettingsPageName
endEvent

Function InitWornArmor()
    wornArmors = GetWornArmors(Game.GetPlayer())
    _wornArmorMenuOIDs = new int[100]
endFunction

Function InitAssignedKeywordFactionPair()
    assignedKeywordsManage = GetAssignedKeywords()
    assignedFactionsManage = GetAssignedFactions()
endFunction

Function InitAvailableFactions()
    availableFactions = GetAllFactions()
    _availableFactionsMenuOIDs = new int[100]
endFunction

Function InitCustomKeywords()
    ; Cannot use maxFactions for array init, because of reasons.
    availableKeywordNames = new string[50]
    availableKeywordFormIDs = new int[50]

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

    currentFactionCount = 19

    int index = currentFactionCount
    while index < maxFactions
        availableKeywordNames[index] = ""
        availableKeywordFormIDs[index] = 0
        index += 1
    endWhile
endFunction

Function AddNewFaction(string factionName, Keyword factionKeyword)
    if currentFactionCount >= maxFactions
        Debug.Notification("Cannot add more factions. Limit reached.")
        return
    endif

    ; Add the new faction and its keyword
    availableKeywordNames[currentFactionCount] = factionName
    availableKeywordFormIDs[currentFactionCount] = factionKeyword.GetFormID()
    currentFactionCount += 1

    Debug.Notification("Added faction: " + factionName)
endFunction

Function RemoveNewFaction(string factionName, Keyword factionKeyword)
    int removeIndex = -1
    
    int index = 0
    bool break = false
    while !break && index < currentFactionCount
        if availableKeywordNames[index] == factionName
            removeIndex = index
            break = true
        endif
        index += 1
    endWhile

    if removeIndex != -1
        while removeIndex < currentFactionCount - 1
            availableKeywordNames[removeIndex] = availableKeywordNames[removeIndex + 1]
            availableKeywordFormIDs[removeIndex] = availableKeywordFormIDs[removeIndex + 1]
            removeIndex += 1
        endWhile

        availableKeywordNames[currentFactionCount - 1] = ""
        availableKeywordFormIDs[currentFactionCount - 1] = 0
        currentFactionCount -= 1

        Debug.Notification("Removed faction: " + factionName)
    else
        Debug.Notification("Faction not found: " + factionName)
    endif
EndFunction


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
    AddHeaderOption("Worn Armor | Form ID")
    AddEmptyOption()
    AddEmptyOption()
    int index = 0
    while index < wornArmorCount
        if wornArmors[index]
            _wornArmorMenuOIDs[index] = AddTextOption("Worn: " + wornArmors[index].GetName(), wornArmors[index].GetFormID())
        endif
        index += 1
    endWhile
    AddEmptyOption()

    if selectedArmorIndex != -1 && wornArmors[selectedArmorIndex]
        Armor selectedArmor = wornArmors[selectedArmorIndex]

        AddEmptyOption()
        AddHeaderOption("Selected Armor Keywords")
        AddEmptyOption()

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

        AddEmptyOption()
        AddEmptyOption()
        AddHeaderOption("Add Keyword to Armor")
        _keywordDropdownOID = AddMenuOption("Select Keyword", availableKeywordNames[selectedKeywordIndex])
        AddEmptyOption()

        _addKeywordTextOptionOID = AddTextOption("Add Selected Keyword", "")

        AddEmptyOption()
        _removeKeywordTextOptionOID = AddTextOption("Remove Selected Keyword", "")
    else
        AddEmptyOption()
        AddHeaderOption("No Armor Selected")
    endif
endFunction

Function PlayerInformationPage()
    AddHeaderOption("Player Factions | Disguise Values")

    Actor player = Game.GetPlayer()

    Faction[] playerFactions = GetFactionsForActor(player)

    ; For the race bonus value, store the values
    float raceBonusValue = 0

    if playerFactions.Length > 0
        int index = 0
        while index < playerFactions.Length
            Faction currentFaction = playerFactions[index]
            float tempRaceBonusValue = 0
            tempRaceBonusValue = GetRaceBonusValueForFaction(currentFaction)
            if tempRaceBonusValue > 0
                raceBonusValue = tempRaceBonusValue  
            endif

            string factionEditorID = GetFactionEditorID(currentFaction)
            float disguiseValue = GetDisguiseValueForFaction(currentFaction)
            float disguiseBonusValue = GetDisguiseBonusValueForFaction(currentFaction)
            ; Display the faction name (via Faction ID, because most factions does not have a name) and disguise value (+BonusValue +RaceBonusValue)
            AddTextOption("Faction: " + factionEditorID, Math.Floor(disguiseValue) + " (+" + Math.Floor(disguiseBonusValue) + " +" + Math.Floor(tempRaceBonusValue) + ")")
            index += 1
        endWhile
    else
        AddTextOption("No Factions Found", 0)
    endif

    AddEmptyOption()
    AddEmptyOption()
    AddHeaderOption("Player Race Bonus Value")
    AddEmptyOption()
    Race playerRace = player.GetRace()
    AddTextOption("Race: " + playerRace.GetName(), Math.Floor(raceBonusValue), 0)
endFunction

Function SettingsPage()
    AddHeaderOption("General Settings")
    AddEmptyOption()
    ; Reload the factions, because of modded factions, if the user changes the load order, the FormID changes!
    _resetModTextOptionOID = AddTextOption("Reset Mod", "", 1) ; Not implemented!
endFunction

Function FactionManagementPage()
    int index = 0
    
    AddEmptyOption()
    
    Faction selectedFaction = availableFactions[selectedFactionIndex]
    string factionEditorID = GetFactionEditorID(selectedFaction)

    AddEmptyOption()
    AddHeaderOption("Selected Faction")
    if selectedFaction
        AddTextOption("Faction: " + selectedFaction.GetName(), factionEditorID, 1)
    else
        AddTextOption("Faction: No Faction selected", "", 1)
    endif
    

    ; Dropdown for selecting a new keyword to add
    AddEmptyOption()
    ; Button to add the selected keyword to the armor
    AddEmptyOption()
    int flag = 0
    if !selectedFaction
        flag = 1
    endif
    _addFactionOptionOID = AddTextOption("Add Selected Faction as Disguise Valid", "", flag)
    AddEmptyOption()

    int factionCount = availableFactions.Length
    AddEmptyOption()
    AddEmptyOption()
    AddHeaderOption("Select Faction")
    AddEmptyOption()
    AddEmptyOption()
    if factionCount > 0
        ; List all available factions
        index = 0
        while index < factionCount
            Faction currentFaction = availableFactions[index]
            factionEditorID = GetFactionEditorID(currentFaction)

            ; Display the faction name and store its index for future selection
            _availableFactionsMenuOIDs[index] = AddTextOption(currentFaction.GetName(), factionEditorID)
            index += 1
        endWhile
    else
        AddTextOption("No Factions Available", "")
    endif
endFunction

Function DisguiseKeywordAssignmentsPage()
    AddHeaderOption("Disguise Keyword Assignments")
    if assignedFactionsManage.Length == 0 || assignedKeywordsManage.Length == 0
        Debug.Notification("Trying to reinitialize Keyword Assignments...")
        InitAssignedKeywordFactionPair()
    endif
    
    int index = 0
    ; assignedKeywordManage and assignedFactionManage should have the same length!
    while index < assignedKeywordsManage.Length
        string currentKeyword = assignedKeywordsManage[index]
        Faction currentFaction = assignedFactionsManage[index]
        
        _factionKeywordAssignementsOIDs[index] = AddTextOption(currentKeyword, currentFaction.GetName())

        index += 1
    endWhile
    AddEmptyOption()
    AddEmptyOption()
    int flag = 1
    if selectedKeywordFactionIndex >= 0
        flag = 0
    endif

    _removeFactionKeywordAssignementOID = AddTextOption("Remove Keyword-Faction assignement", "", flag)
endFunction

; This event handles resetting and updating the page
Event OnPageReset(string a_page)
    UnloadCustomContent()
    if (a_page == playerInformationPageName)
        PlayerInformationPage()
    elseif (a_page == armorKeywordSettingPageName)
        InitWornArmor()
        ArmorKeywordPage()
    elseif (a_page == factionManagementPageName)
        FactionManagementPage()
    elseif (a_page == factionOverviewPageName)
        InitAssignedKeywordFactionPair()
        DisguiseKeywordAssignmentsPage()
    elseif (a_page == modSettingsPageName)
        SettingsPage()
    endif
EndEvent

Event OnOptionMenuOpen(int a_option)
    if a_option == _keywordDropdownOID
        ; Populate the menu with available keywords (factions)
        SetMenuDialogOptions(availableKeywordNames)
        SetMenuDialogStartIndex(selectedKeywordIndex)
        SetMenuDialogDefaultIndex(0)
    endif
EndEvent

Function HandleArmorSelection(int a_option)
    int index = 0
    bool breakLoop = false
    while !breakLoop && index < wornArmorCount
        if a_option == _wornArmorMenuOIDs[index] && wornArmors[index]
            selectedArmorIndex = index
            ForcePageReset()
            breakLoop = true
        endif
        index += 1
    endWhile
endFunction

Function HandleKeywordFactionSelection(int a_option)
    int index = 0
    bool breakLoop = false
    while !breakLoop && index < _factionKeywordAssignementsOIDs.Length
        if a_option == _factionKeywordAssignementsOIDs[index]
            selectedKeywordFactionIndex = index
            ForcePageReset()
            breakLoop = true
        endif
        index += 1
    endWhile
endFunction

Function HandleFactionSelection(int a_option)
    int index = 0
    bool breakLoop = false
    while !breakLoop && index < availableFactions.Length
        if a_option == _availableFactionsMenuOIDs[index] && availableFactions[index]
            Debug.Notification("Selected Faction: " + availableFactions[index].GetName())
            selectedFactionIndex = index
            ForcePageReset()
            breakLoop = true
        endif
        index += 1
    endWhile
endFunction

Function HandleAddKeywordToArmor()
    Keyword newKeyword = Game.GetFormFromFile(availableKeywordFormIDs[selectedKeywordIndex], "TrueFactionSystem.esp") as Keyword
    Debug.Notification(""+availableKeywordFormIDs[selectedKeywordIndex])
    if AddKeywordToArmor(wornArmors[selectedArmorIndex], newKeyword)
        Debug.Notification("Keyword successfully added to: " + wornArmors[selectedArmorIndex].GetName())
    else
        Debug.Notification("Failed to add keyword.")
    endif
endFunction

Function HandleRemoveKeywordFromArmor()
    Keyword newKeyword = Game.GetFormFromFile(availableKeywordFormIDs[selectedKeywordIndex], "TrueFactionSystem.esp") as Keyword
    Debug.Notification(""+availableKeywordFormIDs[selectedKeywordIndex])
    if RemoveKeywordFromArmor(wornArmors[selectedArmorIndex], newKeyword)
        Debug.Notification("Keyword successfully removed from: " + wornArmors[selectedArmorIndex].GetName())
    else
        Debug.Notification("Failed to remove keyword.")
    endif
endFunction

Function HandleAddFactionAsKeyword()
    Faction selectedFaction = availableFactions[selectedFactionIndex]
    if selectedFaction
        Keyword newKeyword = HandleAddFactionFromMCM(selectedFaction)  ; Call native function to add the faction
        if newKeyword
            string factionEditorID = GetFactionEditorID(selectedFaction)
            AddNewFaction(selectedFaction.GetName(), newKeyword)
        else
            Debug.Notification("Failed to add faction: " + selectedFaction.GetName())
        endif
    endif
endFunction

Function HandleRemoveKeywordFactionAssignements()
    if RemoveFactionKeywordAssignment(assignedKeywordsManage[selectedKeywordFactionIndex], assignedFactionsManage[selectedKeywordFactionIndex])
        Keyword factionKeyword = GetKeywordByEditorID(assignedKeywordsManage[selectedKeywordFactionIndex])
        RemoveNewFaction(assignedFactionsManage[selectedKeywordFactionIndex].GetName(), factionKeyword)
        InitAssignedKeywordFactionPair()
        selectedKeywordFactionIndex = -1
        Debug.Notification("Removed keyword: " + assignedKeywordsManage[selectedKeywordFactionIndex] + " from faction: " + assignedFactionsManage[selectedKeywordFactionIndex].GetName())
    else
        Debug.Notification("Failed to remove Keyword-Faction assignement!")
    endif
endFunction

Event OnOptionSelect(int a_option)
    HandleFactionSelection(a_option)
    HandleArmorSelection(a_option)
    HandleKeywordFactionSelection(a_option)
    

    if a_option == _addKeywordTextOptionOID && selectedArmorIndex >= 0 && wornArmors[selectedArmorIndex]
        HandleAddKeywordToArmor()
    elseif a_option == _removeKeywordTextOptionOID && selectedArmorIndex >= 0 && wornArmors[selectedArmorIndex]
        HandleRemoveKeywordFromArmor()
    endif
    
    if a_option == _addFactionOptionOID && selectedFactionIndex >= 0 && selectedFactionIndex < availableFactions.Length && availableFactions[selectedFactionIndex]
        HandleAddFactionAsKeyword()
    endif

    if a_option == _removeFactionKeywordAssignementOID && selectedKeywordFactionIndex >= 0
        HandleRemoveKeywordFactionAssignements()
    endif

    ForcePageReset()
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