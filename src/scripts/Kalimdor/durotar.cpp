/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Durotar
Complete(%): 100
Comment: Quest support: 5441.
Category: Durotar
EndScriptData */

/* ContentData
npc_lazy_peon
EndContentData */

#include "ScriptPCH.h"

/*######
## npc_lazy_peon
######*/

enum LazyPeon
{
    SAY_SPELL_HIT             = -1000622,

    QUEST_LAZY_PEONS          = 5441,
    GO_LUMBERPILE             = 175784,
    SPELL_BUFF_SLEEP          = 17743,
    SPELL_AWAKEN_PEON         = 19938
};

struct npc_lazy_peonAI : public ScriptedAI
{
    npc_lazy_peonAI(Creature* c) : ScriptedAI(c) {}

    uint64 uiPlayerGUID;

    uint32 m_uiRebuffTimer;
    bool work;

    void Reset ()
    {
        uiPlayerGUID = 0;
        m_uiRebuffTimer = 0;
        work = false;
    }

    void MovementInform(uint32 /*type*/, uint32 id)
    {
        if (id == 1)
        {
            work = true;
            if (GameObject* Lumberpile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                me->SetFacingToObject(Lumberpile);
        }
    }

    void SpellHit(Unit* caster, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_AWAKEN_PEON && caster->GetTypeId() == TYPEID_PLAYER
            && CAST_PLR(caster)->GetQuestStatus(QUEST_LAZY_PEONS) == QUEST_STATUS_INCOMPLETE)
        {
            caster->ToPlayer()->KilledMonsterCredit(me->GetEntry(),me->GetGUID());
            DoScriptText(SAY_SPELL_HIT, me, caster);
            me->RemoveAllAuras();
            if (GameObject* Lumberpile = me->FindNearestGameObject(GO_LUMBERPILE, 20))
                me->GetMotionMaster()->MovePoint(1,Lumberpile->GetPositionX()-1,Lumberpile->GetPositionY(),Lumberpile->GetPositionZ());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (work == true)
            me->HandleEmoteCommand(466);

        if (m_uiRebuffTimer <= uiDiff)
        {
            DoCast(me, SPELL_BUFF_SLEEP);
            m_uiRebuffTimer = 300000;        //Rebuff agian in 5 minutes
        }
        else
            m_uiRebuffTimer -= uiDiff;

        if (!UpdateVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_lazy_peon(Creature* pCreature)
{
    return new npc_lazy_peonAI(pCreature);
}

void AddSC_durotar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_lazy_peon";
    newscript->GetAI = &GetAI_npc_lazy_peon;
    newscript->RegisterSelf();
}
