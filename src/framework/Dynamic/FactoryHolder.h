/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_FACTORY_HOLDER
#define BLIZZLIKE_FACTORY_HOLDER

#include "Platform/Define.h"
#include "Utilities/TypeList.h"
#include "ObjectRegistry.h"
#include "Policies/SingletonImp.h"

// FactoryHolder holds a factory object of a specific type
template<class T, class Key = std::string>
class FactoryHolder
{
    public:
        typedef ObjectRegistry<FactoryHolder<T, Key >, Key > FactoryHolderRegistry;
        typedef BlizzLike::Singleton<FactoryHolderRegistry > FactoryHolderRepository;

        FactoryHolder(Key k) : i_key(k) {}
        virtual ~FactoryHolder() {}
        inline Key key() const { return i_key; }

        void RegisterSelf(void) { FactoryHolderRepository::Instance().InsertItem(this, i_key); }
        void DeregisterSelf(void) { FactoryHolderRepository::Instance().RemoveItem(this, false); }

        // Abstract Factory create method
        virtual T* Create(void *data = NULL) const = 0;
    private:
        Key i_key;
};

/* Permissible is a classic way of letting the object decide
 * whether how good they handle things.  This is not retricted
 * to factory selectors.
 */
template<class T>
class Permissible
{
    public:
        virtual ~Permissible() {}
        virtual int Permit(const T *) const = 0;
};
#endif

