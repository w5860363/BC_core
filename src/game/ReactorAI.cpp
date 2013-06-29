/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "ByteBuffer.h"
#include "ReactorAI.h"
#include "Errors.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "CreatureAIImpl.h"

#define REACTOR_VISIBLE_RANGE (26.46f)

int
ReactorAI::Permissible(const Creature* creature)
{
    if (creature->isCivilian() || creature->IsNeutralToAll())
        return PERMIT_BASE_REACTIVE;

    return PERMIT_BASE_NO;
}

void
ReactorAI::MoveInLineOfSight(Unit*)
{
}

void
ReactorAI::UpdateAI(const uint32 /*time_diff*/)
{
    // update i_victimGuid if me->getVictim() != 0 and changed
    if (!UpdateVictim())
        return;

    if (me->isAttackReady())
    {
        if (me->IsWithinMeleeRange(me->getVictim()))
        {
            me->AttackerStateUpdate(me->getVictim());
            me->resetAttackTimer();
        }
    }
}

