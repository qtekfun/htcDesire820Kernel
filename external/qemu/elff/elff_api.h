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


#ifndef ELFF_API_H_
#define ELFF_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void* ELFF_HANDLE;

typedef struct Elf_InlineInfo {
  const char*     routine_name;

  const char*     inlined_in_file;

  const char*     inlined_in_file_dir;

  uint32_t        inlined_at_line;
} Elf_InlineInfo;

static inline int
elfinlineinfo_is_last_entry(const Elf_InlineInfo* info) {
    return info->routine_name == 0;
}

typedef struct Elf_AddressInfo {
  const char*       routine_name;

  const char*       file_name;

  const char*       dir_name;

  uint32_t          line_number;

  Elf_InlineInfo*   inline_stack;
} Elf_AddressInfo;


ELFF_HANDLE elff_init(const char* elf_file_path);

void elff_close(ELFF_HANDLE handle);

int elff_is_exec(ELFF_HANDLE handle);

int elff_get_pc_address_info(ELFF_HANDLE handle,
                             uint64_t address,
                             Elf_AddressInfo* address_info);

void elff_free_pc_address_info(ELFF_HANDLE handle,
                               Elf_AddressInfo* address_info);

#ifdef __cplusplus
}   
#endif

#endif  
