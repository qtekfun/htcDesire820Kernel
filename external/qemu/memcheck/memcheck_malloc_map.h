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


#ifndef QEMU_MEMCHECK_MEMCHECK_MALLOC_MAP_H
#define QEMU_MEMCHECK_MEMCHECK_MALLOC_MAP_H

#include "sys-tree.h"
#include "memcheck_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AllocMap {
    
    struct AllocMapEntry*   rbh_root;
} AllocMap;


void allocmap_init(AllocMap* map);

RBTMapResult allocmap_insert(AllocMap* map,
                             const MallocDescEx* desc,
                             MallocDescEx* replaced);

MallocDescEx* allocmap_find(const AllocMap* map,
                            target_ulong address,
                            uint32_t block_size);

int allocmap_pull(AllocMap* map, target_ulong address, MallocDescEx* pulled);

int allocmap_pull_first(AllocMap* map, MallocDescEx* pulled);

int allocmap_copy(AllocMap* to,
                  const AllocMap* from,
                  uint32_t set_flags,
                  uint32_t clear_flags);

int allocmap_empty(AllocMap* map);

#ifdef __cplusplus
};  
#endif

#endif  
