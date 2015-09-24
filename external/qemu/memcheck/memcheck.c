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


#include "qemu-queue.h"
#include "qemu_file.h"
#include "elff_api.h"
#include "memcheck.h"
#include "memcheck_proc_management.h"
#include "memcheck_util.h"
#include "memcheck_logging.h"


uint32_t trace_flags = 0;

int memcheck_enabled = 0;

int memcheck_instrument_mmu = 0;

int memcheck_watch_call_stack = 1;


static void
av_invalid_pointer(ProcDesc* proc, target_ulong ptr, int routine)
{
    if (trace_flags & TRACE_CHECK_INVALID_PTR_ENABLED) {
        printf("memcheck: Access violation is detected in process %s[pid=%u]:\n"
          "  INVALID POINTER 0x%08X is used in '%s' operation.\n"
          "  Allocation descriptor for this pointer has not been found in the\n"
          "  allocation map for the process. Most likely, this is an attempt\n"
          "  to %s a pointer that has been freed.\n",
          proc->image_path, proc->pid, ptr, routine == 1 ? "free" : "realloc",
          routine == 1 ? "free" : "reallocate");
    }
}

/* Prints read / write access violation information.
 * Param:
 *  proc - Process that caused access violation.
 *  desc - Allocation descriptor for the violation.
 *  addr - Address at which vilation has occurred.
 *  data_size - Size of data accessed at the 'addr'.
 *  val - If access violation has occurred at write operation, this parameter
 *      contains value that's being written to 'addr'. For read violation this
 *      parameter is not used.
 *  retaddr - Code address (in TB) where access violation has occurred.
 *  is_read - If 1, access violation has occurred when memory at 'addr' has been
 *      read. If 0, access violation has occurred when memory was written.
 */
static void
av_access_violation(ProcDesc* proc,
                    MallocDescEx* desc,
                    target_ulong addr,
                    uint32_t data_size,
                    uint64_t val,
                    target_ulong retaddr,
                    int is_read)
{
    target_ulong vaddr;
    Elf_AddressInfo elff_info;
    ELFF_HANDLE elff_handle = NULL;

    desc->malloc_desc.av_count++;
    if ((is_read && !(trace_flags & TRACE_CHECK_READ_VIOLATION_ENABLED)) ||
        (!is_read && !(trace_flags & TRACE_CHECK_WRITE_VIOLATION_ENABLED))) {
        return;
    }

    
    vaddr = memcheck_tpc_to_gpc(retaddr);
    printf("memcheck: Access violation is detected in process %s[pid=%u]:\n",
           proc->image_path, proc->pid);

    
    const MMRangeDesc* rdesc = procdesc_get_range_desc(proc, vaddr);
    if (rdesc != NULL) {
        int elff_res;
        printf("  In module %s at address 0x%08X\n", rdesc->path, vaddr);
        elff_res =
          memcheck_get_address_info(vaddr, rdesc, &elff_info, &elff_handle);
        if (elff_res == 0) {
            printf("  In routine %s in %s/%s:%u\n",
                   elff_info.routine_name, elff_info.dir_name,
                   elff_info.file_name, elff_info.line_number);
            if (elff_info.inline_stack != NULL) {
                const Elf_InlineInfo* inl = elff_info.inline_stack;
                int index = 0;
                for (; inl[index].routine_name != NULL; index++) {
                    char align[64];
                    size_t set_align = 4 + index * 2;
                    if (set_align >= sizeof(align)) {
                        set_align = sizeof(align) -1;
                    }
                    memset(align, ' ', set_align);
                    align[set_align] = '\0';
                    printf("%s", align);
                    if (inl[index].inlined_in_file == NULL) {
                        printf("inlined to %s in unknown location\n",
                               inl[index].routine_name);
                    } else {
                        printf("inlined to %s in %s/%s:%u\n",
                               inl[index].routine_name,
                               inl[index].inlined_in_file_dir,
                               inl[index].inlined_in_file,
                               inl[index].inlined_at_line);
                    }
                }
            }
            elff_free_pc_address_info(elff_handle, &elff_info);
            elff_close(elff_handle);
        } else if (elff_res == 1) {
            printf("  Unable to obtain routine information. Symbols file is not found.\n");
        } else {
            printf("  Unable to obtain routine information.\n"
                   "  Symbols file doesn't contain debugging information for address 0x%08X.\n",
                    mmrangedesc_get_module_offset(rdesc, vaddr));
        }
    } else {
        printf("  In unknown module at address 0x%08X\n", vaddr);
    }

    printf("  Process attempts to %s %u bytes %s address 0x%08X\n",
           is_read ? "read" : "write", data_size,
           is_read ? "from" : "to", addr);
    printf("  Accessed range belongs to the %s guarding area of allocated block.\n",
           addr < (target_ulong)mallocdesc_get_user_ptr(&desc->malloc_desc) ?
                "prefix" : "suffix");
    printf("  Allocation descriptor for this violation:\n");
    memcheck_dump_malloc_desc(desc, 1, 0);
}

