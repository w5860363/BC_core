/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Pandemonius
Complete(%): 75
Comment: Not known how void blast is done (amount of rapid cast seems to be related to players in party). All mobs remaining in surrounding area should aggro when engaged.
Category: Auchindoun, Mana Tombs
EndScriptData */

#include "ScriptPCH.h"

#define SAY_AGGRO_1                     -1557008
#define SAY_AGGRO_2                     -1557009
#define SAY_AGGRO_3                     -1557010

#define SAY_KILL_1                      -1557011
#define SAY_KILL_2                      -1557012

#define SAY_DEATH                       -1557013

#define EMOTE_DARK_SHELL                -1557014

#define SPELL_VOID_BLAST                32325
#define H_SPELL_VOID_BLAST              38760
#define SPELL_DARK_SHELL                32358
#define H_SPELL_DARK_SHELL              38759

struct boss_pandemoniusAI : public ScriptedAI
{
    boss_pandemoniusAI(Creature* c) : ScriptedAI(c)
    {
        HeroicMode = me->GetMap()->IsHeroic();
    }

    bool HeroicMode;
    uint32 VoidBlast_Timer;
    uint32 DarkShell_Timer;
    uint32 VoidBlast_Counter;

    void Reset()
    {
        VoidBlast_Timer = 30000;
        DarkShell_Timer = 20000;
        VoidBlast_Counter = 0;
    }

    void JustDied(Unit*)
    {
        DoScriptText(SAY_DEATH, me);
    }

    void KilledUnit(Unit*)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_KILL_1, me); break;
            case 1: DoScriptText(SAY_KILL_2, me); break;
        }
    }

    void EnterCombat(Unit*)
    {
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO_1, me); break;
            case 1: DoScriptText(SAY_AGGRO_2, me); break;
            case 2: DoScriptText(SAY_AGGRO_3, me); break;
        }

    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (VoidBlast_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            {
                DoCast(pTarget,HeroicMode ? H_SPELL_VOID_BLAST : SPELL_VOID_BLAST);
                VoidBlast_Timer = 500;
                ++VoidBlast_Counter;
            }

            if (VoidBlast_Counter == 5)
            {
                VoidBlast_Timer = 25000+rand()%10000;
                VoidBlast_Counter = 0;
            }
        } else VoidBlast_Timer -= diff;

        if (!VoidBlast_Counter)
        {
            if (DarkShell_Timer <= diff)
            {
                if (me->IsNonMeleeSpellCasted(false))
                    me->InterruptNonMeleeSpells(true);

                DoScriptText(EMOTE_DARK_SHELL, me);

                DoCast(me,HeroicMode ? H_SPELL_DARK_SHELL : SPELL_DARK_SHELL);
                DarkShell_Timer = 20000;
            } else DarkShell_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_pandemonius(Creature* pCreature)
{
    return new boss_pandemoniusAI (pCreature);
}

void AddSC_boss_pandemonius()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_pandemonius";
    newscript->GetAI = &GetAI_boss_pandemonius;
    newscript->RegisterSelf();
}
