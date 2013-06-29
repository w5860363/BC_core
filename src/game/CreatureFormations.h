/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _FORMATIONS_H
#define _FORMATIONS_H

#include "Common.h"
#include <ace/Basic_Types.h>
#include <ace/Singleton.h>
#include <ace/Thread_Mutex.h>

class CreatureFormation;

struct FormationInfo
{
    uint32 leaderGUID;
    uint8 formationAI;
};

struct FormationData
{
    uint32 formationId;
    float follow_dist;
    float follow_angle;
};

struct Formation
{
    uint32 leaderGUID;
    float follow_dist;
    float follow_angle;
    uint8 formationAI;
};


class CreatureFormationManager
{
    friend class ACE_Singleton<CreatureFormationManager, ACE_Null_Mutex>;
    public:
        void AddCreatureToFormation(uint32 formation_id, Creature* creature);
        void RemoveCreatureFromFormation(CreatureFormation *formation, Creature* creature);
        void LoadCreatureFormations();
};

#ifdef formation_mgr
#undef formation_mgr
#endif

#define formation_mgr (*ACE_Singleton<CreatureFormationManager, ACE_Null_Mutex>::instance())

typedef UNORDERED_MAP<uint32/*formationId*/, FormationInfo*>   CreatureFormationInfoType;
typedef UNORDERED_MAP<uint32 /*memberGUID*/, FormationData*>   CreatureFormationDataType;

extern CreatureFormationInfoType    CreatureFormationMap;
extern CreatureFormationDataType    CreatureFormationDataMap;

class CreatureFormation
{
    private:
        Creature* m_leader;
        typedef std::map<Creature*, Formation*>  CreatureFormationMemberType;
        CreatureFormationMemberType m_members;

        uint32 m_formationID;
        bool m_Formed;

    public:
        //Fromation cannot be created empty
        explicit CreatureFormation(uint32 id) : m_leader(NULL), m_formationID(id), m_Formed(false) {}
        ~CreatureFormation() { sLog.outDebug("Destroying formation"); }

        Creature* getLeader() const { return m_leader; }
        uint32 GetId() const { return m_formationID; }
        bool isEmpty() const { return m_members.empty(); }
        bool isFormed() const { return m_Formed; }

        void AddMember(Creature* member);
        void RemoveMember(Creature* member);
        void Reset(bool dismiss);

        void LeaderMoveTo(float x, float y, float z);
        void MemberAttackStart(Creature* member, Unit* target);
};

#endif