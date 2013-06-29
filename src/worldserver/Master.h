/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _MASTER_H
#define _MASTER_H

#include "Common.h"
#include "Policies/Singleton.h"

// Start the server
class Master
{
    public:
        Master();
        ~Master();
        int Run();
        static volatile uint32 m_masterLoopCounter;

    private:
        bool _StartDB();

        void _HookSignals();
        void _UnhookSignals();
        static void _OnSignal(int s);

        void clearOnlineAccounts();
};

#define sMaster BlizzLike::Singleton<Master>::Instance()
#endif

