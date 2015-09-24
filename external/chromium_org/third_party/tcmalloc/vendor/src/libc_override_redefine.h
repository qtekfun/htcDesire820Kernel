// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_LIBC_OVERRIDE_REDEFINE_H_
#define TCMALLOC_LIBC_OVERRIDE_REDEFINE_H_

#ifdef HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>    
#endif

#ifndef __THROW    
# define __THROW   
#endif

void* operator new(size_t size)                  { return tc_new(size);       }
void operator delete(void* p) __THROW            { tc_delete(p);              }
void* operator new[](size_t size)                { return tc_newarray(size);  }
void operator delete[](void* p) __THROW          { tc_deletearray(p);         }
void* operator new(size_t size, const std::nothrow_t& nt) __THROW {
  return tc_new_nothrow(size, nt);
}
void* operator new[](size_t size, const std::nothrow_t& nt) __THROW {
  return tc_newarray_nothrow(size, nt);
}
void operator delete(void* ptr, const std::nothrow_t& nt) __THROW {
  return tc_delete_nothrow(ptr, nt);
}
void operator delete[](void* ptr, const std::nothrow_t& nt) __THROW {
  return tc_deletearray_nothrow(ptr, nt);
}
extern "C" {
  void* malloc(size_t s) __THROW                 { return tc_malloc(s);       }
  void  free(void* p) __THROW                    { tc_free(p);                }
  void* realloc(void* p, size_t s) __THROW       { return tc_realloc(p, s);   }
  void* calloc(size_t n, size_t s) __THROW       { return tc_calloc(n, s);    }
  void  cfree(void* p) __THROW                   { tc_cfree(p);               }
  void* memalign(size_t a, size_t s) __THROW     { return tc_memalign(a, s);  }
  void* valloc(size_t s) __THROW                 { return tc_valloc(s);       }
  void* pvalloc(size_t s) __THROW                { return tc_pvalloc(s);      }
  int posix_memalign(void** r, size_t a, size_t s) __THROW {
    return tc_posix_memalign(r, a, s);
  }
  void malloc_stats(void) __THROW                { tc_malloc_stats();         }
  int mallopt(int cmd, int v) __THROW            { return tc_mallopt(cmd, v); }
#ifdef HAVE_STRUCT_MALLINFO
  struct mallinfo mallinfo(void) __THROW         { return tc_mallinfo();      }
#endif
  size_t malloc_size(void* p) __THROW            { return tc_malloc_size(p); }
  size_t malloc_usable_size(void* p) __THROW     { return tc_malloc_size(p); }
}  

static void ReplaceSystemAlloc() { }

#endif  
