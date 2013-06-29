/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_CREATUREAISELECTOR_H
#define BLIZZLIKE_CREATUREAISELECTOR_H

class CreatureAI;
class Creature;
class MovementGenerator;

namespace FactorySelector
{
    CreatureAI* selectAI(Creature* );
    MovementGenerator* selectMovementGenerator(Creature* );
}
#endif

