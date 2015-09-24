// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef _MALLOC_HOOK_H_
#define _MALLOC_HOOK_H_

#include <stddef.h>
#include <sys/types.h>
extern "C" {
#include <gperftools/malloc_hook_c.h>  
}

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4800)
#endif


class PERFTOOLS_DLL_DECL MallocHook {
 public:
  
  
  typedef MallocHook_NewHook NewHook;
  inline static bool AddNewHook(NewHook hook) {
    return MallocHook_AddNewHook(hook);
  }
  inline static bool RemoveNewHook(NewHook hook) {
    return MallocHook_RemoveNewHook(hook);
  }
  inline static void InvokeNewHook(const void* p, size_t s);

  
  
  typedef MallocHook_DeleteHook DeleteHook;
  inline static bool AddDeleteHook(DeleteHook hook) {
    return MallocHook_AddDeleteHook(hook);
  }
  inline static bool RemoveDeleteHook(DeleteHook hook) {
    return MallocHook_RemoveDeleteHook(hook);
  }
  inline static void InvokeDeleteHook(const void* p);

  
  
  
  
  typedef MallocHook_PreMmapHook PreMmapHook;
  inline static bool AddPreMmapHook(PreMmapHook hook) {
    return MallocHook_AddPreMmapHook(hook);
  }
  inline static bool RemovePreMmapHook(PreMmapHook hook) {
    return MallocHook_RemovePreMmapHook(hook);
  }
  inline static void InvokePreMmapHook(const void* start,
                                       size_t size,
                                       int protection,
                                       int flags,
                                       int fd,
                                       off_t offset);

  
  
  
  
  
  
  
  
  
  
  typedef MallocHook_MmapReplacement MmapReplacement;
  inline static bool SetMmapReplacement(MmapReplacement hook) {
    return MallocHook_SetMmapReplacement(hook);
  }
  inline static bool RemoveMmapReplacement(MmapReplacement hook) {
    return MallocHook_RemoveMmapReplacement(hook);
  }
  inline static bool InvokeMmapReplacement(const void* start,
                                           size_t size,
                                           int protection,
                                           int flags,
                                           int fd,
                                           off_t offset,
                                           void** result);


  
  
  typedef MallocHook_MmapHook MmapHook;
  inline static bool AddMmapHook(MmapHook hook) {
    return MallocHook_AddMmapHook(hook);
  }
  inline static bool RemoveMmapHook(MmapHook hook) {
    return MallocHook_RemoveMmapHook(hook);
  }
  inline static void InvokeMmapHook(const void* result,
                                    const void* start,
                                    size_t size,
                                    int protection,
                                    int flags,
                                    int fd,
                                    off_t offset);

  
  
  
  
  
  
  
  
  
  typedef MallocHook_MunmapReplacement MunmapReplacement;
  inline static bool SetMunmapReplacement(MunmapReplacement hook) {
    return MallocHook_SetMunmapReplacement(hook);
  }
  inline static bool RemoveMunmapReplacement(MunmapReplacement hook) {
    return MallocHook_RemoveMunmapReplacement(hook);
  }
  inline static bool InvokeMunmapReplacement(const void* p,
                                             size_t size,
                                             int* result);

  
  typedef MallocHook_MunmapHook MunmapHook;
  inline static bool AddMunmapHook(MunmapHook hook) {
    return MallocHook_AddMunmapHook(hook);
  }
  inline static bool RemoveMunmapHook(MunmapHook hook) {
    return MallocHook_RemoveMunmapHook(hook);
  }
  inline static void InvokeMunmapHook(const void* p, size_t size);

  
  typedef MallocHook_MremapHook MremapHook;
  inline static bool AddMremapHook(MremapHook hook) {
    return MallocHook_AddMremapHook(hook);
  }
  inline static bool RemoveMremapHook(MremapHook hook) {
    return MallocHook_RemoveMremapHook(hook);
  }
  inline static void InvokeMremapHook(const void* result,
                                      const void* old_addr,
                                      size_t old_size,
                                      size_t new_size,
                                      int flags,
                                      const void* new_addr);

  
  
  
  
  
  
