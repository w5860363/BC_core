/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef _AUTH_SARC4_H
#define _AUTH_SARC4_H

#include "Common.h"
#include <openssl/evp.h>

class SARC4
{
    public:
        SARC4(uint8 len);
        SARC4(uint8 *seed, uint8 len);
        ~SARC4();
        void Init(uint8 *seed);
        void UpdateData(int len, uint8 *data);
    private:
        EVP_CIPHER_CTX m_ctx;
};

#endif
