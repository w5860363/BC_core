/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_TOTEMAI_H
#define BLIZZLIKE_TOTEMAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Totem;

class TotemAI : public CreatureAI
{
    public:

        TotemAI(Creature* c);

        void MoveInLineOfSight(Unit*);
        void AttackStart(Unit*);
        void EnterEvadeMode();
        bool IsVisible(Unit*) const;

        void UpdateAI(const uint32);
        static int Permissible(const Creature* );

    private:
        Totem &i_totem;
        uint64 i_victimGuid;
};
#endif