  typedef MallocHook_PreSbrkHook PreSbrkHook;
  inline static bool AddPreSbrkHook(PreSbrkHook hook) {
    return MallocHook_AddPreSbrkHook(hook);
  }
  inline static bool RemovePreSbrkHook(PreSbrkHook hook) {
    return MallocHook_RemovePreSbrkHook(hook);
  }
  inline static void InvokePreSbrkHook(ptrdiff_t increment);

  
  
  
  
  typedef MallocHook_SbrkHook SbrkHook;
  inline static bool AddSbrkHook(SbrkHook hook) {
    return MallocHook_AddSbrkHook(hook);
  }
  inline static bool RemoveSbrkHook(SbrkHook hook) {
    return MallocHook_RemoveSbrkHook(hook);
  }
  inline static void InvokeSbrkHook(const void* result, ptrdiff_t increment);

  
  
  
  
  
  inline static int GetCallerStackTrace(void** result, int max_depth,
                                        int skip_count) {
    return MallocHook_GetCallerStackTrace(result, max_depth, skip_count);
  }

  
  
  
  
  static void* UnhookedMMap(void *start, size_t length, int prot, int flags,
                            int fd, off_t offset);
  static int UnhookedMUnmap(void *start, size_t length);

  
  inline static NewHook GetNewHook();
  inline static NewHook SetNewHook(NewHook hook) {
    return MallocHook_SetNewHook(hook);
  }

  inline static DeleteHook GetDeleteHook();
  inline static DeleteHook SetDeleteHook(DeleteHook hook) {
    return MallocHook_SetDeleteHook(hook);
  }

  inline static PreMmapHook GetPreMmapHook();
  inline static PreMmapHook SetPreMmapHook(PreMmapHook hook) {
    return MallocHook_SetPreMmapHook(hook);
  }

  inline static MmapHook GetMmapHook();
  inline static MmapHook SetMmapHook(MmapHook hook) {
    return MallocHook_SetMmapHook(hook);
  }

  inline static MunmapHook GetMunmapHook();
  inline static MunmapHook SetMunmapHook(MunmapHook hook) {
    return MallocHook_SetMunmapHook(hook);
  }

  inline static MremapHook GetMremapHook();
  inline static MremapHook SetMremapHook(MremapHook hook) {
    return MallocHook_SetMremapHook(hook);
  }

  inline static PreSbrkHook GetPreSbrkHook();
  inline static PreSbrkHook SetPreSbrkHook(PreSbrkHook hook) {
    return MallocHook_SetPreSbrkHook(hook);
  }

  inline static SbrkHook GetSbrkHook();
  inline static SbrkHook SetSbrkHook(SbrkHook hook) {
    return MallocHook_SetSbrkHook(hook);
  }
  

 private:
  
  static void InvokeNewHookSlow(const void* p, size_t s);
  static void InvokeDeleteHookSlow(const void* p);
  static void InvokePreMmapHookSlow(const void* start,
                                    size_t size,
                                    int protection,
                                    int flags,
                                    int fd,
                                    off_t offset);
  static void InvokeMmapHookSlow(const void* result,
                                 const void* start,
                                 size_t size,
                                 int protection,
                                 int flags,
                                 int fd,
                                 off_t offset);
  static bool InvokeMmapReplacementSlow(const void* start,
                                        size_t size,
                                        int protection,
                                        int flags,
                                        int fd,
                                        off_t offset,
                                        void** result);
  static void InvokeMunmapHookSlow(const void* p, size_t size);
  static bool InvokeMunmapReplacementSlow(const void* p,
                                          size_t size,
                                          int* result);
  static void InvokeMremapHookSlow(const void* result,
                                   const void* old_addr,
                                   size_t old_size,
                                   size_t new_size,
                                   int flags,
                                   const void* new_addr);
  static void InvokePreSbrkHookSlow(ptrdiff_t increment);
  static void InvokeSbrkHookSlow(const void* result, ptrdiff_t increment);
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif


#endif 
