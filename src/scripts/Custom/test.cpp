/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Test
Complete(%): 100
Comment: Script used for testing escortAI
Category: Script Examples
EndScriptData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

#define SAY_WALK        "Hmm a nice day for a walk alright"
#define SAY_ATTACK      "Wild Felboar attack!"
#define SAY_TIME_TO_GO  "Time for me to go! See ya around $N!"
#define SAY_BYE         "Bye Bye!"
#define SAY_AGGRO1      "Help $N! I'm under attack!"
#define SAY_AGGRO2      "Die scum!"
#define WHISPER_TOO_FAR "How dare you leave me like that! I hate you! =*("
#define SAY_DIE1        "...no...how could you let me die $N"
#define SAY_DIE2        "ugh..."
#define SAY_DEATHCOIL   "Taste death!"
#define SAY_FIREWORKS   "Fireworks!"
#define SAY_BUFF        "Hmm, I think I could use a buff"

#define GOSSIP_TEXT1    "Click to Test Escort(Attack, Defend, Run)"
#define GOSSIP_TEXT2    "Click to Test Escort(NoAttack, NoDefend, Walk)"
#define GOSSIP_TEXT3    "Click to Test Escort(NoAttack, Defend, Walk)"

struct npc_testAI : public npc_escortAI
{
    public:

        // CreatureAI functions
        npc_testAI(Creature* c) : npc_escortAI(c) {}

        uint32 DeathCoilTimer;
        uint32 ChatTimer;

        // Pure Virtual Functions
        void WaypointReached(uint32 i)
        {
            switch (i)
            {
                case 1:
                    me->Say(SAY_WALK, LANG_UNIVERSAL, 0);
                    break;

                case 3:
                {
                    me->Say(SAY_ATTACK, LANG_UNIVERSAL, 0);
                    Creature* temp = me->SummonCreature(21878, me->GetPositionX()+5, me->GetPositionY()+7, me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                    if (temp)
                        temp->AI()->AttackStart(me);
                }
                break;

                case 4:
                {
                    me->Say(SAY_TIME_TO_GO, LANG_UNIVERSAL, GetPlayerForEscort()->GetGUID());
                    me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);

                    Unit* temp = Unit::GetUnit(*me, GetPlayerForEscort()->GetGUID());
                    if (temp)
                    {
                        temp->MonsterSay(SAY_BYE, LANG_UNIVERSAL, 0);
                        temp->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                    }
                }
                break;
            }
        }

        void Aggro(Unit*)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                me->Say(SAY_AGGRO1, LANG_UNIVERSAL, GetPlayerForEscort()->GetGUID());
            else me->Say(SAY_AGGRO2, LANG_UNIVERSAL, 0);
        }

        void Reset()
        {
            DeathCoilTimer = 4000;
            ChatTimer = 4000;
        }

        void JustDied(Unit* killer)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
            {
                //killer = me when player got to far from creature
                if (killer == me)
                {
                    Unit* pTemp = Unit::GetUnit(*me, GetPlayerForEscort()->GetGUID());
                    if (pTemp)
                        me->MonsterWhisper(WHISPER_TOO_FAR, pTemp->GetGUID());
                }
                else me->Say(SAY_DIE1, LANG_UNIVERSAL, GetPlayerForEscort()->GetGUID());
            }
            else me->Say(SAY_DIE2, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            //Must update npc_escortAI
            npc_escortAI::UpdateAI(diff);

            //Combat check
            if (me->isInCombat() && me->getVictim())
            {
                if (DeathCoilTimer <= diff)
                {
                    me->Say(SAY_DEATHCOIL, LANG_UNIVERSAL, 0);
                    me->CastSpell(me->getVictim(), 33130, false);

                    DeathCoilTimer = 4000;
                } else DeathCoilTimer -= diff;
            } else
            {
                //Out of combat but being escorted
                if (HasEscortState(STATE_ESCORT_ESCORTING))
                {
                    if (ChatTimer <= diff)
                    {
                        if (me->HasAura(3593, 0))
                        {
                            me->Say(SAY_FIREWORKS, LANG_UNIVERSAL, 0);
                            me->CastSpell(me, 11540, false);
                        } else
                        {
                            me->Say(SAY_BUFF, LANG_UNIVERSAL, 0);
                            me->CastSpell(me, 3593, false);
                        }

                        ChatTimer = 12000;
                    } else ChatTimer -= diff;
                }
            }
        }
};

CreatureAI* GetAI_test(Creature* pCreature)
{
    npc_testAI* testAI = new npc_testAI(pCreature);

    testAI->AddWaypoint(0, 1231, -4419, 23);
    testAI->AddWaypoint(1, 1198, -4440, 23, 0);
    testAI->AddWaypoint(2, 1208, -4392, 23);
    testAI->AddWaypoint(3, 1231, -4419, 23, 5000);
    testAI->AddWaypoint(4, 1208, -4392, 23, 5000);

    return (CreatureAI*)testAI;
}

bool GossipHello_npc_test(Player* pPlayer, Creature* pCreature)
{
    pPlayer->TalkedToCreature(pCreature->GetEntry(), pCreature->GetGUID());
    pPlayer->PrepareGossipMenu(pCreature,0);

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

    pPlayer->SendPreparedGossip(pCreature);
    return true;
}

bool GossipSelect_npc_test(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_escortAI*)(pCreature->AI()))->Start(true, true, pPlayer->GetGUID());

        return true;                                        // prevent BlizzLikeCore handling
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_escortAI*)(pCreature->AI()))->Start(false, false, pPlayer->GetGUID());

        return true;                                        // prevent BlizzLikeCore handling
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+3)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_escortAI*)(pCreature->AI()))->Start(false, false, pPlayer->GetGUID());

        return true;                                        // prevent BlizzLikeCore handling
    }
    return false;
}

void AddSC_test()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "test";
    newscript->GetAI = &GetAI_test;
    newscript->pGossipHello          = &GossipHello_npc_test;
    newscript->pGossipSelect         = &GossipSelect_npc_test;
    newscript->RegisterSelf();
}

