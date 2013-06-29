/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Doctor_Theolen_Krastinov
Complete(%): 100
Comment:
Category: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

enum eEnums
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,

    SPELL_REND                  = 16509,
    SPELL_BACKHAND              = 18103,
    SPELL_FRENZY                = 8269
};

struct boss_theolenkrastinovAI : public ScriptedAI
{
    boss_theolenkrastinovAI(Creature* c) : ScriptedAI(c) {}

    uint32 m_uiRend_Timer;
    uint32 m_uiBackhand_Timer;
    uint32 m_uiFrenzy_Timer;

    void Reset()
    {
        m_uiRend_Timer = 8000;
        m_uiBackhand_Timer = 9000;
        m_uiFrenzy_Timer = 1000;
    }

    void JustDied(Unit* /*pKiller*/)
    {
        ScriptedInstance* pInstance = me->GetInstanceData();
        if (pInstance)
        {
            pInstance->SetData(DATA_DOCTORTHEOLENKRASTINOV_DEATH, 0);

            if (pInstance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //Rend_Timer
        if (m_uiRend_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_REND);
            m_uiRend_Timer = 10000;
        }
        else
            m_uiRend_Timer -= uiDiff;

        //Backhand_Timer
        if (m_uiBackhand_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_BACKHAND);
            m_uiBackhand_Timer = 10000;
        }
        else
            m_uiBackhand_Timer -= uiDiff;

        //Frenzy_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 26)
        {
            if (m_uiFrenzy_Timer <= uiDiff)
            {
                DoCast(me, SPELL_FRENZY);
                DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                m_uiFrenzy_Timer = 120000;
            }
            else
                m_uiFrenzy_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_theolenkrastinov(Creature* pCreature)
{
    return new boss_theolenkrastinovAI (pCreature);
}

void AddSC_boss_theolenkrastinov()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_doctor_theolen_krastinov";
    newscript->GetAI = &GetAI_boss_theolenkrastinov;
    newscript->RegisterSelf();
}

