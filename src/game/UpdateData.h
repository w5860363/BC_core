/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef __UPDATEDATA_H
#define __UPDATEDATA_H

#include "ByteBuffer.h"
class WorldPacket;

enum ObjectUpdateType
{
    UPDATETYPE_VALUES               = 0,
    UPDATETYPE_MOVEMENT             = 1,
    UPDATETYPE_CREATE_OBJECT        = 2,
    UPDATETYPE_CREATE_OBJECT2       = 3,
    UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4,
    UPDATETYPE_NEAR_OBJECTS         = 5
};

enum ObjectUpdateFlags
{
    UPDATEFLAG_NONE                 = 0x0000,
    UPDATEFLAG_SELF                 = 0x0001,
    UPDATEFLAG_TRANSPORT            = 0x0002,
    UPDATEFLAG_HAS_ATTACKING_TARGET = 0x0004,
    UPDATEFLAG_LOWGUID              = 0x0008,
    UPDATEFLAG_HIGHGUID             = 0x0010,
    UPDATEFLAG_LIVING               = 0x0020,
    UPDATEFLAG_HAS_POSITION         = 0x0040,
};

class UpdateData
{
    public:
        UpdateData();

        void AddOutOfRangeGUID(std::set<uint64>& guids);
        void AddOutOfRangeGUID(const uint64 &guid);
        void AddUpdateBlock(const ByteBuffer &block);
        bool BuildPacket(WorldPacket* packet, bool hasTransport = false);
        bool HasData() { return m_blockCount > 0 || !m_outOfRangeGUIDs.empty(); }
        void Clear();

        std::set<uint64> const& GetOutOfRangeGUIDs() const { return m_outOfRangeGUIDs; }

    protected:
        uint32 m_blockCount;
        std::set<uint64> m_outOfRangeGUIDs;
        ByteBuffer m_data;

        void Compress(void* dst, uint32 *dst_size, void* src, int src_size);
};
#endif

