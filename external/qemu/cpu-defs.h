/*
 * common defines for all CPUs
 *
 * Copyright (c) 2003 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CPU_DEFS_H
#define CPU_DEFS_H

#ifndef NEED_CPU_H
#error cpu.h included from common code
#endif

#include "config.h"
#include <setjmp.h>
#include <inttypes.h>
#include <signal.h>
#include "osdep.h"
#include "qemu-queue.h"
#include "targphys.h"

#ifndef TARGET_LONG_BITS
#error TARGET_LONG_BITS must be defined before including this header
#endif

#define TARGET_LONG_SIZE (TARGET_LONG_BITS / 8)

#if TARGET_LONG_SIZE == 4
typedef int32_t target_long;
typedef uint32_t target_ulong;
#define TARGET_FMT_lx "%08x"
#define TARGET_FMT_ld "%d"
#define TARGET_FMT_lu "%u"
#elif TARGET_LONG_SIZE == 8
typedef int64_t target_long;
typedef uint64_t target_ulong;
#define TARGET_FMT_lx "%016" PRIx64
#define TARGET_FMT_ld "%" PRId64
#define TARGET_FMT_lu "%" PRIu64
#else
#error TARGET_LONG_SIZE undefined
#endif

#define HOST_LONG_SIZE (HOST_LONG_BITS / 8)

#define EXCP_INTERRUPT 	0x10000 
#define EXCP_HLT        0x10001 
#define EXCP_DEBUG      0x10002 
#define EXCP_HALTED     0x10003 

#define TB_JMP_CACHE_BITS 12
#define TB_JMP_CACHE_SIZE (1 << TB_JMP_CACHE_BITS)

#define TB_JMP_PAGE_BITS (TB_JMP_CACHE_BITS / 2)
#define TB_JMP_PAGE_SIZE (1 << TB_JMP_PAGE_BITS)
#define TB_JMP_ADDR_MASK (TB_JMP_PAGE_SIZE - 1)
#define TB_JMP_PAGE_MASK (TB_JMP_CACHE_SIZE - TB_JMP_PAGE_SIZE)

#if !defined(CONFIG_USER_ONLY)
#define CPU_TLB_BITS 8
#define CPU_TLB_SIZE (1 << CPU_TLB_BITS)

#if HOST_LONG_BITS == 32 && TARGET_LONG_BITS == 32
#define CPU_TLB_ENTRY_BITS 4
#else
#define CPU_TLB_ENTRY_BITS 5
#endif

typedef struct CPUTLBEntry {
    target_ulong addr_read;
    target_ulong addr_write;
    target_ulong addr_code;
    size_t addend;
    
    uint8_t dummy[(1 << CPU_TLB_ENTRY_BITS) -
                  (sizeof(target_ulong) * 3 +
                   ((-sizeof(target_ulong) * 3) & (sizeof(size_t) - 1)) +
                   sizeof(size_t))];
} CPUTLBEntry;

extern int CPUTLBEntry_wrong_size[sizeof(CPUTLBEntry) == (1 << CPU_TLB_ENTRY_BITS) ? 1 : -1];

#define CPU_COMMON_TLB \
       \
    CPUTLBEntry tlb_table[NB_MMU_MODES][CPU_TLB_SIZE];                  \
    target_phys_addr_t iotlb[NB_MMU_MODES][CPU_TLB_SIZE];               \
    target_ulong tlb_flush_addr;                                        \
    target_ulong tlb_flush_mask;

#else

#define CPU_COMMON_TLB

#endif


#ifdef HOST_WORDS_BIGENDIAN
typedef struct icount_decr_u16 {
    uint16_t high;
    uint16_t low;
} icount_decr_u16;
#else
typedef struct icount_decr_u16 {
    uint16_t low;
    uint16_t high;
} icount_decr_u16;
#endif

struct kvm_run;
struct KVMState;
struct qemu_work_item;

typedef struct CPUBreakpoint {
    target_ulong pc;
    int flags; 
    QTAILQ_ENTRY(CPUBreakpoint) entry;
} CPUBreakpoint;

typedef struct CPUWatchpoint {
    target_ulong vaddr;
    target_ulong len_mask;
    int flags; 
    QTAILQ_ENTRY(CPUWatchpoint) entry;
} CPUWatchpoint;

#define CPU_TEMP_BUF_NLONGS 128
#define CPU_COMMON                                                      \
    struct TranslationBlock *current_tb;   \
                                                  \
                                                      \
    unsigned long mem_io_pc; 
                             \
    target_ulong mem_io_vaddr; 
             \
    uint32_t halted;        \
    uint32_t interrupt_request;                                         \
    volatile sig_atomic_t exit_request;                                 \
    CPU_COMMON_TLB                                                      \
    struct TranslationBlock *tb_jmp_cache[TB_JMP_CACHE_SIZE];           \
                      \
    long temp_buf[CPU_TEMP_BUF_NLONGS];                                 \
                                                                        \
    int64_t icount_extra;    \
               \
    union {                                                             \
        uint32_t u32;                                                   \
        icount_decr_u16 u16;                                            \
    } icount_decr;                                                      \
    uint32_t can_do_io;      \
                                                                        \
                           \
                                                 \
    QTAILQ_HEAD(breakpoints_head, CPUBreakpoint) breakpoints;            \
    int singlestep_enabled;                                             \
                                                                        \
    QTAILQ_HEAD(watchpoints_head, CPUWatchpoint) watchpoints;            \
    CPUWatchpoint *watchpoint_hit;                                      \
                                                                        \
    struct GDBRegisterState *gdb_regs;                                  \
                                                                        \
                                               \
    jmp_buf jmp_env;                                                    \
    int exception_index;                                                \
                                                                        \
    CPUState *next_cpu;                  \
    int cpu_index;                         \
    uint32_t host_tid;                              \
    int numa_node;             \
    int nr_cores;          \
    int nr_threads;              \
    int running;   \
                                                         \
    void *opaque;                                                       \
                                                                        \
    uint32_t created;                                                   \
    uint32_t stop;                                    \
    uint32_t stopped;                         \
    struct QemuThread *thread;                                          \
    struct QemuCond *halt_cond;                                         \
    struct qemu_work_item *queued_work_first, *queued_work_last;        \
    const char *cpu_model_str;                                          \
    struct KVMState *kvm_state;                                         \
    struct kvm_run *kvm_run;                                            \
    int kvm_fd;                                                         \
    int kvm_vcpu_dirty;                                                 \
    struct hax_vcpu_state *hax_vcpu;

#endif
