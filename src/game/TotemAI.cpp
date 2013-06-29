/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "TotemAI.h"
#include "Totem.h"
#include "Creature.h"
#include "Player.h"
#include "DBCStores.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

int
TotemAI::Permissible(const Creature* creature)
{
    if (creature->isTotem())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

TotemAI::TotemAI(Creature* c) : CreatureAI(c), i_totem(static_cast<Totem&>(*c)), i_victimGuid(0)
{
}

void
TotemAI::MoveInLineOfSight(Unit*)
{
}

void TotemAI::EnterEvadeMode()
{
    i_totem.CombatStop();
}

void
TotemAI::UpdateAI(const uint32 /*diff*/)
{
    if (i_totem.GetTotemType() != TOTEM_ACTIVE)
        return;

    if (!i_totem.isAlive() || i_totem.IsNonMeleeSpellCasted(false))
        return;

    // Search spell
    SpellEntry const *spellInfo = sSpellStore.LookupEntry(i_totem.GetSpell());
    if (!spellInfo)
        return;

    // Get spell rangy
    SpellRangeEntry const* srange = sSpellRangeStore.LookupEntry(spellInfo->rangeIndex);
    float max_range = GetSpellMaxRange(srange);

    // SPELLMOD_RANGE not applied in this place just because not existence range mods for attacking totems

    // pointer to appropriate target if found any
    Unit* victim = i_victimGuid ? ObjectAccessor::GetUnit(i_totem, i_victimGuid) : NULL;

    // Search victim if no, not attackable, or out of range, or friendly (possible in case duel end)
    if (!victim ||
        !victim->isTargetableForAttack() || !i_totem.IsWithinDistInMap(victim, max_range) ||
        i_totem.IsFriendlyTo(victim) || !victim->isVisibleForOrDetect(&i_totem,false))
    {
        CellPair p(BlizzLike::ComputeCellPair(i_totem.GetPositionX(),i_totem.GetPositionY()));
        Cell cell(p);
        cell.data.Part.reserved = ALL_DISTRICT;

        victim = NULL;

        BlizzLike::NearestAttackableUnitInObjectRangeCheck u_check(&i_totem, &i_totem, max_range);
        BlizzLike::UnitLastSearcher<BlizzLike::NearestAttackableUnitInObjectRangeCheck> checker(victim, u_check);

        TypeContainerVisitor<BlizzLike::UnitLastSearcher<BlizzLike::NearestAttackableUnitInObjectRangeCheck>, GridTypeMapContainer > grid_object_checker(checker);
        TypeContainerVisitor<BlizzLike::UnitLastSearcher<BlizzLike::NearestAttackableUnitInObjectRangeCheck>, WorldTypeMapContainer > world_object_checker(checker);

        //TODO: Backport BLizzLike Add to CreatureAI field pointing to creature itself
        //cell.Visit(p, grid_object_checker,  *m_creature.GetMap(), *m_creature, max_range);
        //cell.Visit(p, world_object_checker, *m_creature.GetMap(), *m_creature, max_range);
        cell.Visit(p, grid_object_checker,  *i_totem.GetMap());
        cell.Visit(p, world_object_checker, *i_totem.GetMap());
    }

    // If have target
    if (victim)
    {
        // remember
        i_victimGuid = victim->GetGUID();

        // attack
        i_totem.SetInFront(victim);                         // client change orientation by self
        i_totem.CastSpell(victim, i_totem.GetSpell(), false);
    }
    else
        i_victimGuid = 0;
}

bool
TotemAI::IsVisible(Unit*) const
{
    return false;
}

void
TotemAI::AttackStart(Unit*)
{
    // Sentry totem sends ping on attack
    if (i_totem.GetEntry() == SENTRY_TOTEM_ENTRY && i_totem.GetOwner()->GetTypeId() == TYPEID_PLAYER)
    {
        WorldPacket data(MSG_MINIMAP_PING, (8+4+4));
        data << i_totem.GetGUID();
        data << i_totem.GetPositionX();
        data << i_totem.GetPositionY();
        i_totem.GetOwner()->ToPlayer()->GetSession()->SendPacket(&data);
    }
}