static int
memcheck_common_access_validation(target_ulong addr,
                                  uint32_t data_size,
                                  ProcDesc** proc_ptr,
                                  MallocDescEx** desc_ptr)
{
    MallocDescEx* desc;
    target_ulong validating_range_end;
    target_ulong user_range_end;

    ProcDesc* proc = get_current_process();
    *proc_ptr = proc;
    if (proc == NULL) {
        *desc_ptr = NULL;
        return 1;
    }

    desc = procdesc_find_malloc_for_range(proc, addr, data_size);
    *desc_ptr = desc;
    if (desc == NULL) {
        return 1;
    }

    if (addr < mallocdesc_get_user_ptr(&desc->malloc_desc)) {
        
        return -1;
    }

    validating_range_end = addr + data_size;
    user_range_end = mallocdesc_get_user_alloc_end(&desc->malloc_desc);

    if (validating_range_end <= user_range_end) {
        
        return 0;
    }

    
    if ((validating_range_end - user_range_end) < data_size) {
        
        return 0;
    }

    return -1;
}

static inline int
procdesc_contains_allocs(ProcDesc* proc, target_ulong addr, uint32_t buf_size) {
    if (proc != NULL) {
        
        const target_ulong end_page = (addr + buf_size - 1) & TARGET_PAGE_MASK;
        
        addr &= TARGET_PAGE_MASK;
        
        buf_size = end_page - addr + TARGET_PAGE_SIZE + 1;
        return procdesc_find_malloc_for_range(proc, addr, buf_size) ? 1 : 0;
    } else {
        return 0;
    }
}


void
memcheck_init(const char* tracing_flags)
{
    if (*tracing_flags == '0') {
        
        return;
    } else if (*tracing_flags == '1') {
        
        trace_flags = TRACE_CHECK_LEAK_ENABLED             |
                      TRACE_CHECK_READ_VIOLATION_ENABLED   |
                      TRACE_CHECK_INVALID_PTR_ENABLED      |
                      TRACE_CHECK_WRITE_VIOLATION_ENABLED;
    }

    
    while (*tracing_flags) {
        switch (*tracing_flags) {
            case 'A':
                
                trace_flags |= TRACE_ALL_ENABLED;
                break;
            case 'F':
                
                trace_flags |= TRACE_PROC_FORK_ENABLED;
                break;
            case 'S':
                
                trace_flags |= TRACE_PROC_START_ENABLED;
                break;
            case 'E':
                
                trace_flags |= TRACE_PROC_EXIT_ENABLED;
                break;
            case 'C':
                
                trace_flags |= TRACE_PROC_CLONE_ENABLED;
                break;
            case 'N':
                
                trace_flags |= TRACE_PROC_NEW_PID_ENABLED;
                break;
            case 'B':
                
                trace_flags |= TRACE_PROC_LIBC_INIT_ENABLED;
                break;
            case 'L':
                
                trace_flags |= TRACE_CHECK_LEAK_ENABLED;
                break;
            case 'I':
                
                trace_flags |= TRACE_CHECK_INVALID_PTR_ENABLED;
                break;
            case 'R':
                
                trace_flags |= TRACE_CHECK_READ_VIOLATION_ENABLED;
                break;
            case 'W':
                
                trace_flags |= TRACE_CHECK_WRITE_VIOLATION_ENABLED;
                break;
            case 'M':
                
                trace_flags |= TRACE_PROC_MMAP_ENABLED;
                break;
            default:
                break;
        }
        if (trace_flags == TRACE_ALL_ENABLED) {
            break;
        }
        tracing_flags++;
    }

    if (trace_flags & (TRACE_CHECK_READ_VIOLATION_ENABLED |
                       TRACE_CHECK_WRITE_VIOLATION_ENABLED)) {
        memcheck_instrument_mmu = 1;
    } else {
        memcheck_instrument_mmu = 0;
    }

    memcheck_init_proc_management();

    
    if (getenv("ANDROID_PROJECT_OUT") == NULL) {
        printf("memcheck: Missing ANDROID_PROJECT_OUT environment variable, that is used\n"
               "to calculate path to symbol files.\n");
    }

    
    memcheck_enabled = 1;
}

