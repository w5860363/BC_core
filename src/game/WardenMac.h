/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _WARDEN_MAC_H
#define _WARDEN_MAC_H

#include "Auth/SARC4.h"
#include <map>
#include "Auth/BigNumber.h"
#include "ByteBuffer.h"

class WorldSession;
class WardenBase;

class WardenMac : WardenBase
{
    public:
        WardenMac();
        ~WardenMac();

        void Init(WorldSession *pClient, BigNumber *K);
        ClientWardenModule *GetModuleForClient(WorldSession *session);
        void InitializeModule();
        void RequestHash();
        void HandleHashResult(ByteBuffer &buff);
        void RequestData();
        void HandleData(ByteBuffer &buff);
};

#endif
