/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "ScriptPCH.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_MIND_BLAST                                       = 15587,
    SPELL_SLEEP                                            = 8399,
};

//Id's from ACID
enum Yells
{
    SAY_AGGRO                                              = -1048002,
    SAY_SLEEP                                              = -1048001,
    SAY_DEATH                                              = -1048000
};

struct boss_kelrisAI : public ScriptedAI
{
    boss_kelrisAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    uint32 uiMindBlastTimer;
    uint32 uiSleepTimer;

    ScriptedInstance *pInstance;

    void Reset()
    {
        uiMindBlastTimer = urand(2000,5000);
        uiSleepTimer = urand(9000,12000);
        if (pInstance)
            pInstance->SetData(TYPE_KELRIS, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
        if (pInstance)
            pInstance->SetData(TYPE_KELRIS, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/)
    {
        DoScriptText(SAY_DEATH, me);
        if (pInstance)
            pInstance->SetData(TYPE_KELRIS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (uiMindBlastTimer < diff)
        {
            DoCastVictim(SPELL_MIND_BLAST);
            uiMindBlastTimer = urand(7000,9000);
        } else uiMindBlastTimer -= diff;

        if (uiSleepTimer < diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
            {
                DoScriptText(SAY_SLEEP, me);
                DoCast(pTarget, SPELL_SLEEP);
            }
            uiSleepTimer = urand(15000,20000);
        } else uiSleepTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kelris(Creature* pCreature)
{
    return new boss_kelrisAI (pCreature);
}

void AddSC_boss_kelris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_kelris";
    newscript->GetAI = &GetAI_boss_kelris;
    newscript->RegisterSelf();
}
