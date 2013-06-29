/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Ironaya
Complete(%): 100
Comment:
Category: Uldaman
EndScriptData */

#include "ScriptPCH.h"

#define SAY_AGGRO                   -1070000

#define SPELL_ARCINGSMASH           8374
#define SPELL_KNOCKAWAY             10101
#define SPELL_WSTOMP                11876

struct boss_ironayaAI : public ScriptedAI
{
    boss_ironayaAI(Creature* c) : ScriptedAI(c) {}

    uint32 Arcing_Timer;
    bool hasCastedWstomp;
    bool hasCastedKnockaway;

    void Reset()
    {
        Arcing_Timer = 3000;
        hasCastedKnockaway = false;
        hasCastedWstomp = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //If we are <50% hp do knockaway ONCE
        if (!hasCastedKnockaway && me->GetHealth()*2 < me->GetMaxHealth())
        {
            DoCast(me->getVictim(), SPELL_KNOCKAWAY, true);

            // current aggro target is knocked away pick new target
            Unit* Target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);

            if (!Target || Target == me->getVictim())
                Target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);

            if (Target)
                me->TauntApply(Target);

            //Shouldn't cast this agian
            hasCastedKnockaway = true;
        }

        //Arcing_Timer
        if (Arcing_Timer <= diff)
        {
            DoCast(me, SPELL_ARCINGSMASH);
            Arcing_Timer = 13000;
        } else Arcing_Timer -= diff;

        if (!hasCastedWstomp && me->GetHealth()*4 < me->GetMaxHealth())
        {
            DoCast(me, SPELL_WSTOMP);
            hasCastedWstomp = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ironaya(Creature* pCreature)
{
    return new boss_ironayaAI (pCreature);
}

void AddSC_boss_ironaya()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ironaya";
    newscript->GetAI = &GetAI_boss_ironaya;
    newscript->RegisterSelf();
}

