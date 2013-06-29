/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

/* ScriptData
Name: Deadmines
Complete(%): 0
Comment: Placeholder
Category: Deadmines
EndScriptData */

#include "ScriptPCH.h"
#include "deadmines.h"
#include "Spell.h"

/*#####
# item_Defias_Gunpowder
#####*/

bool ItemUse_item_defias_gunpowder(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    ScriptedInstance *pInstance = pPlayer->GetInstanceData();

    if (!pInstance)
    {
        pPlayer->GetSession()->SendNotification("Instance script not initialized");
        return true;
    }
    if (pInstance->GetData(EVENT_CANNON) != CANNON_NOT_USED)
        return false;
    if (targets.getGOTarget() && targets.getGOTarget()->GetTypeId() == TYPEID_GAMEOBJECT && targets.getGOTarget()->GetEntry() == GO_DEFIAS_CANNON)
        pInstance->SetData(EVENT_CANNON, CANNON_GUNPOWDER_USED);

    pPlayer->DestroyItemCount(pItem->GetEntry(), 1, true);
    return true;
}

void AddSC_deadmines()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "item_defias_gunpowder";
    newscript->pItemUse = &ItemUse_item_defias_gunpowder;
    newscript->RegisterSelf();
}
