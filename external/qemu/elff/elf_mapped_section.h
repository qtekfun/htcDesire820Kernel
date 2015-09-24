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


#ifndef ELFF_ELF_MAPPED_SECTION_H_
#define ELFF_ELF_MAPPED_SECTION_H_

#include "elf_defs.h"
#include "android/utils/mapfile.h"

class ElfMappedSection {
 public:
  
  ElfMappedSection();

  
  ~ElfMappedSection();

  bool map(MapFile* handle, Elf_Xword offset, Elf_Word size);

  
  bool is_mapped() const {
    return mapped_at_ != NULL;
  }

  
  const void* data() const {
    assert(is_mapped());
    return data_;
  }

  
  Elf_Word size() const {
    assert(is_mapped());
    return size_;
  }

  
  bool is_contained(const void* ptr, size_t rsize) const {
    assert(is_mapped());
    return is_mapped() && is_in_section(ptr, rsize, data(), size());
  }

 protected:
  void*         mapped_at_;

  
  const void*   data_;

  
  Elf_Word      size_;
};

#endif  
