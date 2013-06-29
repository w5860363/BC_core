/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleVoiceSettingsOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_VOICE_SETTINGS");
    // uint8 isVoiceEnabled, uint8 isMicrophoneEnabled
    recv_data.read_skip<uint8>();
    recv_data.read_skip<uint8>();
    recv_data.hexlike();
}

void WorldSession::HandleChannelEnableVoiceOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_CHANNEL_ENABLE_VOICE");
    // Enable Voice button in channel context menu
    recv_data.hexlike();
}

void WorldSession::HandleChannelVoiceChatQuery(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_CHANNEL_VOICE_CHAT_QUERY");
    recv_data.read_skip<uint32>();
    recv_data.read_skip<char*>();
    recv_data.hexlike();
}

