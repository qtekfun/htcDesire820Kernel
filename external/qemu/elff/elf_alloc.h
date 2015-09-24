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


#ifndef ELFF_ELF_ALLOC_H_
#define ELFF_ELF_ALLOC_H_

#include <stdint.h>
#include "elff-common.h"

class ElfFile;

#define ELFALLOC_ALIGNMENT_MASK 3

#define ELF_ALLOC_CHUNK_SIZE  (32 * 1024)

typedef struct ElfAllocatorChunk {
  ElfAllocatorChunk*  prev;

  
  void*               avail;

  
  size_t              size;

  
  size_t              remains;
} ElfAllocatorChunk;

/* Encapsulates memory allocator for DWARF-related objects.
 * Due to the implementation of ELF/DWARF framework in this library, data,
 * collected during ELF/DWARF parsing stays in memory for as long, as instance
 * of ElfFile that's being parsed is alive. To save performance on the numerous
 * memory allocations (and then, deallocations) we will use this simple memory
 * allocator that will grab memory from the heap in large chunks and then will
 * provide DWARF objects with blocks of the required size inside those chunks.
 * This will be much faster than going to the heap all the time, and since we
 * will use overwritten operators new/delete for the DWARF objects that use
 * this allocator, this is going to be pretty flexible and reliable solution
 * for DWARF object allocation implementation. See DwarfAllocBase for more
 * details.
 *
 * Instance (always one) of this class is created by ElfFile object when it is
 * initializing.
 */
class ElfAllocator {
 public:
  
  ElfAllocator();

  
  ~ElfAllocator();

  void* alloc(size_t size);

 protected:
  ElfAllocatorChunk*  current_chunk_;
};

class DwarfAllocBase {
 public:
  
  DwarfAllocBase() {
  }

  
  virtual ~DwarfAllocBase() {
  }

  void* operator new(size_t size, const ElfFile* elf);

  /* Overwitten operator delete.
   * Since deleting for chunk-allocated objects is a "no-op", we don't do
   * anything in this operator. We, however, are obliged to implement this
   * operator in order to compliment overwritten operator 'new'.
   */
  void operator delete(void* ptr) {
  }

  /* Overwitten operator delete.
   * Since deleting for chunk-allocated objects is a "no-op", we don't do
   * anything in this operator. We, however, are obliged to implement this
   * operator in order to compliment overwritten operator 'new'.
   */
  void operator delete[](void* ptr) {
  }

 private:
  void* operator new(size_t size) throw() {
    return NULL;
  }
};

extern "C" void* elff_alloc(size_t  size);
extern "C" void  elff_free(void* ptr);

#endif  
