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


#ifndef QEMU_MEMCHECK_MEMCHECK_COMMON_H
#define QEMU_MEMCHECK_MEMCHECK_COMMON_H

#include "qemu-common.h"
#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif


#define TRACE_DEV_REG_LIBC_INIT             1536

#define TRACE_DEV_REG_MALLOC                1537

#define TRACE_DEV_REG_FREE_PTR              1538

#define TRACE_DEV_REG_QUERY_MALLOC          1539

#define TRACE_DEV_REG_PRINT_USER_STR        1540


typedef struct MallocDesc {
    target_ulong    ptr;

    
    uint32_t        requested_bytes;

    uint32_t        prefix_size;

    
    uint32_t        suffix_size;

    uint32_t        libc_pid;

    uint32_t        allocator_pid;

    
    uint32_t        av_count;
} MallocDesc;
#define ALLOC_RES_OFFSET        ((uint32_t)(ptrdiff_t)&(((MallocDesc*)0)->libc_pid))
#define ALLOC_RES_ADDRESS(p)    (p + ALLOC_RES_OFFSET)

typedef struct MallocDescQuery {
    target_ulong    ptr;

    uint32_t        libc_pid;

    
    uint32_t        query_pid;

    uint32_t        routine;

    target_ulong    desc;
} MallocDescQuery;
#define QUERY_RES_OFFSET        ((uint32_t)(ptrdiff_t)&(((MallocDescQuery*)0)->libc_pid))
#define QUERY_RES_ADDRESS(p)    (p + QUERY_RES_OFFSET)

typedef struct MallocFree {
    
    uint32_t    ptr;

    uint32_t    libc_pid;

    
    uint32_t    free_pid;
} MallocFree;
#define FREE_RES_OFFSET         ((uint32_t)(ptrdiff_t)&(((MallocFree*)0)->libc_pid))
#define FREE_RES_ADDRESS(p)     (p + FREE_RES_OFFSET)

typedef struct MallocDescEx {
    
    MallocDesc      malloc_desc;

    target_ulong*   call_stack;

    
    uint32_t        call_stack_count;

    
    uint32_t        flags;
} MallocDescEx;

#define MDESC_FLAG_TRANSITION_ENTRY         0x00000001

#define MDESC_FLAG_INHERITED_ON_FORK        0x00000002

typedef struct MMRangeDesc {
    
    target_ulong            map_start;

    
    target_ulong            map_end;

    
    target_ulong            exec_offset;

    
    char*                   path;
} MMRangeDesc;

typedef enum {
    
    RBT_MAP_RESULT_ENTRY_INSERTED = 0,

    
    RBT_MAP_RESULT_ENTRY_ALREADY_EXISTS,

    RBT_MAP_RESULT_ENTRY_REPLACED,

    
    RBT_MAP_RESULT_ERROR = -1,
} RBTMapResult;

typedef struct AddrArray {
    
    target_ulong*   addr;

    
    int             num;
} AddrArray;


static inline target_ulong
mallocdesc_get_user_ptr(const MallocDesc* desc)
{
    return desc->ptr + desc->prefix_size;
}

static inline uint32_t
mallocdesc_get_alloc_size(const MallocDesc* desc)
{
    return desc->prefix_size + desc->requested_bytes + desc->suffix_size;
}

static inline target_ulong
mallocdesc_get_alloc_end(const MallocDesc* desc)
{
    return desc->ptr + mallocdesc_get_alloc_size(desc);
}

static inline target_ulong
mallocdesc_get_user_alloc_end(const MallocDesc* desc)
{
    return mallocdesc_get_user_ptr(desc) + desc->requested_bytes;
}

static inline int
mallocdescex_is_transition_entry(const MallocDescEx* desc)
{
    return (desc->flags & MDESC_FLAG_TRANSITION_ENTRY) != 0;
}

static inline int
mallocdescex_is_inherited_on_fork(const MallocDescEx* desc)
{
    return (desc->flags & MDESC_FLAG_INHERITED_ON_FORK) != 0;
}

static inline target_ulong
mmrangedesc_get_module_offset(const MMRangeDesc* rdesc, target_ulong address)
{
    return address - rdesc->map_start + rdesc->exec_offset;
}

static inline int
addrarray_check(const AddrArray* addr_array, target_ulong addr)
{
    if (addr_array->num != 0) {
        int m_min = 0;
        int m_max = addr_array->num - 1;

        
        addr &= ~1;
        
        while (m_min <= m_max) {
            const int m = (m_min + m_max) >> 1;
            const target_ulong saved = addr_array->addr[m];
            if (addr == saved) {
                return 1;
            }
            if (addr < saved) {
                m_max = m - 1;
            } else {
                m_min = m + 1;
            }
        }
    }
    return 0;
}

static inline int
addrarray_add(AddrArray* addr_array, target_ulong addr)
{
    target_ulong* new_arr;
    int m_min;
    int m_max;

    
    addr &= ~1;
    if (addr_array->num == 0) {
        
        addr_array->addr = qemu_malloc(sizeof(target_ulong));
        assert(addr_array->addr != NULL);
        if (addr_array->addr == NULL) {
            return 0;
        }
        *addr_array->addr = addr;
        addr_array->num++;
        return 1;
    }

    
    m_min = 0;
    m_max = addr_array->num - 1;
    while (m_min <= m_max) {
        const int m = (m_min + m_max) >> 1;
        const target_ulong saved = addr_array->addr[m];
        if (addr == saved) {
            return -1;
        }
        if (addr < saved) {
            m_max = m - 1;
        } else {
            m_min = m + 1;
        }
    }
    if (m_max < 0) {
        m_max = 0;
    }
    
    new_arr = qemu_malloc(sizeof(target_ulong) * (addr_array->num + 1));
    assert(new_arr != NULL);
    if (new_arr == NULL) {
        return 0;
    }
    
    if (m_max != 0) {
        memcpy(new_arr, addr_array->addr, m_max * sizeof(target_ulong));
    }
    if (addr > addr_array->addr[m_max]) {
        new_arr[m_max] = addr_array->addr[m_max];
        m_max++;
    }
    
    new_arr[m_max] = addr;
    
    if (m_max < addr_array->num) {
        memcpy(new_arr + m_max + 1, addr_array->addr + m_max,
               (addr_array->num - m_max) * sizeof(target_ulong));
    }
    
    qemu_free(addr_array->addr);
    addr_array->addr = new_arr;
    addr_array->num++;
    return 1;
}

#ifdef __cplusplus
};  
#endif

#endif  
