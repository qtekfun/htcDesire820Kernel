// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_VDSO_SUPPORT_H_
#define BASE_VDSO_SUPPORT_H_

#include <config.h>
#include "base/basictypes.h"
#include "base/elf_mem_image.h"

#ifdef HAVE_ELF_MEM_IMAGE

#define HAVE_VDSO_SUPPORT 1

#include <stdlib.h>     

namespace base {

class VDSOSupport {
 public:
  VDSOSupport();

  typedef ElfMemImage::SymbolInfo SymbolInfo;
  typedef ElfMemImage::SymbolIterator SymbolIterator;

  
  bool IsPresent() const { return image_.IsPresent(); }

  
  SymbolIterator begin() const { return image_.begin(); }
  SymbolIterator end() const { return image_.end(); }

  
  
  
  
  bool LookupSymbol(const char *name, const char *version,
                    int symbol_type, SymbolInfo *info_out) const;

  
  
  
  
  bool LookupSymbolByAddress(const void *address, SymbolInfo *info_out) const;

  
  
  
  
  const void *SetBase(const void *s);

  
  
  static const void *Init();

 private:
  
  
  ElfMemImage image_;

  
  
  
  
  
  
  
  
  
  static const void *vdso_base_;

  
  
  
  
  static long InitAndGetCPU(unsigned *cpu, void *cache,     
                            void *unused);
  static long GetCPUViaSyscall(unsigned *cpu, void *cache,  
                               void *unused);
  typedef long (*GetCpuFn)(unsigned *cpu, void *cache,      
                           void *unused);

  
  
  static GetCpuFn getcpu_fn_;

  friend int GetCPU(void);  

  DISALLOW_COPY_AND_ASSIGN(VDSOSupport);
};

int GetCPU();
}  

#endif  

#endif  
