/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _VMAPDEFINITIONS_H
#define _VMAPDEFINITIONS_H
#include <cstring>

#define LIQUID_TILE_SIZE (533.333f / 128.f)

namespace VMAP
{
    const char VMAP_MAGIC[] = "VMAP_3.0";                   // used in final vmap files
    const char RAW_VMAP_MAGIC[] = "VMAPs03";                // used in extracted vmap files with raw data

    // defined in TileAssembler.cpp currently...
    bool readChunk(FILE *rf, char *dest, const char *compare, uint32 len);
}

#ifdef MMAP_GENERATOR
    #include <assert.h>
    #define ASSERT(x) assert(x)
    #define DEBUG_LOG(...) 0
    #define DETAIL_LOG(...) 0
    #define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while (0)
#elif !defined NO_CORE_FUNCS
    #include "Errors.h"
    #include "Log.h"
    #define ERROR_LOG(...) sLog.outError(__VA_ARGS__);
    #define DETAIL_LOG(...) sLog.outDetail(__VA_ARGS__);
#else
    #include <assert.h>
    #define ASSERT(x) assert(x)
    #define DEBUG_LOG(...) do{ printf(__VA_ARGS__); printf("\n"); } while (0)
    #define DETAIL_LOG(...) do{ printf(__VA_ARGS__); printf("\n"); } while (0)
    #define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while (0)
#endif

#endif // _VMAPDEFINITIONS_H