void
memcheck_guest_libc_initialized(uint32_t pid)
{
    ProcDesc* proc = get_process_from_pid(pid);
    if (proc == NULL) {
        ME("memcheck: Unable to obtain process for libc_init pid=%u", pid);
        return;
    }
    proc->flags |= PROC_FLAG_LIBC_INITIALIZED;

    procdesc_empty_alloc_map(proc);
    T(PROC_LIBC_INIT, "memcheck: libc.so has been initialized for %s[pid=%u]\n",
      proc->image_path, proc->pid);
}

void
memcheck_guest_alloc(target_ulong guest_address)
{
    MallocDescEx desc;
    MallocDescEx replaced;
    RBTMapResult insert_res;
    ProcDesc* proc;
    ThreadDesc* thread;
    uint32_t indx;

    
    memcheck_get_malloc_descriptor(&desc.malloc_desc, guest_address);
    desc.flags = 0;
    desc.call_stack = NULL;
    desc.call_stack_count = 0;

    proc = get_process_from_pid(desc.malloc_desc.allocator_pid);
    if (proc == NULL) {
        ME("memcheck: Unable to obtain process for allocation pid=%u",
           desc.malloc_desc.allocator_pid);
        memcheck_fail_alloc(guest_address);
        return;
    }

    if (!procdesc_is_executing(proc)) {
        desc.flags |= MDESC_FLAG_TRANSITION_ENTRY;
    }

    
    thread = get_current_thread();
    desc.call_stack_count = thread->call_stack_count;
    if (desc.call_stack_count) {
        desc.call_stack = qemu_malloc(desc.call_stack_count * sizeof(target_ulong));
        if (desc.call_stack == NULL) {
            ME("memcheck: Unable to allocate %u bytes for the calling stack",
               desc.call_stack_count * sizeof(target_ulong));
            return;
        }
    }

    for (indx = 0; indx < thread->call_stack_count; indx++) {
        desc.call_stack[indx] =
           thread->call_stack[thread->call_stack_count - 1 - indx].call_address;
    }

    
    insert_res = procdesc_add_malloc(proc, &desc, &replaced);
    if (insert_res == RBT_MAP_RESULT_ENTRY_INSERTED) {
        
        if (memcheck_instrument_mmu) {
            invalidate_tlb_cache(desc.malloc_desc.ptr,
                                mallocdesc_get_alloc_end(&desc.malloc_desc));
        }
    } else if (insert_res == RBT_MAP_RESULT_ENTRY_REPLACED) {
        ME("memcheck: Duplicate allocation blocks:");
        if (VERBOSE_CHECK(memcheck)) {
            printf("   New block:\n");
            memcheck_dump_malloc_desc(&desc, 1, 1);
            printf("   Replaced block:\n");
            memcheck_dump_malloc_desc(&replaced, 1, 1);
        }
        if (replaced.call_stack != NULL) {
            qemu_free(replaced.call_stack);
        }
    } else {
        ME("memcheck: Unable to insert an entry to the allocation map:");
        if (VERBOSE_CHECK(memcheck)) {
            memcheck_dump_malloc_desc(&desc, 1, 1);
        }
        memcheck_fail_alloc(guest_address);
        return;
    }
}

