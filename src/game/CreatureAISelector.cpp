/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "Creature.h"
#include "CreatureAISelector.h"
#include "PassiveAI.h"
#include "Policies/SingletonImp.h"
#include "MovementGenerator.h"
#include "Pet.h"
#include "TemporarySummon.h"
#include "CreatureAIFactory.h"
#include "ScriptMgr.h"

INSTANTIATE_SINGLETON_1(CreatureAIRegistry);
INSTANTIATE_SINGLETON_1(MovementGeneratorRegistry);

namespace FactorySelector
{
    CreatureAI* selectAI(Creature* creature)
    {
        const CreatureAICreator *ai_factory = NULL;
        CreatureAIRegistry &ai_registry(CreatureAIRepository::Instance());

        if (creature->isPet())
            ai_factory = ai_registry.GetRegistryItem("PetAI");

        //scriptname in db
        if (!ai_factory)
            if (CreatureAI* scriptedAI = sScriptMgr.GetAI(creature))
                return scriptedAI;

        // AIname in db
        std::string ainame=creature->GetAIName();
        if (!ai_factory && !ainame.empty())
            ai_factory = ai_registry.GetRegistryItem(ainame.c_str());

        // select by NPC flags
        if (!ai_factory)
        {
            if (creature->HasSummonMask(SUMMON_MASK_CONTROLABLE_GUARDIAN) && ((Guardian*)creature)->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                ai_factory = ai_registry.GetRegistryItem("PetAI");
            else if (creature->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK))
                ai_factory = ai_registry.GetRegistryItem("NullCreatureAI");
            else if (creature->isGuard())
                ai_factory = ai_registry.GetRegistryItem("GuardAI");
            else if (creature->HasSummonMask(SUMMON_MASK_CONTROLABLE_GUARDIAN))
                ai_factory = ai_registry.GetRegistryItem("PetAI");
            else if (creature->isTotem())
                ai_factory = ai_registry.GetRegistryItem("TotemAI");
            else if (creature->isTrigger())
            {
                if (creature->m_spells[0])
                    ai_factory = ai_registry.GetRegistryItem("TriggerAI");
                else
                    ai_factory = ai_registry.GetRegistryItem("NullCreatureAI");
            }
            else if (creature->GetCreatureType() == CREATURE_TYPE_CRITTER && !creature->HasSummonMask(SUMMON_MASK_GUARDIAN))
                ai_factory = ai_registry.GetRegistryItem("CritterAI");
        }

        // select by permit check
        if (!ai_factory)
        {
            int best_val = -1;
            typedef CreatureAIRegistry::RegistryMapType RMT;
            RMT const &l = ai_registry.GetRegisteredItems();
            for (RMT::const_iterator iter = l.begin(); iter != l.end(); ++iter)
            {
                const CreatureAICreator *factory = iter->second;
                const SelectableAI *p = dynamic_cast<const SelectableAI *>(factory);
                ASSERT(p != NULL);
                int val = p->Permit(creature);
                if (val > best_val)
                {
                    best_val = val;
                    ai_factory = p;
                }
            }
        }

        // select NullCreatureAI if not another cases
        ainame = (ai_factory == NULL) ? "NullCreatureAI" : ai_factory->key();

        DEBUG_LOG("Creature %u used AI is %s.", creature->GetGUIDLow(), ainame.c_str());
        return (ai_factory == NULL ? new NullCreatureAI(creature) : ai_factory->Create(creature));
    }

    MovementGenerator* selectMovementGenerator(Creature* creature)
    {
        MovementGeneratorRegistry &mv_registry(MovementGeneratorRepository::Instance());
        ASSERT(creature->GetCreatureInfo() != NULL);
        const MovementGeneratorCreator *mv_factory = mv_registry.GetRegistryItem(creature->GetDefaultMovementType());

        /* if (mv_factory == NULL)
        {
            int best_val = -1;
            std::vector<std::string> l;
            mv_registry.GetRegisteredItems(l);
            for (std::vector<std::string>::iterator iter = l.begin(); iter != l.end(); ++iter)
            {
            const MovementGeneratorCreator *factory = mv_registry.GetRegistryItem((*iter).c_str());
            const SelectableMovement *p = dynamic_cast<const SelectableMovement *>(factory);
            ASSERT(p != NULL);
            int val = p->Permit(creature);
            if (val > best_val)
            {
                best_val = val;
                mv_factory = p;
            }
            }
        }*/

        return (mv_factory == NULL ? NULL : mv_factory->Create(creature));

    }
}

