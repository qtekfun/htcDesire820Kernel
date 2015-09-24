// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_HEAP_CHECKER_H_
#define BASE_HEAP_CHECKER_H_

#include <sys/types.h>  
#ifndef _MSC_VER
#include <stdint.h>     
#endif
#include <stdarg.h>     
#include <vector>

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif


class PERFTOOLS_DLL_DECL HeapLeakChecker {
 public:

  
  

  
  
  
  
  
  
  
  static bool IsActive();

  
  
  
  
  
  
  
  static HeapLeakChecker* GlobalChecker();

  
  
  
  
  
  
  static bool NoGlobalLeaks();

  
  
  
  
  static void CancelGlobalCheck();

  
  

  
  
  
  
  
  explicit HeapLeakChecker(const char *name);

  
  
  ~HeapLeakChecker();

  
  
  
  
  
  
  
  bool NoLeaks() { return DoNoLeaks(DO_NOT_SYMBOLIZE); }

  
  
  bool QuickNoLeaks()  { return NoLeaks(); }
  bool BriefNoLeaks()  { return NoLeaks(); }
  bool SameHeap()      { return NoLeaks(); }
  bool QuickSameHeap() { return NoLeaks(); }
  bool BriefSameHeap() { return NoLeaks(); }

  
  
  
  
  
  
  
  
  ssize_t BytesLeaked() const;
  ssize_t ObjectsLeaked() const;

  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class Disabler {
   public:
    Disabler();
    ~Disabler();
   private:
    Disabler(const Disabler&);        
    void operator=(const Disabler&);  
  };

  
  
  
  
  
  
  
  
  
  
  
  
  template <typename T>
  static T* IgnoreObject(T* ptr) {
    DoIgnoreObject(static_cast<const void*>(const_cast<const T*>(ptr)));
    return ptr;
  }

  
  
  
  static void UnIgnoreObject(const void* ptr);

  
  

  class Allocator;
  struct RangeValue;

 private:

  
  

  
  
  char* MakeProfileNameLocked();

  
  void Create(const char *name, bool make_start_snapshot);

  enum ShouldSymbolize { SYMBOLIZE, DO_NOT_SYMBOLIZE };

  
  bool DoNoLeaks(ShouldSymbolize should_symbolize);

  
  static bool NoGlobalLeaksMaybeSymbolize(ShouldSymbolize should_symbolize);

  
  
  
  static void* GetDisableChecksStart();
  static void DisableChecksToHereFrom(const void* start_address);
  static void DisableChecksIn(const char* pattern);
  friend void RangeDisabledLeaks();
  friend void NamedTwoDisabledLeaks();
  friend void* RunNamedDisabledLeaks(void*);
  friend void TestHeapLeakCheckerNamedDisabling();
  
  friend int main(int, char**);


  
  static void DoIgnoreObject(const void* ptr);

  
  
  
  static void DisableChecksFromToLocked(const void* start_address,
                                        const void* end_address,
                                        int max_depth);

  
  static void IgnoreAllLiveObjectsLocked(const void* self_stack_top);

  
  
  
  
  
  
  
  
  
  
  
  static int IgnoreLiveThreadsLocked(void* parameter,
                                     int num_threads,
                                     pid_t* thread_pids,
                                     va_list ap);

  
  
  
  
  
  
  static void IgnoreNonThreadLiveObjectsLocked();

  
  
  
  
  
  
  
  static void IgnoreLiveObjectsLocked(const char* name, const char* name2);

  
  
  static bool DoMainHeapCheck();

  
  enum ProcMapsTask {
    RECORD_GLOBAL_DATA,
    DISABLE_LIBRARY_ALLOCS
  };

  
  enum ProcMapsResult {
    PROC_MAPS_USED,
    CANT_OPEN_PROC_MAPS,
    NO_SHARED_LIBS_IN_PROC_MAPS
  };

  
  static ProcMapsResult UseProcMapsLocked(ProcMapsTask proc_maps_task);

  
  
  
  static void DisableLibraryAllocsLocked(const char* library,
                                         uintptr_t start_address,
                                         uintptr_t end_address);

  
  
  
  
  
  
  static inline bool HaveOnHeapLocked(const void** ptr, size_t* object_size);

  
  
  static void TurnItselfOffLocked();

  
  HeapLeakChecker();

  
  

  
  
  
  static const void* GetAllocCaller(void* ptr);
  friend void VerifyHeapProfileTableStackGet();

  
  static void BeforeConstructorsLocked();
  friend void HeapLeakChecker_BeforeConstructors();

  
  friend void HeapLeakChecker_AfterDestructors();

  
  friend void HeapLeakChecker_InternalInitStart();

  
  
  friend void HeapLeakChecker_RunHeapCleanups();

  
  

  class SpinLock* lock_;  
  const char* name_;  
                      

  
  
  
  void* start_snapshot_;

  bool has_checked_;  
  ssize_t inuse_bytes_increase_;  
  ssize_t inuse_allocs_increase_;  
                                   
  bool keep_profiles_;  

  

  
  HeapLeakChecker(const HeapLeakChecker&);
  void operator=(const HeapLeakChecker&);
};


template <class T>
class HiddenPointer {
 public:
  explicit HiddenPointer(T* t)
      : masked_t_(reinterpret_cast<uintptr_t>(t) ^ kHideMask) {
  }
  
  T* get() const { return reinterpret_cast<T*>(masked_t_ ^ kHideMask); }

 private:
  
  
  static const uintptr_t kHideMask =
      static_cast<uintptr_t>(0xF03A5F7BF03A5F7Bll);
  uintptr_t masked_t_;
};

class PERFTOOLS_DLL_DECL HeapCleaner {
 public:
  typedef void (*void_function)(void);
  HeapCleaner(void_function f);
  static void RunHeapCleanups();
 private:
  static std::vector<void_function>* heap_cleanups_;
};

#define REGISTER_HEAPCHECK_CLEANUP(name, body)  \
  namespace { \
  void heapcheck_cleanup_##name() { body; } \
  static HeapCleaner heapcheck_cleaner_##name(&heapcheck_cleanup_##name); \
  }

#endif  
