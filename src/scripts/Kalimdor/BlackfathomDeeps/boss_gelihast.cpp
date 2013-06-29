/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "ScriptPCH.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_NET                                              = 6533
};

struct boss_gelihastAI : public ScriptedAI
{
    boss_gelihastAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    uint32 uiNetTimer;

    ScriptedInstance *pInstance;

    void Reset()
    {
        uiNetTimer = urand(2000,4000);
        if (pInstance)
            pInstance->SetData(TYPE_GELIHAST, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/)
    {
        if (pInstance)
            pInstance->SetData(TYPE_GELIHAST, IN_PROGRESS);
    }

    void JustDied(Unit* /*killer*/)
    {
        if (pInstance)
            pInstance->SetData(TYPE_GELIHAST, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (uiNetTimer < diff)
        {
            DoCastVictim(SPELL_NET);
            uiNetTimer = urand(4000,7000);
        } else uiNetTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gelihast(Creature* pCreature)
{
    return new boss_gelihastAI (pCreature);
}

void AddSC_boss_gelihast()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_gelihast";
    newscript->GetAI = &GetAI_boss_gelihast;
    newscript->RegisterSelf();
}
