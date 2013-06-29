/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_THREADINGMODEL_H
#define BLIZZLIKE_THREADINGMODEL_H

#include "Platform/Define.h"

namespace BlizzLike
{
    inline void Guard(void *) {}

    template<typename MUTEX> class GeneralLock
    {
        public:
            GeneralLock(MUTEX &m) : i_mutex(m)
            {
                i_mutex.acquire();
            }

            ~GeneralLock()
            {
                i_mutex.release();
            }
        private:
            GeneralLock(const GeneralLock &);
            GeneralLock& operator=(const GeneralLock &);
            MUTEX &i_mutex;
    };

    template <class T>
        class SingleThreaded
    {
        public:

            struct Lock                                     // empty object
            {
                Lock() {}
                Lock(const T &) {}
                Lock(const SingleThreaded<T> &)             // for single threaded we ignore this
                {
                }
            };

            typedef T VolatileType;
    };

    // object level lockable
    template<class T, class MUTEX>
        class ObjectLevelLockable
    {
        public:
            ObjectLevelLockable() : i_mtx() {}

            friend class Lock;

            class Lock
            {
                public:
                    Lock(ObjectLevelLockable<T, MUTEX> &host) : i_lock(host.i_mtx)
                    {
                    }

                private:
                    GeneralLock<MUTEX> i_lock;
            };

            typedef volatile T VolatileType;

        private:
            // prevent the compiler creating a copy construct
            ObjectLevelLockable(const ObjectLevelLockable<T, MUTEX> &);
            ObjectLevelLockable<T, MUTEX>& operator=(const ObjectLevelLockable<T, MUTEX> &);

            MUTEX i_mtx;
    };

    template<class T, class MUTEX>
        class ClassLevelLockable
    {
        public:
            class Lock;
            friend class Lock;
            typedef volatile T VolatileType;

            ClassLevelLockable() {}

            class Lock
            {
                public:
                    Lock(T& /*host*/) { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    Lock(ClassLevelLockable<T, MUTEX> &) { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    Lock() { ClassLevelLockable<T, MUTEX>::si_mtx.acquire(); }
                    ~Lock() { ClassLevelLockable<T, MUTEX>::si_mtx.release(); }
            };

        private:
            static MUTEX si_mtx;
    };

}

template<class T, class MUTEX> MUTEX BlizzLike::ClassLevelLockable<T, MUTEX>::si_mtx;

#define INSTANTIATE_CLASS_MUTEX(CTYPE,MUTEX) \
    template class BlizzLike::ClassLevelLockable<CTYPE, MUTEX >
#endif

