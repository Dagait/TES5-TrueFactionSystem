# True Faction Manager (TFM)

True Faction Manager is an SKSE plugin for Skyrim Special Edition that allows the player to act as a member of different factions depending on the armor worn. The plugin uses dynamic faction detection based on the faction armor worn and adjusts the behavior of NPCs accordingly. 

## Functionalities

- **Dynamic faction affiliation**: If the player is wearing armor associated with a specific faction (e.g. Bandits, Imperials, Stormcloaks), they will be recognized as an ally to NPCs of that faction and will not be attacked.
  
- **Recognition mechanism based on disguise**: The player is not immediately recognized by faction NPCs. The probability of being detected depends on the armor worn and how close the player approaches the NPCs.

- **Armor-based camouflage**: Each armor has a “Disguise Value”. The higher this value, the less likely the player is to be detected. If the disguise value is below a certain threshold, the player will be recognized as an enemy by faction NPCs and attacked.

## Installation instructions

1. make sure that you have **SKSE64** (Skyrim Script Extender) installed.
2. copy the `.dll` file into your Skyrim `SKSE/Plugins` folder.
3. load the game and test the plugin by wearing faction armor and approaching the associated faction NPCs.

## Instructions for use

- If you are wearing a faction's armor, such as bandit armor, you can walk into a bandit camp undetected. The NPCs will only attack you when they realize that you are not a real bandit (depending on your stealth value and proximity).
- If you are not wearing faction armor or your stealth level is too low, the NPCs will regard you as an enemy as usual.

## Development and future goals

### Current features:

- **Faction-specific recognition**: Depending on the armor, the system recognizes which faction the player might belong to.
- **Probability of detection**: The armor's stealth value affects how likely the player is to be detected.
- **Combat behavior**: NPCs attack the player if they recognize him as an enemy.

### Planned features:

-[ ] **Background detection system**: Implementation of a background process that automatically checks faction affiliation and detection probability every few seconds without the player having to change their armor.
-[ ] **Advanced faction integration**: Support for more factions such as the Thalmor, Thieves Guild and others.
-[ ] **Improved detection logic**: Consideration of NPC level, line of sight, environment and other factors in faction detection.
-[ ] **Quest integration**: Possible integration into existing quests to adjust NPC behavior depending on player stealth.
-[ ] **Mod support**: Support for custom faction armor and factions from mods.

## Known issues

- Currently, the faction and camouflage check is only performed when the player changes their armor. The implementation of a background process that regularly checks this is still in progress.
