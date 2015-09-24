// Copyright (c) 2006, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef BASE_SYMBOLIZE_H_
#define BASE_SYMBOLIZE_H_

#include "utilities.h"
#include "config.h"
#include "glog/logging.h"

#ifdef HAVE_SYMBOLIZE

#if defined(__ELF__)  
#if defined(__OpenBSD__)
#include <sys/exec_elf.h>
#else
#include <elf.h>
#endif

#if !defined(ANDROID)
#include <link.h>  
#endif

#if !defined(SIZEOF_VOID_P)
# if defined(__LP64__)
#  define SIZEOF_VOID_P 8
# else
#  define SIZEOF_VOID_P 4
# endif
#endif

#ifndef ElfW
# if SIZEOF_VOID_P == 4
#  define ElfW(type) Elf32_##type
# elif SIZEOF_VOID_P == 8
#  define ElfW(type) Elf64_##type
# else
#  error "Unknown sizeof(void *)"
# endif
#endif

_START_GOOGLE_NAMESPACE_

bool GetSectionHeaderByName(int fd, const char *name, size_t name_len,
                            ElfW(Shdr) *out);

_END_GOOGLE_NAMESPACE_

#endif  

_START_GOOGLE_NAMESPACE_

// and return the size of the output written. On error, the callback
typedef int (*SymbolizeCallback)(int fd, void *pc, char *out, size_t out_size,
                                 uint64 relocation);
void InstallSymbolizeCallback(SymbolizeCallback callback);

_END_GOOGLE_NAMESPACE_

#endif

_START_GOOGLE_NAMESPACE_

bool Symbolize(void *pc, char *out, int out_size);

_END_GOOGLE_NAMESPACE_

#endif  
