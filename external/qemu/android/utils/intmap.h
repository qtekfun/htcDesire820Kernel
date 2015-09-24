/* Copyright (C) 2011 The Android Open Source Project
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
#ifndef _ANDROID_UTILS_INTMAP_H
#define _ANDROID_UTILS_INTMAP_H


typedef struct AIntMap  AIntMap;

AIntMap*  aintMap_new(void);

int       aintmap_getCount( AIntMap* map );

int       aintmap_has( AIntMap*  map, int key );

void*     aintMap_get( AIntMap*  map, int  key );

void*     aintMap_getWithDefault( AIntMap*  map, int key, void*  def );

void*     aintMap_set( AIntMap* map, int key, void* value );

void*     aintMap_del( AIntMap* map, int key );

void      aintMap_free( AIntMap*  map );

typedef struct AIntMapIterator {
    int    key;
    void*  value;
    void*  magic[4];
} AIntMapIterator;

void aintMapIterator_init( AIntMapIterator* iter, AIntMap* map );

int  aintMapIterator_next( AIntMapIterator* iter );

void aintMapIterator_done( AIntMapIterator* iter );

#define AINTMAP_FOREACH_KEY(map, keyvarname, stmnt) \
    do { \
        AIntMapIterator  __aintmap_foreach_iter[1]; \
        aintMapIterator_init(__aintmap_foreach_iter, (map)); \
        while (aintMapIterator_next(__aintmap_foreach_iter)) { \
            int keyvarname = __aintmap_foreach_iter->key; \
            stmnt; \
        } \
        aintMapIterator_done(__aintmap_foreach_iter); \
    } while (0)

#define AINTMAP_FOREACH_VALUE(map, valvarname, stmnt) \
    do { \
        AIntMapIterator  __aintmap_foreach_iter[1]; \
        aintMapIterator_init(__aintmap_foreach_iter, (map)); \
        while (aintMapIterator_next(__aintmap_foreach_iter)) { \
            void* valvarname = __aintmap_foreach_iter->value; \
            stmnt; \
        } \
        aintMapIterator_done(__aintmap_foreach_iter); \
    } while (0)

#endif 
