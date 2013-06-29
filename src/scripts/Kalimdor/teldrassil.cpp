/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Teldrassil
Complete(%): 100
Comment: Quest support: 938
Category: Teldrassil
EndScriptData */

/* ContentData
npc_mist
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedFollowerAI.h"

/*####
# npc_mist
####*/

enum eMist
{
    SAY_AT_HOME             = -1000323,
    EMOTE_AT_HOME           = -1000324,
    QUEST_MIST              = 938,
    NPC_ARYNIA              = 3519,

    POINT_ID_TO_FOREST      = 1,
    FACTION_DARNASSUS       = 79
};

const float m_afToForestLoc[] = {10648.7f, 1790.63f, 1324.08f};

struct npc_mistAI : public FollowerAI
{
    npc_mistAI(Creature* pCreature) : FollowerAI(pCreature) { }

    uint32 m_uiPostEventTimer;
    uint32 m_uiPhasePostEvent;

    uint64 AryniaGUID;

    void Reset()
    {
        m_uiPostEventTimer = 1000;
        m_uiPhasePostEvent = 0;

        AryniaGUID = 0;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        FollowerAI::MoveInLineOfSight(pWho);

        if (!me->getVictim() && !HasFollowState(STATE_FOLLOW_COMPLETE | STATE_FOLLOW_POSTEVENT) && pWho->GetEntry() == NPC_ARYNIA)
        {
            if (me->IsWithinDistInMap(pWho, INTERACTION_DISTANCE))
            {
                if (Player* pPlayer = GetLeaderForFollower())
                {
                    if (pPlayer->GetQuestStatus(QUEST_MIST) == QUEST_STATUS_INCOMPLETE)
                        pPlayer->GroupEventHappens(QUEST_MIST, me);
                }

                AryniaGUID = pWho->GetGUID();
                SetFollowComplete(true);
            }
        }
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        FollowerAI::MovementInform(uiMotionType, uiPointId);

        if (uiMotionType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_ID_TO_FOREST)
            SetFollowComplete();
    }

    void UpdateFollowerAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
        {
            if (HasFollowState(STATE_FOLLOW_POSTEVENT))
            {
                if (m_uiPostEventTimer <= uiDiff)
                {
                    m_uiPostEventTimer = 3000;

                    Unit* pArynia = Unit::GetUnit(*me, AryniaGUID);
                    if (!pArynia || !pArynia->isAlive())
                    {
                        SetFollowComplete();
                        return;
                    }

                    switch (m_uiPhasePostEvent)
                    {
                    case 0:
                        DoScriptText(SAY_AT_HOME, pArynia);
                        break;
                    case 1:
                        DoScriptText(EMOTE_AT_HOME, me);
                        me->GetMotionMaster()->MovePoint(POINT_ID_TO_FOREST, m_afToForestLoc[0], m_afToForestLoc[1], m_afToForestLoc[2]);
                        break;
                    }

                    ++m_uiPhasePostEvent;
                }
                else
                    m_uiPostEventTimer -= uiDiff;
            }

            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_mist(Creature* pCreature)
{
    return new npc_mistAI(pCreature);
}

bool QuestAccept_npc_mist(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_MIST)
    {
        if (npc_mistAI* pMistAI = CAST_AI(npc_mistAI, pCreature->AI()))
            pMistAI->StartFollow(pPlayer, FACTION_DARNASSUS, pQuest);
    }

    return true;
}

void AddSC_teldrassil()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_mist";
    newscript->GetAI = &GetAI_npc_mist;
    newscript->pQuestAccept = &QuestAccept_npc_mist;
    newscript->RegisterSelf();
}
