/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Golemagg
Complete(%): 90
Comment: Timers need to be confirmed, Golemagg's Trust need to be checked
Category: Molten Core
EndScriptData */

#include "ScriptPCH.h"
#include "molten_core.h"

enum eEnums
{
    SPELL_MAGMASPLASH       = 13879,
    SPELL_PYROBLAST         = 20228,
    SPELL_EARTHQUAKE        = 19798,
    SPELL_ENRAGE            = 19953,
    SPELL_GOLEMAGG_TRUST    = 20553,

    // Core Rager
    EMOTE_LOWHP             = -1409002,
    SPELL_MANGLE            = 19820
};

struct boss_golemaggAI : public ScriptedAI
{
    boss_golemaggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = pCreature->GetInstanceData();
    }

    ScriptedInstance *pInstance;

    uint32 uiPyroblastTimer;
    uint32 uiEarthquakeTimer;
    uint32 uiBuffTimer;
    bool bEnraged;

    void Reset()
    {
        uiPyroblastTimer = 7*IN_MILLISECONDS;              // These timers are probably wrong
        uiEarthquakeTimer = 3*IN_MILLISECONDS;
        uiBuffTimer = 2500;
        bEnraged = false;

        DoCast(me, SPELL_MAGMASPLASH, true);
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if (pInstance)
            pInstance->SetData(DATA_GOLEMAGG_DEATH, 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //Pyroblast
        if (uiPyroblastTimer <= uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_PYROBLAST);

            uiPyroblastTimer = 7*IN_MILLISECONDS;
        }
        else
            uiPyroblastTimer -= uiDiff;

        // Enrage
        if (!bEnraged && me->GetHealth()*100 / me->GetMaxHealth() < 10)
        {
            DoCast(me, SPELL_ENRAGE);
            bEnraged = true;
        }

        // Earthquake
        if (bEnraged)
        {
            if (uiEarthquakeTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_EARTHQUAKE);
                uiEarthquakeTimer = 3*IN_MILLISECONDS;
            }
            else
                uiEarthquakeTimer -= uiDiff;
        }

        /*
        // Golemagg's Trust
        if (uiBuffTimer <= uiDiff)
        {
            DoCast(me, SPELL_GOLEMAGG_TRUST);
            uiBuffTimer = 2500;
        }
        else
            uiBuffTimer -= uiDiff;
        */

        DoMeleeAttackIfReady();
    }
};

struct mob_core_ragerAI : public ScriptedAI
{
    mob_core_ragerAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    ScriptedInstance *pInstance;

    uint32 uiMangleTimer;

    void Reset()
    {
        uiMangleTimer = 7*IN_MILLISECONDS;                 // These times are probably wrong
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage)
    {
        if (me->GetHealth()*100 / me->GetMaxHealth() < 50)
        {
            if (pInstance)
            {
                if (Unit* pGolemagg = Unit::GetUnit(*me, pInstance->GetData64(DATA_GOLEMAGG)))
                {
                    if (pGolemagg->isAlive())
                    {
                        DoScriptText(EMOTE_LOWHP, me);
                        me->SetHealth(me->GetMaxHealth());
                    }
                    else
                        uiDamage = me->GetHealth();
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        // Mangle
        if (uiMangleTimer <= uiDiff)
        {
            DoCast(me->getVictim(),SPELL_MANGLE);
            uiMangleTimer = 10*IN_MILLISECONDS;
        }
        else
            uiMangleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_golemagg(Creature* pCreature)
{
    return new boss_golemaggAI (pCreature);
}

CreatureAI* GetAI_mob_core_rager(Creature* pCreature)
{
    return new mob_core_ragerAI (pCreature);
}

void AddSC_boss_golemagg()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_golemagg";
    newscript->GetAI = &GetAI_boss_golemagg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_core_rager";
    newscript->GetAI = &GetAI_mob_core_rager;
    newscript->RegisterSelf();
}

