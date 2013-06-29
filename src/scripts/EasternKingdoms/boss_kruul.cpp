/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Kruul
Complete(%): 100
Comment: Highlord Kruul are presumably no longer in-game on regular bases, however future events could bring him back.
Category: Bosses
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_SHADOWVOLLEY          21341
#define SPELL_CLEAVE                20677
#define SPELL_THUNDERCLAP           23931
#define SPELL_TWISTEDREFLECTION     21063
#define SPELL_VOIDBOLT              21066
#define SPELL_RAGE                  21340
#define SPELL_CAPTURESOUL           21054

struct boss_kruulAI : public ScriptedAI
{
    boss_kruulAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowVolley_Timer;
    uint32 Cleave_Timer;
    uint32 ThunderClap_Timer;
    uint32 TwistedReflection_Timer;
    uint32 VoidBolt_Timer;
    uint32 Rage_Timer;
    uint32 Hound_Timer;

    void Reset()
    {
        ShadowVolley_Timer = 10000;
        Cleave_Timer = 14000;
        ThunderClap_Timer = 20000;
        TwistedReflection_Timer = 25000;
        VoidBolt_Timer = 30000;
        Rage_Timer = 60000;                                 //Cast rage after 1 minute
        Hound_Timer = 8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void KilledUnit()
    {
        // When a player, pet or totem gets killed, Lord Kazzak casts this spell to instantly regenerate 70,000 health.
        DoCast(me, SPELL_CAPTURESOUL);
    }

    void SummonHounds(Unit* pVictim)
    {
        if (Creature* Hound = DoSpawnCreature(19207, irand(-9,9), irand(-9,9), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000))
            Hound->AI()->AttackStart(pVictim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //ShadowVolley_Timer
        if (ShadowVolley_Timer <= diff)
        {
            if (urand(0,99) < 45)
                DoCast(me->getVictim(), SPELL_SHADOWVOLLEY);

            ShadowVolley_Timer = 5000;
        } else ShadowVolley_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            if (urand(0,1))
                DoCast(me->getVictim(), SPELL_CLEAVE);

            Cleave_Timer = 10000;
        } else Cleave_Timer -= diff;

        //ThunderClap_Timer
        if (ThunderClap_Timer <= diff)
        {
            if (urand(0,9) < 2)
                DoCast(me->getVictim(), SPELL_THUNDERCLAP);

            ThunderClap_Timer = 12000;
        } else ThunderClap_Timer -= diff;

        //TwistedReflection_Timer
        if (TwistedReflection_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_TWISTEDREFLECTION);
            TwistedReflection_Timer = 30000;
        } else TwistedReflection_Timer -= diff;

        //VoidBolt_Timer
        if (VoidBolt_Timer <= diff)
        {
            if (urand(0,9) < 4)
                DoCast(me->getVictim(), SPELL_VOIDBOLT);

            VoidBolt_Timer = 18000;
        } else VoidBolt_Timer -= diff;

        //Rage_Timer
        if (Rage_Timer <= diff)
        {
            DoCast(me, SPELL_RAGE);
            Rage_Timer = 70000;
        } else Rage_Timer -= diff;

        //Hound_Timer
        if (Hound_Timer <= diff)
        {
            SummonHounds(me->getVictim());
            SummonHounds(me->getVictim());
            SummonHounds(me->getVictim());

            Hound_Timer = 45000;
        } else Hound_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_kruul(Creature* pCreature)
{
    return new boss_kruulAI (pCreature);
}

void AddSC_boss_kruul()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kruul";
    newscript->GetAI = &GetAI_boss_kruul;
    newscript->RegisterSelf();
}
