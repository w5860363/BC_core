/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _ACCMGR_H
#define _ACCMGR_H

#include <string>

#include "Common.h"
#include "ace/Singleton.h"

enum AccountOpResult
{
    AOR_OK,
    AOR_NAME_TOO_LONG,
    AOR_PASS_TOO_LONG,
    AOR_NAME_ALREDY_EXIST,
    AOR_NAME_NOT_EXIST,
    AOR_DB_INTERNAL_ERROR
};

#define MAX_ACCOUNT_STR 16

class AccountMgr
{
    public:
        AccountMgr();
        ~AccountMgr();

        AccountOpResult CreateAccount(std::string username, std::string password);
        AccountOpResult DeleteAccount(uint32 accid);
        AccountOpResult ChangeUsername(uint32 accid, std::string new_uname, std::string new_passwd);
        AccountOpResult ChangePassword(uint32 accid, std::string new_passwd);
        bool CheckPassword(uint32 accid, std::string passwd);

        uint32 GetId(std::string username);
        uint32 GetSecurity(uint32 acc_id);
        uint32 GetSecurity(uint32 acc_id, int32 realm_id);
        static bool GetName(uint32 acc_id, std::string &name);
        uint32 GetCharactersCount(uint32 acc_id);
        static std::string CalculateShaPassHash(std::string& name, std::string& password);

        static bool normalizeString(std::string& utf8str);
};

#define sAccountMgr ACE_Singleton<AccountMgr, ACE_Null_Mutex>::instance()
#endif

