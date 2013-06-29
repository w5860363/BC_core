/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include <cstdlib>
#include "ObjectLifeTime.h"

namespace BlizzLike
{
    extern "C" void external_wrapper(void *p)
    {
        std::atexit( (void (*)())p );
    }

    void at_exit( void (*func)() )
    {
        external_wrapper((void*)func);
    }
}

