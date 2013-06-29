/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef VMAPEXPORT_H
#define VMAPEXPORT_H

#include "warnings.h"

enum ModelFlags
{
    MOD_M2 = 1,
    MOD_WORLDSPAWN = 1<<1,
    MOD_HAS_BOUND = 1<<2
};

extern const char * szWorkDirWmo;
extern const char * szRawVMAPMagic;                         // vmap magic string for extracted raw vmap data

#endif
