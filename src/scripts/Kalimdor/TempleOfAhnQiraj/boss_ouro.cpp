/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Ouro
Complete(%): 85
Comment: No model for submerging. Currently just invisible.
Category: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptPCH.h"
#include "temple_of_ahnqiraj.h"

#define SPELL_SWEEP             26103
#define SPELL_SANDBLAST         26102
#define SPELL_GROUND_RUPTURE    26100
#define SPELL_BIRTH             26262                       //The Birth Animation

#define SPELL_DIRTMOUND_PASSIVE 26092

struct boss_ouroAI : public ScriptedAI
{
    boss_ouroAI(Creature* c) : ScriptedAI(c) {}

    uint32 Sweep_Timer;
    uint32 SandBlast_Timer;
    uint32 Submerge_Timer;
    uint32 Back_Timer;
    uint32 ChangeTarget_Timer;
    uint32 Spawn_Timer;

    bool Enrage;
    bool Submerged;
    bool InCombat;

    void Reset()
    {
        Sweep_Timer = 5000 + rand()%5000;
        SandBlast_Timer = 20000 + rand()%15000;
        Submerge_Timer = 90000 + rand()%60000;
        Back_Timer = 30000 + rand()%15000;
        ChangeTarget_Timer = 5000 + rand()%3000;
        Spawn_Timer = 10000 + rand()%10000;

        Enrage = false;
        Submerged = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoCast(me->getVictim(), SPELL_BIRTH);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Sweep_Timer
        if (!Submerged && Sweep_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SWEEP);
            Sweep_Timer = 15000 + rand()%15000;
        } else Sweep_Timer -= diff;

        //SandBlast_Timer
        if (!Submerged && SandBlast_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SANDBLAST);
            SandBlast_Timer = 20000 + rand()%15000;
        } else SandBlast_Timer -= diff;

        //Submerge_Timer
        if (!Submerged && Submerge_Timer <= diff)
        {
            //Cast
            me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(35);
            DoCast(me, SPELL_DIRTMOUND_PASSIVE);

            Submerged = true;
            Back_Timer = 30000 + rand()%15000;
        } else Submerge_Timer -= diff;

        //ChangeTarget_Timer
        if (Submerged && ChangeTarget_Timer <= diff)
        {
            Unit* pTarget = NULL;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);

            if (pTarget)
                DoTeleportTo(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());

            ChangeTarget_Timer = 10000 + rand()%10000;
        } else ChangeTarget_Timer -= diff;

        //Back_Timer
        if (Submerged && Back_Timer <= diff)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(14);

            DoCast(me->getVictim(), SPELL_GROUND_RUPTURE);

            Submerged = false;
            Submerge_Timer = 60000 + rand()%60000;
        } else Back_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ouro(Creature* pCreature)
{
    return new boss_ouroAI (pCreature);
}

void AddSC_boss_ouro()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ouro";
    newscript->GetAI = &GetAI_boss_ouro;
    newscript->RegisterSelf();
}

