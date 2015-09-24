/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _ANDROID_GRAPHICS_REFLIST_H
#define _ANDROID_GRAPHICS_REFLIST_H

#include <inttypes.h>
#include <android/utils/system.h>


typedef struct ARefList {
    
    uint16_t   count, size, max;
    uint16_t   iteration;
    union {
        void*   item0;
        void**  items;
    } u;
} ARefList;

AINLINED void
areflist_init(ARefList*  l)
{
    l->count     = 0;
    l->size      = 0;
    l->max       = 1;
    l->iteration = 0;
}

AINLINED int
areflist_getCount(const ARefList*  l)
{
    return l->count;
}

void  areflist_setEmpty(ARefList*  l);

AINLINED void
areflist_done(ARefList*  l)
{
    areflist_setEmpty(l);
}

AINLINED ABool
areflist_isEmpty(const ARefList*  l)
{
    return (areflist_getCount(l) == 0);
}

int    areflist_indexOf(const ARefList*  l, void*  item);

AINLINED ABool
areflist_has(const ARefList*  l, void*  item)
{
    return areflist_indexOf(l, item) >= 0;
}

void    areflist_add(ARefList*  l, void*  item);

ABool   areflist_delFirst(ARefList*  l, void*  item);

ABool   areflist_delAll(ARefList*  l, void*  item);

AINLINED void
areflist_push(ARefList*  l, void*  item)
{
    areflist_add(l, item);
}

void*  areflist_popLast(ARefList*  l);

void*   areflist_get(const ARefList*  l, int  n);

AINLINED int
areflist_count(ARefList*  l)
{
    return l->count;
}

void  areflist_append(ARefList*  l, const ARefList*  src);

void    _areflist_remove_deferred(ARefList*  l);

void**  _areflist_at(const ARefList*  l, int  n);

#define  AREFLIST_LOOP(list_,itemvar_) \
    do { \
        ARefList*  _reflist_loop   = (list_); \
        int        _reflist_loop_i = 0; \
        int        _reflist_loop_n = _reflist_loop->size; \
        _reflist_loop->iteration += 2; \
        for ( ; _reflist_loop_i < _reflist_loop_n; _reflist_loop_i++ ) { \
            void** _reflist_loop_at = _areflist_at(_reflist_loop, _reflist_loop_i); \
            (itemvar_) = *(_reflist_loop_at); \
            if ((itemvar_) != NULL) {

#define  AREFLIST_LOOP_END \
            } \
        } \
        if (_reflist_loop->iteration & 1) \
            _areflist_remove_deferred(_reflist_loop); \
    } while (0);

#define  AREFLIST_LOOP_CONST(list_,itemvar_) \
    do { \
        const ARefList*  _reflist_loop   = (list_); \
        int              _reflist_loop_i = 0; \
        int              _reflist_loop_n = _reflist_loop->size; \
        for ( ; _reflist_loop_i < _reflist_loop_n; _reflist_loop_i++ ) { \
            void** _reflist_loop_at = _areflist_at(_reflist_loop, _reflist_loop_i); \
            (itemvar_) = *(_reflist_loop_at); \
            if ((itemvar_) != NULL) {

#define  AREFLIST_LOOP_DEL() \
    (_reflist_loop->iteration |= 1, *_reflist_loop_at = NULL)

#define  AREFLIST_LOOP_SET(val) \
    (_reflist_loop->iteration |= 1, *_reflist_loop_at = (val))


#define  AREFLIST_FOREACH(list_,item_,statement_) \
    ({ ARefList*  _reflist   = (list_); \
       int        _reflist_i = 0; \
       int        _reflist_n = _reflist->size; \
       _reflist->iteration += 2; \
       for ( ; _reflist_i < _reflist_n; _reflist_i++ ) { \
           void**  __reflist_at   = _areflist_at(_reflist, _reflist_i); \
           void*  item_ = *__reflist_at; \
           if (item_ != NULL) { \
               statement_; \
           } \
       } \
       _reflist->iteration -= 2; \
       if (_reflist->iteration == 1) \
           _areflist_remove_deferred(_reflist); \
    })

#define  AREFLIST_FOREACH_CONST(list_,item_,statement_) \
    ({ const ARefList*  _reflist = (list_); \
       int        _reflist_i = 0; \
       int        _reflist_n = _reflist->size; \
       for ( ; _reflist_i < _reflist_n; _reflist_i++ ) { \
           void**  __reflist_at = _areflist_at(_reflist, _reflist_i); \
           void*  item_ = *__reflist_at; \
           if (item_ != NULL) { \
               statement_; \
           } \
       } \
    })

#define  AREFLIST_DEL_ITERATED()  \
    ({ *__reflist_at = NULL; \
       _reflist->iteration |= 1; })

#define  AREFLIST_SET_ITERATED(item) \
    ({ *__reflist_at = (item); \
       if (item == NULL) _reflist->iteration |= 1; })

void  areflist_copy(ARefList*  dst, const ARefList*  src);

#endif 
