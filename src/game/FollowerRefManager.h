/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _FOLLOWERREFMANAGER
#define _FOLLOWERREFMANAGER

#include "Utilities/LinkedReference/RefManager.h"

class Unit;
class TargetedMovementGeneratorBase;

class FollowerRefManager : public RefManager<Unit, TargetedMovementGeneratorBase>
{

};
#endif

