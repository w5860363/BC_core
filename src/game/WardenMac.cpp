/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "Auth/WardenKeyGeneration.h"
#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "ByteBuffer.h"
#include <openssl/md5.h>
#include "World.h"
#include "Player.h"
#include "Util.h"
#include "WardenMac.h"
#include "WardenModuleMac.h"
#include "AccountMgr.h"

WardenMac::WardenMac()
{
}

WardenMac::~WardenMac()
{
}

void WardenMac::Init(WorldSession *pClient, BigNumber *K)
{
    Client = pClient;
    // Generate Warden Key
    SHA1Randx WK(K->AsByteArray(), K->GetNumBytes());
    WK.generate(InputKey, 16);
    WK.generate(OutputKey, 16);
    /*
    Seed: 4D808D2C77D905C41A6380EC08586AFE (0x05 packet)
    Hash: <?> (0x04 packet)
    Module MD5: 0DBBF209A27B1E279A9FEC5C168A15F7
    New Client Key: <?>
    New Server Key: <?>
    */
    uint8 mod_seed[16] = { 0x4D, 0x80, 0x8D, 0x2C, 0x77, 0xD9, 0x05, 0xC4, 0x1A, 0x63, 0x80, 0xEC, 0x08, 0x58, 0x6A, 0xFE };

    memcpy(Seed, mod_seed, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);
    sLog.outDebug("WARDEN: Server side warden for client %u initializing...", pClient->GetAccountId());
    sLog.outDebug("WARDEN:   C->S Key: %s", ByteArrayToHexStr(InputKey, 16).c_str());
    sLog.outDebug("WARDEN:   S->C Key: %s", ByteArrayToHexStr(OutputKey, 16).c_str());
    sLog.outDebug("WARDEN:   Seed: %s", ByteArrayToHexStr(Seed, 16).c_str());
    sLog.outDebug("WARDEN: Loading Module...");

    Module = GetModuleForClient(Client);

    sLog.outDebug("WARDEN:   Module Key: %s", ByteArrayToHexStr(Module->Key, 16).c_str());
    sLog.outDebug("WARDEN:   Module ID: %s", ByteArrayToHexStr(Module->ID, 16).c_str());
    RequestModule();
}

ClientWardenModule *WardenMac::GetModuleForClient(WorldSession *session)
{
    ClientWardenModule *mod = new ClientWardenModule;

    uint32 len = sizeof(Module_0DBBF209A27B1E279A9FEC5C168A15F7_Data);

    // data assign
    mod->CompressedSize = len;
    mod->CompressedData = new uint8[len];
    memcpy(mod->CompressedData, Module_0DBBF209A27B1E279A9FEC5C168A15F7_Data, len);
    memcpy(mod->Key, Module_0DBBF209A27B1E279A9FEC5C168A15F7_Key, 16);
        
    // md5 hash
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, mod->CompressedData, len);
    MD5_Final((uint8*)&mod->ID, &ctx);

    return mod;
}

void WardenMac::InitializeModule()
{
    sLog.outDebug("WARDEN: Initialize module");
}

void WardenMac::RequestHash()
{
    sLog.outDebug("WARDEN: Request hash");

    // Create packet structure
    WardenHashRequest Request;
    Request.Command = WARDEN_SMSG_HASH_REQUEST;
    memcpy(Request.Seed, Seed, 16);

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenHashRequest));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenHashRequest));
    pkt.append((uint8*)&Request, sizeof(WardenHashRequest));
    Client->SendPacket(&pkt);
}

