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


#include "qemu-common.h"
#include "cpu.h"
#include "exec-all.h"

#define OUTSIDE_JIT
#define MMUSUFFIX       _outside_jit
#define GETPC()         NULL
#define env             cpu_single_env
#define ACCESS_TYPE     1
#define CPU_MMU_INDEX   (cpu_mmu_index(env))

#define SHIFT 0
#include "softmmu_template.h"

#define SHIFT 1
#include "softmmu_template.h"

#define SHIFT 2
#include "softmmu_template.h"

#define SHIFT 3
#include "softmmu_template.h"

#undef CPU_MMU_INDEX
#undef ACCESS_TYPE
#undef env
#undef MMUSUFFIX