void
memcheck_guest_free(target_ulong guest_address)
{
    MallocFree desc;
    MallocDescEx pulled;
    int pull_res;
    ProcDesc* proc;

    
    memcheck_get_free_descriptor(&desc, guest_address);

    proc = get_process_from_pid(desc.free_pid);
    if (proc == NULL) {
        ME("memcheck: Unable to obtain process for pid=%u on free",
           desc.free_pid);
        memcheck_fail_free(guest_address);
        return;
    }

    
    pull_res = procdesc_pull_malloc(proc, desc.ptr, &pulled);
    if (pull_res) {
        av_invalid_pointer(proc, desc.ptr, 1);
        memcheck_fail_free(guest_address);
        return;
    }

    
    if (desc.ptr != mallocdesc_get_user_ptr(&pulled.malloc_desc)) {
        if (trace_flags & TRACE_CHECK_INVALID_PTR_ENABLED) {
            printf("memcheck: Access violation is detected in process %s[pid=%u]:\n",
                   proc->image_path, proc->pid);
            printf("  INVALID POINTER 0x%08X is used in 'free' operation.\n"
                   "  This pointer is unexpected for 'free' operation, as allocation\n"
                   "  descriptor found for this pointer in the process' allocation map\n"
                   "  suggests that 0x%08X is the pointer to be used to free this block.\n"
                   "  Allocation descriptor matching the pointer:\n",
                   desc.ptr,
                   (uint32_t)mallocdesc_get_user_ptr(&pulled.malloc_desc));
            memcheck_dump_malloc_desc(&pulled, 1, 0);
        }
    }
    if (pulled.call_stack != NULL) {
        qemu_free(pulled.call_stack);
    }
}

void
memcheck_guest_query_malloc(target_ulong guest_address)
{
    MallocDescQuery qdesc;
    MallocDescEx* found;
    ProcDesc* proc;

    
    memcheck_get_query_descriptor(&qdesc, guest_address);

    proc = get_process_from_pid(qdesc.query_pid);
    if (proc == NULL) {
        ME("memcheck: Unable to obtain process for pid=%u on query_%s",
           qdesc.query_pid, qdesc.routine == 1 ? "free" : "realloc");
        memcheck_fail_query(guest_address);
        return;
    }

    
    found = procdesc_find_malloc(proc, qdesc.ptr);
    if (found == NULL) {
        av_invalid_pointer(proc, qdesc.ptr, qdesc.routine);
        memcheck_fail_query(guest_address);
        return;
    }

    
    memcheck_set_malloc_descriptor(qdesc.desc, &found->malloc_desc);
}

void
memcheck_guest_print_str(target_ulong str) {
    char str_copy[4096];
    memcheck_get_guest_string(str_copy, str, sizeof(str_copy));
    printf("%s", str_copy);
}

int
memcheck_validate_ld(target_ulong addr,
                     uint32_t data_size,
                     target_ulong retaddr)
{
    ProcDesc* proc;
    MallocDescEx* desc;

    int res = memcheck_common_access_validation(addr, data_size, &proc, &desc);
    if (res == -1) {
        av_access_violation(proc, desc, addr, data_size, 0, retaddr, 1);
        return 1;
    }

    return res ? procdesc_contains_allocs(proc, addr, data_size) : 0;
}

/* Validates write operations, detected in __stx_mmu routine.
 * This routine is called from __stx_mmu wrapper implemented in
 * softmmu_template.h on condition that storing is occurring from user memory.
 * Param:
 *  addr - Virtual address in the guest space where memory is written.
 *  data_size - Size of the write.
 *  value - Value to be written. Note that we typecast all values to 64 bits,
 *      since this will fit all data sizes.
 *  retaddr - Code address (in TB) that accesses memory.
 * Return:
 *  1 if TLB record for the accessed page should be invalidated in order to
 *  ensure that subsequent attempts to access data in this page will cause
 *  __ld/stx_mmu to be used. If memchecker is no longer interested in monitoring
 * access to this page, this routine returns 0.
 */
int
memcheck_validate_st(target_ulong addr,
                     uint32_t data_size,
                     uint64_t value,
                     target_ulong retaddr)
{
    MallocDescEx* desc;
    ProcDesc* proc;

    int res = memcheck_common_access_validation(addr, data_size, &proc, &desc);
    if (res == -1) {
        av_access_violation(proc, desc, addr, data_size, value, retaddr, 0);
        return 1;
    }

    return res ? procdesc_contains_allocs(proc, addr, data_size) : 0;
}

int
memcheck_is_checked(target_ulong addr, uint32_t size) {
    return procdesc_contains_allocs(get_current_process(), addr, size) ? 1 : 0;
}
