/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Boss_Death_knight_darkreaver
Complete(%): 100
Comment:
Category: Scholomance
EndScriptData */

#include "ScriptPCH.h"

struct boss_death_knight_darkreaverAI : public ScriptedAI
{
    boss_death_knight_darkreaverAI(Creature* c) : ScriptedAI(c) {}

    void Reset()
    {
    }

    void DamageTaken(Unit* /*done_by*/, uint32 &damage)
    {
        if (me->GetHealth() <= damage)
            DoCast(me, 23261, true);   //Summon Darkreaver's Fallen Charger
    }

    void EnterCombat(Unit* /*who*/)
    {
    }
};
CreatureAI* GetAI_boss_death_knight_darkreaver(Creature* pCreature)
{
    return new boss_death_knight_darkreaverAI (pCreature);
}

void AddSC_boss_death_knight_darkreaver()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_death_knight_darkreaver";
    newscript->GetAI = &GetAI_boss_death_knight_darkreaver;
    newscript->RegisterSelf();
}

