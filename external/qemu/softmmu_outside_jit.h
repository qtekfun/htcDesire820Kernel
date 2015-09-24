/* Copyright (C) 2007-2009 The Android Open Source Project
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

#ifndef QEMU_SOFTMMU_OUTSIDE_JIT_H
#define QEMU_SOFTMMU_OUTSIDE_JIT_H

#ifdef __cplusplus
extern "C" {
#endif


uint8_t REGPARM __ldb_outside_jit(target_ulong addr, int mmu_idx);
void REGPARM __stb_outside_jit(target_ulong addr, uint8_t val, int mmu_idx);
uint16_t REGPARM __ldw_outside_jit(target_ulong addr, int mmu_idx);
void REGPARM __stw_outside_jit(target_ulong addr, uint16_t val, int mmu_idx);
uint32_t REGPARM __ldl_outside_jit(target_ulong addr, int mmu_idx);
void REGPARM __stl_outside_jit(target_ulong addr, uint32_t val, int mmu_idx);
uint64_t REGPARM __ldq_outside_jit(target_ulong addr, int mmu_idx);
void REGPARM __stq_outside_jit(target_ulong addr, uint64_t val, int mmu_idx);

#define OUTSIDE_JIT
#define env cpu_single_env

#if defined(TARGET_MIPS)
#define MEMSUFFIX MMU_MODE2_SUFFIX
#else
#define MEMSUFFIX MMU_MODE1_SUFFIX
#endif
#define ACCESS_TYPE 1

#define DATA_SIZE 1
#include "softmmu_header.h"

#define DATA_SIZE 2
#include "softmmu_header.h"

#define DATA_SIZE 4
#include "softmmu_header.h"

#define DATA_SIZE 8
#include "softmmu_header.h"

#undef MEMSUFFIX
#undef ACCESS_TYPE

#define MEMSUFFIX MMU_MODE0_SUFFIX
#define ACCESS_TYPE 0

#define DATA_SIZE 1
#include "softmmu_header.h"

#define DATA_SIZE 2
#include "softmmu_header.h"

#define DATA_SIZE 4
#include "softmmu_header.h"

#define DATA_SIZE 8
#include "softmmu_header.h"

#undef MEMSUFFIX
#undef ACCESS_TYPE

#undef env
#undef OUTSIDE_JIT

#ifdef __cplusplus
};  
#endif

#endif  
