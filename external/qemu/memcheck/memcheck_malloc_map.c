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


#include "memcheck_malloc_map.h"
#include "memcheck_util.h"
#include "memcheck_logging.h"

extern int memcheck_instrument_mmu;

typedef struct AllocMapEntry {
    
    RB_ENTRY(AllocMapEntry) rb_entry;

    
    MallocDescEx            desc;
} AllocMapEntry;


static inline target_ulong
allocmapentry_alloc_begins(const AllocMapEntry* adesc)
{
    return adesc->desc.malloc_desc.ptr;
}

static inline target_ulong
allocmapentry_alloc_ends(const AllocMapEntry* adesc)
{
    return mallocdesc_get_alloc_end(&adesc->desc.malloc_desc);
}


static inline int
cmp_rb(AllocMapEntry* d1, AllocMapEntry* d2)
{
    const target_ulong start1 = allocmapentry_alloc_begins(d1);
    const target_ulong start2 = allocmapentry_alloc_begins(d2);

    if (start1 < start2) {
        return (allocmapentry_alloc_ends(d1) - 1) < start2 ? -1 : 0;
    }
    return (allocmapentry_alloc_ends(d2) - 1) < start1 ? 1 : 0;
}

RB_GENERATE(AllocMap, AllocMapEntry, rb_entry, cmp_rb);


static RBTMapResult
allocmap_insert_desc(AllocMap* map,
                     AllocMapEntry* adesc,
                     MallocDescEx* replaced)
{
    AllocMapEntry* existing = AllocMap_RB_INSERT(map, adesc);
    if (existing == NULL) {
        return RBT_MAP_RESULT_ENTRY_INSERTED;
    }

    
    if (replaced == NULL) {
        return RBT_MAP_RESULT_ENTRY_ALREADY_EXISTS;
    }

    memcpy(replaced, &existing->desc, sizeof(MallocDescEx));
    AllocMap_RB_REMOVE(map, existing);
    qemu_free(existing);
    AllocMap_RB_INSERT(map, adesc);
    return RBT_MAP_RESULT_ENTRY_REPLACED;
}

static inline AllocMapEntry*
allocmap_find_entry(const AllocMap* map,
                    target_ulong address,
                    uint32_t block_size)
{
    AllocMapEntry adesc;
    adesc.desc.malloc_desc.ptr = address;
    adesc.desc.malloc_desc.requested_bytes = block_size;
    adesc.desc.malloc_desc.prefix_size = 0;
    adesc.desc.malloc_desc.suffix_size = 0;
    return AllocMap_RB_FIND((AllocMap*)map, &adesc);
}


void
allocmap_init(AllocMap* map)
{
    RB_INIT(map);
}

RBTMapResult
allocmap_insert(AllocMap* map, const MallocDescEx* desc, MallocDescEx* replaced)
{
    RBTMapResult ret;

    
    AllocMapEntry* adesc = qemu_malloc(sizeof(AllocMapEntry));
    if (adesc == NULL) {
        ME("memcheck: Unable to allocate new AllocMapEntry on insert.");
        return RBT_MAP_RESULT_ERROR;
    }
    memcpy(&adesc->desc, desc, sizeof(MallocDescEx));

    
    ret = allocmap_insert_desc(map, adesc, replaced);
    if (ret == RBT_MAP_RESULT_ENTRY_ALREADY_EXISTS ||
        ret == RBT_MAP_RESULT_ERROR) {
        qemu_free(adesc);
    }
    return ret;
}

MallocDescEx*
allocmap_find(const AllocMap* map, target_ulong address, uint32_t block_size)
{
    AllocMapEntry* adesc = allocmap_find_entry(map, address, block_size);
    return adesc != NULL ? &adesc->desc : NULL;
}

int
allocmap_pull(AllocMap* map, target_ulong address, MallocDescEx* pulled)
{
    AllocMapEntry* adesc = allocmap_find_entry(map, address, 1);
    if (adesc != NULL) {
        memcpy(pulled, &adesc->desc, sizeof(MallocDescEx));
        AllocMap_RB_REMOVE(map, adesc);
        qemu_free(adesc);
        return 0;
    } else {
        return -1;
    }
}

int
allocmap_pull_first(AllocMap* map, MallocDescEx* pulled)
{
    AllocMapEntry* first = RB_MIN(AllocMap, map);
    if (first != NULL) {
        memcpy(pulled, &first->desc, sizeof(MallocDescEx));
        AllocMap_RB_REMOVE(map, first);
        qemu_free(first);
        return 0;
    } else {
        return -1;
    }
}

int
allocmap_copy(AllocMap* to,
              const AllocMap* from,
              uint32_t set_flags,
              uint32_t clear_flags)
{
    AllocMapEntry* entry;
    RB_FOREACH(entry, AllocMap, (AllocMap*)from) {
        RBTMapResult ins_res;
        AllocMapEntry* new_entry =
                (AllocMapEntry*)qemu_malloc(sizeof(AllocMapEntry));
        if (new_entry == NULL) {
            ME("memcheck: Unable to allocate new AllocMapEntry on copy.");
            return -1;
        }
        memcpy(new_entry, entry, sizeof(AllocMapEntry));
        new_entry->desc.flags &= ~clear_flags;
        new_entry->desc.flags |= set_flags;
        if (entry->desc.call_stack_count) {
            new_entry->desc.call_stack =
               qemu_malloc(entry->desc.call_stack_count * sizeof(target_ulong));
            memcpy(new_entry->desc.call_stack, entry->desc.call_stack,
                   entry->desc.call_stack_count * sizeof(target_ulong));
        } else {
            new_entry->desc.call_stack = NULL;
        }
        new_entry->desc.call_stack_count = entry->desc.call_stack_count;
        ins_res = allocmap_insert_desc(to, new_entry, NULL);
        if (ins_res == RBT_MAP_RESULT_ENTRY_INSERTED) {
            if (memcheck_instrument_mmu) {
                
                invalidate_tlb_cache(new_entry->desc.malloc_desc.ptr,
                        mallocdesc_get_alloc_end(&new_entry->desc.malloc_desc));
            }
        } else {
            ME("memcheck: Unable to insert new map entry on copy. Insert returned %u",
               ins_res);
            if (new_entry->desc.call_stack != NULL) {
                qemu_free(new_entry->desc.call_stack);
            }
            qemu_free(new_entry);
            return -1;
        }
    }

    return 0;
}

int
allocmap_empty(AllocMap* map)
{
    MallocDescEx pulled;
    int removed = 0;

    while (!allocmap_pull_first(map, &pulled)) {
        removed++;
        if (pulled.call_stack != NULL) {
            qemu_free(pulled.call_stack);
        }
    }

    return removed;
}
