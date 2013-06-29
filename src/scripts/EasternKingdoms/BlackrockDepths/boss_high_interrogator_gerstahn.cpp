/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_High_Interrogator_Gerstahn
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_SHADOWWORDPAIN                                   = 10894,
    SPELL_MANABURN                                         = 10876,
    SPELL_PSYCHICSCREAM                                    = 8122,
    SPELL_SHADOWSHIELD                                     = 22417
};

struct boss_high_interrogator_gerstahnAI : public ScriptedAI
{
    boss_high_interrogator_gerstahnAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowWordPain_Timer;
    uint32 ManaBurn_Timer;
    uint32 PsychicScream_Timer;
    uint32 ShadowShield_Timer;

    void Reset()
    {
        ShadowWordPain_Timer = 4000;
        ManaBurn_Timer = 14000;
        PsychicScream_Timer = 32000;
        ShadowShield_Timer = 8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //ShadowWordPain_Timer
        if (ShadowWordPain_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_SHADOWWORDPAIN);
            ShadowWordPain_Timer = 7000;
        } else ShadowWordPain_Timer -= diff;

        //ManaBurn_Timer
        if (ManaBurn_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_MANABURN);
            ManaBurn_Timer = 10000;
        } else ManaBurn_Timer -= diff;

        //PsychicScream_Timer
        if (PsychicScream_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_PSYCHICSCREAM);
            PsychicScream_Timer = 30000;
        } else PsychicScream_Timer -= diff;

        //ShadowShield_Timer
        if (ShadowShield_Timer <= diff)
        {
            DoCast(me, SPELL_SHADOWSHIELD);
            ShadowShield_Timer = 25000;
        } else ShadowShield_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_high_interrogator_gerstahn(Creature* pCreature)
{
    return new boss_high_interrogator_gerstahnAI (pCreature);
}

void AddSC_boss_high_interrogator_gerstahn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_high_interrogator_gerstahn";
    newscript->GetAI = &GetAI_boss_high_interrogator_gerstahn;
    newscript->RegisterSelf();
}
