/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Ramstein_The_Gorger
Complete(%): 70
Comment:
Category: Stratholme
EndScriptData */

#include "ScriptPCH.h"
 #include "stratholme.h"

#define SPELL_TRAMPLE       5568
#define SPELL_KNOCKOUT    17307

 #define C_MINDLESS_UNDEAD   11030

struct boss_ramstein_the_gorgerAI : public ScriptedAI
{
    boss_ramstein_the_gorgerAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = me->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 Trample_Timer;
    uint32 Knockout_Timer;

    void Reset()
    {
        Trample_Timer = 3000;
        Knockout_Timer = 12000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void JustDied(Unit* /*Killer*/)
    {
        for (uint8 i = 0; i < 30; ++i)
        {
            if (Creature* mob = me->SummonCreature(C_MINDLESS_UNDEAD,3969.35+irand(-10,10),-3391.87+irand(-10,10),119.11,5.91,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,1800000))
                mob->AI()->AttackStart(me->SelectNearestTarget(500));
        }

        if (pInstance)
            pInstance->SetData(TYPE_RAMSTEIN,DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Trample
        if (Trample_Timer <= diff)
        {
            DoCast(me, SPELL_TRAMPLE);
            Trample_Timer = 7000;
        } else Trample_Timer -= diff;

        //Knockout
        if (Knockout_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_KNOCKOUT);
            Knockout_Timer = 10000;
        } else Knockout_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_ramstein_the_gorger(Creature* pCreature)
{
    return new boss_ramstein_the_gorgerAI (pCreature);
}

void AddSC_boss_ramstein_the_gorger()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ramstein_the_gorger";
    newscript->GetAI = &GetAI_boss_ramstein_the_gorger;
    newscript->RegisterSelf();
}
