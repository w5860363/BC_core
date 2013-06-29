/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_SKILL_EXTRA_ITEMS_H
#define BLIZZLIKE_SKILL_EXTRA_ITEMS_H

#include "Common.h"

// predef classes used in functions
class Player;
// returns true and sets the appropriate info if the player can create extra items with the given spellId
bool canCreateExtraItems(Player* player, uint32 spellId, float &additionalChance, uint8 &additionalMax);
// function to load the extra item creation info from DB
void LoadSkillExtraItemTable();
#endif