void WardenMac::HandleHashResult(ByteBuffer &buff)
{

    // test
    int keyIn[4];

    uint8 mod_seed[16] = { 0x4D, 0x80, 0x8D, 0x2C, 0x77, 0xD9, 0x05, 0xC4, 0x1A, 0x63, 0x80, 0xEC, 0x08, 0x58, 0x6A, 0xFE };

    for (int i = 0; i < 4; ++i)
    {
        keyIn[i] = *(int*)(&mod_seed[0] + i * 4);
    }

    int keyOut[4];
    int keyIn1, keyIn2;
    keyOut[0] = keyIn[0];
    keyIn[0] ^= 0xDEADBEEFu;
    keyIn1 = keyIn[1];
    keyIn[1] -= 0x35014542u;
    keyIn2 = keyIn[2];
    keyIn[2] += 0x5313F22u;
    keyIn[3] *= 0x1337F00Du;
    keyOut[1] = keyIn1 - 0x6A028A84;
    keyOut[2] = keyIn2 + 0xA627E44;
    keyOut[3] = 0x1337F00D * keyIn[3];
    // end test

    buff.rpos(buff.wpos());

    Sha1Hash sha1;
    sha1.UpdateData((uint8*)keyIn, 16);
    sha1.Finalize();

    //const uint8 validHash[20] = { 0x56, 0x8C, 0x05, 0x4C, 0x78, 0x1A, 0x97, 0x2A, 0x60, 0x37, 0xA2, 0x29, 0x0C, 0x22, 0xB5, 0x25, 0x71, 0xA0, 0x6F, 0x4E };

    // verify key not equal kick player
    if (memcmp(buff.contents() + 1, sha1.GetDigest(), 20) != 0)
    {
        sLog.outWarden("Request hash reply: failed");
        Client->KickPlayer();
        return;
    }

    sLog.outDebug("WARDEN: Request hash reply: succeed");

    // client 7F96EEFDA5B63D20A4DF8E00CBF48304
    //const uint8 client_key[16] = { 0x7F, 0x96, 0xEE, 0xFD, 0xA5, 0xB6, 0x3D, 0x20, 0xA4, 0xDF, 0x8E, 0x00, 0xCB, 0xF4, 0x83, 0x04 };

    // server C2B7ADEDFCCCA9C2BFB3F85602BA809B
    //const uint8 server_key[16] = { 0xC2, 0xB7, 0xAD, 0xED, 0xFC, 0xCC, 0xA9, 0xC2, 0xBF, 0xB3, 0xF8, 0x56, 0x02, 0xBA, 0x80, 0x9B };

    // change keys here
    memcpy(InputKey, keyIn, 16);
    memcpy(OutputKey, keyOut, 16);

    iCrypto.Init(InputKey);
    oCrypto.Init(OutputKey);

    m_initialized = true;

    m_WardenTimer = getMSTime();
}

void WardenMac::RequestData()
{
    sLog.outDebug("WARDEN: Request data");

    ByteBuffer buff;
    buff << uint8(WARDEN_SMSG_CHEAT_CHECKS_REQUEST);

    std::string str = "Test string!";

    buff << uint8(str.size());
    buff.append(str.c_str(), str.size());

    buff.hexlike();

    // Encrypt with warden RC4 key.
    EncryptData(const_cast<uint8*>(buff.contents()), buff.size());

    WorldPacket pkt(SMSG_WARDEN_DATA, buff.size());
    pkt.append(buff);
    Client->SendPacket(&pkt);

    m_WardenDataSent = true;
}

void WardenMac::HandleData(ByteBuffer &buff)
{
    sLog.outDebug("WARDEN: Handle data");

    m_WardenDataSent = false;
    m_WardenKickTimer = 0;

    //uint16 Length;
    //buff >> Length;
    //uint32 Checksum;
    //buff >> Checksum;

    //if (!IsValidCheckSum(Checksum, buff.contents() + buff.rpos(), Length))
    //{
    //    buff.rpos(buff.wpos());
    //    if (sWorld.getConfig(CONFIG_BOOL_WARDEN_KICK))
    //        Client->KickPlayer();
    //    return;
    //}

    bool found = false;

    std::string str = "Test string!";

    Sha1Hash sha1;
    sha1.UpdateData(str);
    uint32 magic = 0xFEEDFACE;                              // unsure
    sha1.UpdateData((uint8*)&magic, 4);
    sha1.Finalize();

    uint8 sha1Hash[20];
    buff.read(sha1Hash, 20);

    if (memcmp(sha1Hash, sha1.GetDigest(), 20))
    {
        sLog.outWarden("Handle data failed: SHA1 hash is wrong!");
        found = true;
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str.c_str(), str.size());
    uint8 ourMD5Hash[16];
    MD5_Final(ourMD5Hash, &ctx);

    uint8 theirsMD5Hash[16];
    buff.read(theirsMD5Hash, 16);

    if (memcmp(ourMD5Hash, theirsMD5Hash, 16))
    {
        sLog.outWarden("Handle data failed: MD5 hash is wrong!");
        found = true;
    }

    if (found)
		{
        std::string accountName;
        AccountMgr::GetName(Client->GetAccountId(), accountName);
        sWorld.SendGMText(LANG_GM_WARDEN_ANNOUNCE, accountName.c_str());
        switch (sWorld.getConfig(CONFIG_WARDEN_ACTION))
            {
            case 1:
                Client->KickPlayer();
                sWorld.SendGMText(LANG_GM_WARDEN_KICK_ANNOUNCE, accountName.c_str());
                break;
            case 2:
                std::stringstream duration;
                duration << sWorld.getConfig(CONFIG_WARDEN_BAN_DURATION) << "s";
                sWorld.BanAccount(BAN_ACCOUNT, accountName, duration.str(), "Cheat","WAC");
                sWorld.SendGMText(LANG_GM_WARDEN_BAN_ANNOUNCE, accountName.c_str());
                break;
            }
        }
}