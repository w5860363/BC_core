/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Arathi Highlands
Complete(%): 100
Comment: Quest support: 665
Category: Arathi Highlands
EndScriptData */

/* ContentData
npc_professor_phizzlethorpe
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

/*######
## npc_professor_phizzlethorpe
######*/

enum eEnums
{
    SAY_PROGRESS_1      = -1000264,
    SAY_PROGRESS_2      = -1000265,
    SAY_PROGRESS_3      = -1000266,
    EMOTE_PROGRESS_4    = -1000267,
    SAY_AGGRO           = -1000268,
    SAY_PROGRESS_5      = -1000269,
    SAY_PROGRESS_6      = -1000270,
    SAY_PROGRESS_7      = -1000271,
    EMOTE_PROGRESS_8    = -1000272,
    SAY_PROGRESS_9      = -1000273,

    QUEST_SUNKEN_TREASURE   = 665,
    MOB_VENGEFUL_SURGE      = 2776
};

struct npc_professor_phizzlethorpeAI : public npc_escortAI
{
    npc_professor_phizzlethorpeAI(Creature* c) : npc_escortAI(c) {}

    void WaypointReached(uint32 uiPointId)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch (uiPointId)
        {
        case 4:DoScriptText(SAY_PROGRESS_2, me, pPlayer);break;
        case 5:DoScriptText(SAY_PROGRESS_3, me, pPlayer);break;
        case 8:DoScriptText(EMOTE_PROGRESS_4, me);break;
        case 9:
            {
            me->SummonCreature(MOB_VENGEFUL_SURGE, -2052.96f, -2142.49f, 20.15f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
            me->SummonCreature(MOB_VENGEFUL_SURGE, -2052.96f, -2142.49f, 20.15f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
            break;
            }
        case 10:DoScriptText(SAY_PROGRESS_5, me, pPlayer);break;
        case 11:
            DoScriptText(SAY_PROGRESS_6, me, pPlayer);
            SetRun();
            break;
        case 19:DoScriptText(SAY_PROGRESS_7, me, pPlayer); break;
        case 20:
            DoScriptText(EMOTE_PROGRESS_8, me);
            DoScriptText(SAY_PROGRESS_9, me, pPlayer);
            if (pPlayer)
                CAST_PLR(pPlayer)->GroupEventHappens(QUEST_SUNKEN_TREASURE, me);
            break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(me);
    }

    void EnterCombat(Unit* /*pWho*/)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

bool QuestAccept_npc_professor_phizzlethorpe(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_SUNKEN_TREASURE)
    {
        DoScriptText(SAY_PROGRESS_1, pCreature, pPlayer);
        if (npc_escortAI* pEscortAI = CAST_AI(npc_professor_phizzlethorpeAI, (pCreature->AI())))
            pEscortAI->Start(false, false, pPlayer->GetGUID(), pQuest);

        pCreature->setFaction(113);
    }
    return true;
}

CreatureAI* GetAI_npc_professor_phizzlethorpeAI(Creature* pCreature)
{
    return new npc_professor_phizzlethorpeAI(pCreature);
}

void AddSC_arathi_highlands()
{
    Script * newscript;

    newscript = new Script;
    newscript->Name = "npc_professor_phizzlethorpe";
    newscript->GetAI = &GetAI_npc_professor_phizzlethorpeAI;
    newscript->pQuestAccept = &QuestAccept_npc_professor_phizzlethorpe;
    newscript->RegisterSelf();
}
