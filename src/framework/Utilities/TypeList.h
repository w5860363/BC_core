/*
 * This file is part of the BlizzLikeCore Project.
 * See CREDITS.md and LICENSE.md files for Copyright information.
 */

#ifndef BLIZZLIKE_TYPELIST_H
#define BLIZZLIKE_TYPELIST_H

/*
  TypeList is the most simple but yet the most powerfull class of all.  It holds
  at compile time the different type of objects in a linked list.
 */

class TypeNull;

template<typename HEAD, typename TAIL>
struct TypeList
{
    typedef HEAD Head;
    typedef TAIL Tail;
};

// enough for now.. can be expand at any point in time as needed
#define TYPELIST_1(T1)                  TypeList<T1,TypeNull>
#define TYPELIST_2(T1, T2)              TypeList<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3)          TypeList<T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4)      TypeList<T1, TYPELIST_3(T2, T3, T4) >
#define TYPELIST_5(T1, T2, T3, T4, T5)  TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >
#endif

