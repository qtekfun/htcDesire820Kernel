/* Copyright (C) 2007-2010 The Android Open Source Project
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


#ifndef QEMU_MEMCHECK_MEMCHECK_MMRANGE_MAP_H
#define QEMU_MEMCHECK_MEMCHECK_MMRANGE_MAP_H

#include "sys-tree.h"
#include "memcheck_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MMRangeMap {
    
    struct MMRangeMapEntry* rbh_root;
} MMRangeMap;


void mmrangemap_init(MMRangeMap* map);

RBTMapResult mmrangemap_insert(MMRangeMap* map,
                               const MMRangeDesc* desc,
                               MMRangeDesc* replaced);

MMRangeDesc* mmrangemap_find(const MMRangeMap* map,
                             target_ulong start,
                             target_ulong end);

int mmrangemap_pull(MMRangeMap* map,
                    target_ulong start,
                    target_ulong end,
                    MMRangeDesc* pulled);

int mmrangemap_copy(MMRangeMap* to, const MMRangeMap* from);

int mmrangemap_empty(MMRangeMap* map);

#ifdef __cplusplus
};  
#endif

#endif  
