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


#ifndef QEMU_MEMCHECK_MEMCHECK_PROC_MANAGEMENT_H
#define QEMU_MEMCHECK_MEMCHECK_PROC_MANAGEMENT_H

#include "qemu-queue.h"
#include "memcheck_common.h"
#include "memcheck_malloc_map.h"
#include "memcheck_mmrange_map.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ProcDesc {
    
    AllocMap                                    alloc_map;

    
    MMRangeMap                                  mmrange_map;

    
    QLIST_ENTRY(ProcDesc)                        global_entry;

    
    QLIST_HEAD(threads, ThreadDesc)              threads;

    
    char*                                       image_path;

    
    uint32_t                                    pid;

    
    uint32_t                                    parent_pid;

    
    uint32_t                                    flags;
} ProcDesc;

#define PROC_FLAG_EXECUTING             0x00000001
#define PROC_FLAG_EXITING               0x00000002
#define PROC_FLAG_IMAGE_PATH_REPLACED   0x00000004
#define PROC_FLAG_LIBC_INITIALIZED      0x00000008

typedef struct ThreadCallStackEntry {
    
    target_ulong    call_address;
    target_ulong    call_address_rel;
    
    target_ulong    ret_address;
    target_ulong    ret_address_rel;
    
    char*           module_path;
} ThreadCallStackEntry;

typedef struct ThreadDesc {
    
    QLIST_ENTRY(ThreadDesc)  global_entry;

    
    QLIST_ENTRY(ThreadDesc)  proc_entry;

    
    ProcDesc*               process;

    
    ThreadCallStackEntry*   call_stack;

    
    uint32_t                call_stack_count;

    
    uint32_t                call_stack_max;

    
    uint32_t                tid;
} ThreadDesc;


static inline int
procdesc_is_forked(const ProcDesc* proc)
{
    return proc->parent_pid != 0;
}

static inline int
procdesc_is_executing(const ProcDesc* proc)
{
    return (proc->flags & PROC_FLAG_EXECUTING) != 0;
}

static inline int
procdesc_is_exiting(const ProcDesc* proc)
{
    return (proc->flags & PROC_FLAG_EXITING) != 0;
}

static inline int
procdesc_is_libc_initialized(const ProcDesc* proc)
{
    return (proc->flags & PROC_FLAG_LIBC_INITIALIZED) != 0;
}

static inline int
procdesc_is_image_path_replaced(const ProcDesc* proc)
{
    return (proc->flags & PROC_FLAG_IMAGE_PATH_REPLACED) != 0;
}


ThreadDesc* get_current_thread(void);

void memcheck_init_proc_management(void);

ProcDesc* get_current_process(void);

ProcDesc* get_process_from_pid(uint32_t pid);

static inline RBTMapResult
procdesc_add_malloc(ProcDesc* proc,
                    const MallocDescEx* desc,
                    MallocDescEx* replaced)
{
    return allocmap_insert(&proc->alloc_map, desc, replaced);
}

static inline MallocDescEx*
procdesc_find_malloc_for_range(ProcDesc* proc,
                               target_ulong address,
                               uint32_t block_size)
{
    return allocmap_find(&proc->alloc_map, address, block_size);
}

static inline MallocDescEx*
procdesc_find_malloc(ProcDesc* proc, target_ulong address)
{
    return procdesc_find_malloc_for_range(proc, address, 1);
}

static inline int
procdesc_pull_malloc(ProcDesc* proc, target_ulong address, MallocDescEx* pulled)
{
    return allocmap_pull(&proc->alloc_map, address, pulled);
}

static inline int
procdesc_empty_alloc_map(ProcDesc* proc)
{
    return allocmap_empty(&proc->alloc_map);
}

static inline MMRangeDesc*
procdesc_find_mapentry(const ProcDesc* proc, target_ulong addr)
{
    return mmrangemap_find(&proc->mmrange_map, addr, addr + 1);
}

static inline const MMRangeDesc*
procdesc_get_range_desc(const ProcDesc* proc, target_ulong addr)
{
    return procdesc_find_mapentry(proc, addr);
}

static inline const char*
procdesc_get_module_path(const ProcDesc* proc, target_ulong addr)
{
    MMRangeDesc* rdesc = procdesc_find_mapentry(proc, addr);
    return rdesc != NULL ? rdesc->path : NULL;
}

#ifdef __cplusplus
};  
#endif

#endif  
