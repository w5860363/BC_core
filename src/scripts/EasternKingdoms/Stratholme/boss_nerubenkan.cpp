/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Nerubenkan
Complete(%): 70
Comment:
Category: Stratholme
EndScriptData */

#include "ScriptPCH.h"
#include "stratholme.h"

#define SPELL_ENCASINGWEBS          4962
#define SPELL_PIERCEARMOR           6016
#define SPELL_CRYPT_SCARABS         31602
#define SPELL_RAISEUNDEADSCARAB     17235

struct boss_nerubenkanAI : public ScriptedAI
{
    boss_nerubenkanAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = me->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 EncasingWebs_Timer;
    uint32 PierceArmor_Timer;
    uint32 CryptScarabs_Timer;
    uint32 RaiseUndeadScarab_Timer;

    void Reset()
    {
        CryptScarabs_Timer = 3000;
        EncasingWebs_Timer = 7000;
        PierceArmor_Timer = 19000;
        RaiseUndeadScarab_Timer = 3000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void JustDied(Unit* /*Killer*/)
    {
        if (pInstance)
            pInstance->SetData(TYPE_NERUB,IN_PROGRESS);
    }

    void RaiseUndeadScarab(Unit* pVictim)
    {
        if (Creature* pUndeadScarab = DoSpawnCreature(10876, irand(-9,9), irand(-9,9), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000))
            if (pUndeadScarab->AI())
                pUndeadScarab->AI()->AttackStart(pVictim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //EncasingWebs
        if (EncasingWebs_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_ENCASINGWEBS);
            EncasingWebs_Timer = 30000;
        } else EncasingWebs_Timer -= diff;

        //PierceArmor
        if (PierceArmor_Timer <= diff)
        {
            if (urand(0,3) < 2)
                DoCast(me->getVictim(), SPELL_PIERCEARMOR);
            PierceArmor_Timer = 35000;
        } else PierceArmor_Timer -= diff;

        //CryptScarabs_Timer
        if (CryptScarabs_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CRYPT_SCARABS);
            CryptScarabs_Timer = 20000;
        } else CryptScarabs_Timer -= diff;

        //RaiseUndeadScarab
        if (RaiseUndeadScarab_Timer <= diff)
        {
            RaiseUndeadScarab(me->getVictim());
            RaiseUndeadScarab_Timer = 16000;
        } else RaiseUndeadScarab_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_nerubenkan(Creature* pCreature)
{
    return new boss_nerubenkanAI (pCreature);
}

void AddSC_boss_nerubenkan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_nerubenkan";
    newscript->GetAI = &GetAI_boss_nerubenkan;
    newscript->RegisterSelf();
}

