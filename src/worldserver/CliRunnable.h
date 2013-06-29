/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef __CLIRUNNABLE_H
#define __CLIRUNNABLE_H

// Command Line Interface handling thread
class CliRunnable : public ACE_Based::Runnable
{
    public:
        void run();
};
#endif

