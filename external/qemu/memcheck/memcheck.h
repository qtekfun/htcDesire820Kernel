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


#ifndef QEMU_MEMCHECK_MEMCHECK_H
#define QEMU_MEMCHECK_MEMCHECK_H

#include "memcheck_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void memcheck_init(const char* tracing_flags);


void memcheck_guest_libc_initialized(uint32_t pid);

void memcheck_guest_alloc(target_ulong guest_address);

void memcheck_guest_free(target_ulong guest_address);

void memcheck_guest_query_malloc(target_ulong guest_address);

void memcheck_guest_print_str(target_ulong str);


void memcheck_init_pid(uint32_t pid);

void memcheck_switch(uint32_t tid);

void memcheck_fork(uint32_t tgid, uint32_t new_pid);

void memcheck_clone(uint32_t tgid, uint32_t new_tid);

void memcheck_set_cmd_line(const char* cmd_arg, unsigned cmdlen);

void memcheck_exit(uint32_t exit_code);

void memcheck_mmap_exepath(target_ulong vstart,
                           target_ulong vend,
                           target_ulong exec_offset,
                           const char* path);

void memcheck_unmap(target_ulong vstart, target_ulong vend);

extern int memcheck_enabled;

#ifdef __cplusplus
};  
#endif

#endif  
