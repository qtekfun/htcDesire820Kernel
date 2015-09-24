// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef __FreeBSD__
# error Should only be including malloc_hook_mmap_freebsd.h on FreeBSD systems.
#endif

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <errno.h>

#undef mmap

#if defined(__amd64__) || defined(__x86_64__)
# define MALLOC_HOOK_SYSCALL __syscall
#else
# define MALLOC_HOOK_SYSCALL syscall
#endif


extern "C" {
  void* mmap(void *start, size_t length,int prot, int flags,
             int fd, off_t offset) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  int munmap(void* start, size_t length) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
  void* sbrk(intptr_t increment) __THROW
    ATTRIBUTE_SECTION(malloc_hook);
}

static inline void* do_mmap(void *start, size_t length,
                            int prot, int flags,
                            int fd, off_t offset) __THROW {
  return (void *)MALLOC_HOOK_SYSCALL(SYS_mmap,
                                     start, length, prot, flags, fd, offset);
}

static inline void* do_sbrk(intptr_t increment) {
  void* curbrk = 0;

#if defined(__x86_64__) || defined(__amd64__)
# ifdef PIC
  __asm__ __volatile__(
      "movq .curbrk@GOTPCREL(%%rip), %%rdx;"
      "movq (%%rdx), %%rax;"
      "movq %%rax, %0;"
      : "=r" (curbrk)
      :: "%rdx", "%rax");
# else
  __asm__ __volatile__(
      "movq .curbrk(%%rip), %%rax;"
      "movq %%rax, %0;"
      : "=r" (curbrk)
      :: "%rax");
# endif
#else
  __asm__ __volatile__(
      "movl .curbrk, %%eax;"
      "movl %%eax, %0;"
      : "=r" (curbrk)
      :: "%eax");
#endif

  if (increment == 0) {
    return curbrk;
  }

  char* prevbrk = static_cast<char*>(curbrk);
  void* newbrk = prevbrk + increment;

  if (brk(newbrk) == -1) {
    return reinterpret_cast<void*>(static_cast<intptr_t>(-1));
  }

  return prevbrk;
}


extern "C" void* mmap(void *start, size_t length, int prot, int flags,
                      int fd, off_t offset) __THROW {
  MallocHook::InvokePreMmapHook(start, length, prot, flags, fd, offset);
  void *result;
  if (!MallocHook::InvokeMmapReplacement(
          start, length, prot, flags, fd, offset, &result)) {
    result = do_mmap(start, length, prot, flags, fd,
                       static_cast<size_t>(offset)); 
  }
  MallocHook::InvokeMmapHook(result, start, length, prot, flags, fd, offset);
  return result;
}

extern "C" int munmap(void* start, size_t length) __THROW {
  MallocHook::InvokeMunmapHook(start, length);
  int result;
  if (!MallocHook::InvokeMunmapReplacement(start, length, &result)) {
    result = MALLOC_HOOK_SYSCALL(SYS_munmap, start, length);
  }

  return result;
}

extern "C" void* sbrk(intptr_t increment) __THROW {
  MallocHook::InvokePreSbrkHook(increment);
  void *result = do_sbrk(increment);
  MallocHook::InvokeSbrkHook(result, increment);
  return result;
}

void* MallocHook::UnhookedMMap(void *start, size_t length, int prot,
                                         int flags, int fd, off_t offset) {
  void* result;
  if (!MallocHook::InvokeMmapReplacement(
	  start, length, prot, flags, fd, offset, &result)) {
    result = do_mmap(start, length, prot, flags, fd, offset);
  }

  return result;
}

int MallocHook::UnhookedMUnmap(void *start, size_t length) {
  int result;
  if (!MallocHook::InvokeMunmapReplacement(start, length, &result)) {
    result = MALLOC_HOOK_SYSCALL(SYS_munmap, start, length);
  }
  return result;
}

#undef MALLOC_HOOK_SYSCALL
