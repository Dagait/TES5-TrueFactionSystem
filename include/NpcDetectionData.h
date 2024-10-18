#pragma once


namespace NPE {
    /**
     * @brief Structure to store data related to NPC detection of the player.
     *
     * This structure keeps track of the NPC's ID and the last time (in in-game hours) the player was detected by the
     * NPC.
     */
    struct NPCDetectionData {
        RE::FormID npcID;        /// The FormID of the NPC who detected the player.
        float lastDetectedTime;  /// The in-game time (in hours) when the NPC last detected the player.
    };

}