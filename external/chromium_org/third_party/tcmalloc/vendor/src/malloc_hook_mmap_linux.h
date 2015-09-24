// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT



#ifndef __linux
# error Should only be including malloc_hook_mmap_linux.h on linux systems.
#endif

#include <unistd.h>
#include <syscall.h>
#include <sys/mman.h>
#include <errno.h>
#include "base/linux_syscall_support.h"


#if defined(__x86_64__) || defined(__PPC64__) || (defined(_MIPS_SIM) && _MIPS_SIM == _ABI64)

static inline void* do_mmap64(void *start, size_t length,
                              int prot, int flags,
                              int fd, __off64_t offset) __THROW {
  return sys_mmap(start, length, prot, flags, fd, offset);
}

#define MALLOC_HOOK_HAVE_DO_MMAP64 1

#elif defined(__i386__) || defined(__PPC__) || defined(__mips__) || \
      defined(__arm__)

static inline void* do_mmap64(void *start, size_t length,
                              int prot, int flags,
                              int fd, __off64_t offset) __THROW {
  void *result;

  
  static bool have_mmap2 = true;
  if (have_mmap2) {
    static int pagesize = 0;
    if (!pagesize) pagesize = getpagesize();

    
    if (offset & (pagesize - 1)) {
      result = MAP_FAILED;
      errno = EINVAL;
      goto out;
    }

    result = (void *)syscall(SYS_mmap2,
                             start, length, prot, flags, fd,
                             (off_t) (offset / pagesize));
    if (result != MAP_FAILED || errno != ENOSYS)  goto out;

    
    have_mmap2 = false;
  }

  if (((off_t)offset) != offset) {
    
    
    result = MAP_FAILED;
    errno = EINVAL;
    goto out;
  }

#ifdef __NR_mmap
  {
    
    
    int32 args[6] = { (int32) start, (int32) length, prot, flags, fd,
                      (off_t) offset };
    result = (void *)syscall(SYS_mmap, args);
  }
#else
  
  result = MAP_FAILED;
#endif

 out:
  return result;
}

#define MALLOC_HOOK_HAVE_DO_MMAP64 1

#endif  


#ifdef MALLOC_HOOK_HAVE_DO_MMAP64



# undef mmap

extern "C" {
  void* mmap64(void *start, size_t length, int prot, int flags,
               int fd, __off64_t offset  ) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  void* mmap(void *start, size_t length,int prot, int flags,
             int fd, off_t offset) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  int munmap(void* start, size_t length) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  void* mremap(void* old_addr, size_t old_size, size_t new_size,
               int flags, ...) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  void* sbrk(ptrdiff_t increment) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
}

extern "C" void* mmap64(void *start, size_t length, int prot, int flags,
                        int fd, __off64_t offset) __THROW {
  MallocHook::InvokePreMmapHook(start, length, prot, flags, fd, offset);
  void *result;
  if (!MallocHook::InvokeMmapReplacement(
          start, length, prot, flags, fd, offset, &result)) {
    result = do_mmap64(start, length, prot, flags, fd, offset);
  }
  MallocHook::InvokeMmapHook(result, start, length, prot, flags, fd, offset);
  return result;
}

# if !defined(__USE_FILE_OFFSET64) || !defined(__REDIRECT_NTH)

extern "C" void* mmap(void *start, size_t length, int prot, int flags,
                      int fd, off_t offset) __THROW {
  MallocHook::InvokePreMmapHook(start, length, prot, flags, fd, offset);
  void *result;
  if (!MallocHook::InvokeMmapReplacement(
          start, length, prot, flags, fd, offset, &result)) {
    result = do_mmap64(start, length, prot, flags, fd,
                       static_cast<size_t>(offset)); 
  }
  MallocHook::InvokeMmapHook(result, start, length, prot, flags, fd, offset);
  return result;
}

# endif  

extern "C" int munmap(void* start, size_t length) __THROW {
  MallocHook::InvokeMunmapHook(start, length);
  int result;
  if (!MallocHook::InvokeMunmapReplacement(start, length, &result)) {
    result = sys_munmap(start, length);
  }
  return result;
}

extern "C" void* mremap(void* old_addr, size_t old_size, size_t new_size,
                        int flags, ...) __THROW {
  va_list ap;
  va_start(ap, flags);
  void *new_address = va_arg(ap, void *);
  va_end(ap);
  void* result = sys_mremap(old_addr, old_size, new_size, flags, new_address);
  MallocHook::InvokeMremapHook(result, old_addr, old_size, new_size, flags,
                               new_address);
  return result;
}

extern "C" void* __sbrk(ptrdiff_t increment);

extern "C" void* sbrk(ptrdiff_t increment) __THROW {
  MallocHook::InvokePreSbrkHook(increment);
  void *result = __sbrk(increment);
  MallocHook::InvokeSbrkHook(result, increment);
  return result;
}

void* MallocHook::UnhookedMMap(void *start, size_t length, int prot,
                                         int flags, int fd, off_t offset) {
  void* result;
  if (!MallocHook::InvokeMmapReplacement(
          start, length, prot, flags, fd, offset, &result)) {
    result = do_mmap64(start, length, prot, flags, fd, offset);
  }
  return result;
}

int MallocHook::UnhookedMUnmap(void *start, size_t length) {
  int result;
  if (!MallocHook::InvokeMunmapReplacement(start, length, &result)) {
    result = syscall(SYS_munmap, start, length);
  }
  return result;
}

#undef MALLOC_HOOK_HAVE_DO_MMAP64

#endif  
