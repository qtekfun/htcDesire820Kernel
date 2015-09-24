// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DYNAMIC_ANNOTATIONS_H_
#define BASE_DYNAMIC_ANNOTATIONS_H_


#ifndef NDEBUG

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  #define ANNOTATE_CONDVAR_LOCK_WAIT(cv, lock) \
    AnnotateCondVarWait(__FILE__, __LINE__, cv, lock)

  
  
  #define ANNOTATE_CONDVAR_WAIT(cv) \
    AnnotateCondVarWait(__FILE__, __LINE__, cv, NULL)

  
  
  #define ANNOTATE_CONDVAR_SIGNAL(cv) \
    AnnotateCondVarSignal(__FILE__, __LINE__, cv)

  
  #define ANNOTATE_CONDVAR_SIGNAL_ALL(cv) \
    AnnotateCondVarSignalAll(__FILE__, __LINE__, cv)

  
  #define ANNOTATE_HAPPENS_BEFORE(obj) ANNOTATE_CONDVAR_SIGNAL(obj)
  #define ANNOTATE_HAPPENS_AFTER(obj)  ANNOTATE_CONDVAR_WAIT(obj)

  
  
  
  
  #define ANNOTATE_PUBLISH_MEMORY_RANGE(pointer, size) \
    AnnotatePublishMemoryRange(__FILE__, __LINE__, pointer, size)

  
  
  
  
  #define ANNOTATE_MUTEX_IS_USED_AS_CONDVAR(mu) \
    AnnotateMutexIsUsedAsCondVar(__FILE__, __LINE__, mu)

  
  
  

  
  
  
  
  #define ANNOTATE_NEW_MEMORY(address, size) \
    AnnotateNewMemory(__FILE__, __LINE__, address, size)

  
  
  

  
  
  
  
  #define ANNOTATE_PCQ_CREATE(pcq) \
    AnnotatePCQCreate(__FILE__, __LINE__, pcq)

  
  #define ANNOTATE_PCQ_DESTROY(pcq) \
    AnnotatePCQDestroy(__FILE__, __LINE__, pcq)

  
  
  #define ANNOTATE_PCQ_PUT(pcq) \
    AnnotatePCQPut(__FILE__, __LINE__, pcq)

  
  #define ANNOTATE_PCQ_GET(pcq) \
    AnnotatePCQGet(__FILE__, __LINE__, pcq)

  
  
  
  

  
  
  
  
  #define ANNOTATE_BENIGN_RACE(address, description) \
    AnnotateBenignRace(__FILE__, __LINE__, address, description)

  
  
  
  
  
  #define ANNOTATE_IGNORE_READS_BEGIN() \
    AnnotateIgnoreReadsBegin(__FILE__, __LINE__)

  
  #define ANNOTATE_IGNORE_READS_END() \
    AnnotateIgnoreReadsEnd(__FILE__, __LINE__)

  
  #define ANNOTATE_IGNORE_WRITES_BEGIN() \
    AnnotateIgnoreWritesBegin(__FILE__, __LINE__)

  
  #define ANNOTATE_IGNORE_WRITES_END() \
    AnnotateIgnoreWritesEnd(__FILE__, __LINE__)

  
  #define ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN() \
    do {\
      ANNOTATE_IGNORE_READS_BEGIN();\
      ANNOTATE_IGNORE_WRITES_BEGIN();\
    }while(0)\

  
  #define ANNOTATE_IGNORE_READS_AND_WRITES_END() \
    do {\
      ANNOTATE_IGNORE_WRITES_END();\
      ANNOTATE_IGNORE_READS_END();\
    }while(0)\

  
  

  
  #define ANNOTATE_TRACE_MEMORY(address) \
    AnnotateTraceMemory(__FILE__, __LINE__, address)

  
  #define ANNOTATE_THREAD_NAME(name) \
    AnnotateThreadName(__FILE__, __LINE__, name)

  
  
  
  

  
  #define ANNOTATE_RWLOCK_CREATE(lock) \
    AnnotateRWLockCreate(__FILE__, __LINE__, lock)

  
  #define ANNOTATE_RWLOCK_DESTROY(lock) \
    AnnotateRWLockDestroy(__FILE__, __LINE__, lock)

  
  
  #define ANNOTATE_RWLOCK_ACQUIRED(lock, is_w) \
    AnnotateRWLockAcquired(__FILE__, __LINE__, lock, is_w)

  
  #define ANNOTATE_RWLOCK_RELEASED(lock, is_w) \
    AnnotateRWLockReleased(__FILE__, __LINE__, lock, is_w)

  
  

  
  
  #define ANNOTATE_EXPECT_RACE(address, description) \
    AnnotateExpectRace(__FILE__, __LINE__, address, description)

  
  #define ANNOTATE_NO_OP(arg) \
    AnnotateNoOp(__FILE__, __LINE__, arg)

#else  

  #define ANNOTATE_RWLOCK_CREATE(lock) 
  #define ANNOTATE_RWLOCK_DESTROY(lock) 
  #define ANNOTATE_RWLOCK_ACQUIRED(lock, is_w) 
  #define ANNOTATE_RWLOCK_RELEASED(lock, is_w) 
  #define ANNOTATE_CONDVAR_LOCK_WAIT(cv, lock) 
  #define ANNOTATE_CONDVAR_WAIT(cv) 
  #define ANNOTATE_CONDVAR_SIGNAL(cv) 
  #define ANNOTATE_CONDVAR_SIGNAL_ALL(cv) 
  #define ANNOTATE_HAPPENS_BEFORE(obj) 
  #define ANNOTATE_HAPPENS_AFTER(obj) 
  #define ANNOTATE_PUBLISH_MEMORY_RANGE(address, size) 
  #define ANNOTATE_PUBLISH_OBJECT(address) 
  #define ANNOTATE_PCQ_CREATE(pcq) 
  #define ANNOTATE_PCQ_DESTROY(pcq) 
  #define ANNOTATE_PCQ_PUT(pcq) 
  #define ANNOTATE_PCQ_GET(pcq) 
  #define ANNOTATE_NEW_MEMORY(address, size) 
  #define ANNOTATE_EXPECT_RACE(address, description) 
  #define ANNOTATE_BENIGN_RACE(address, description) 
  #define ANNOTATE_MUTEX_IS_USED_AS_CONDVAR(mu) 
  #define ANNOTATE_TRACE_MEMORY(arg) 
  #define ANNOTATE_THREAD_NAME(name) 
  #define ANNOTATE_IGNORE_READS_BEGIN() 
  #define ANNOTATE_IGNORE_READS_END() 
  #define ANNOTATE_IGNORE_WRITES_BEGIN() 
  #define ANNOTATE_IGNORE_WRITES_END() 
  #define ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN() 
  #define ANNOTATE_IGNORE_READS_AND_WRITES_END() 
  #define ANNOTATE_NO_OP(arg) 

#endif  

extern "C" void AnnotateRWLockCreate(const char *file, int line,
                                     const volatile void *lock);
extern "C" void AnnotateRWLockDestroy(const char *file, int line,
                                      const volatile void *lock);
extern "C" void AnnotateRWLockAcquired(const char *file, int line,
                                       const volatile void *lock, long is_w);
extern "C" void AnnotateRWLockReleased(const char *file, int line,
                                       const volatile void *lock, long is_w);
extern "C" void AnnotateCondVarWait(const char *file, int line,
                                    const volatile void *cv,
                                    const volatile void *lock);
extern "C" void AnnotateCondVarSignal(const char *file, int line,
                                      const volatile void *cv);
extern "C" void AnnotateCondVarSignalAll(const char *file, int line,
                                         const volatile void *cv);
extern "C" void AnnotatePublishMemoryRange(const char *file, int line,
                                           const volatile void *address,
                                           long size);
extern "C" void AnnotatePCQCreate(const char *file, int line,
                                  const volatile void *pcq);
extern "C" void AnnotatePCQDestroy(const char *file, int line,
                                   const volatile void *pcq);
extern "C" void AnnotatePCQPut(const char *file, int line,
                               const volatile void *pcq);
extern "C" void AnnotatePCQGet(const char *file, int line,
                               const volatile void *pcq);
extern "C" void AnnotateNewMemory(const char *file, int line,
                                  const volatile void *address,
                                  long size);
extern "C" void AnnotateExpectRace(const char *file, int line,
                                   const volatile void *address,
                                   const char *description);
extern "C" void AnnotateBenignRace(const char *file, int line,
                                   const volatile void *address,
                                   const char *description);
extern "C" void AnnotateMutexIsUsedAsCondVar(const char *file, int line,
                                            const volatile void *mu);
extern "C" void AnnotateTraceMemory(const char *file, int line,
                                    const volatile void *arg);
extern "C" void AnnotateThreadName(const char *file, int line,
                                   const char *name);
extern "C" void AnnotateIgnoreReadsBegin(const char *file, int line);
extern "C" void AnnotateIgnoreReadsEnd(const char *file, int line);
extern "C" void AnnotateIgnoreWritesBegin(const char *file, int line);
extern "C" void AnnotateIgnoreWritesEnd(const char *file, int line);
extern "C" void AnnotateNoOp(const char *file, int line,
                             const volatile void *arg);

#ifndef NDEBUG

  
  
  
  
  
  
  
  
  template <class T>
  inline T ANNOTATE_UNPROTECTED_READ(const volatile T &x) {
    ANNOTATE_IGNORE_READS_BEGIN();
    T res = x;
    ANNOTATE_IGNORE_READS_END();
    return res;
  }

  
  #define ANNOTATE_BENIGN_RACE_STATIC(static_var, description)        \
    namespace {                                                       \
      class static_var ## _annotator {                                \
       public:                                                        \
        static_var ## _annotator() {                                  \
          ANNOTATE_BENIGN_RACE(&static_var,                           \
            # static_var ": " description);                           \
        }                                                             \
      };                                                              \
      static static_var ## _annotator the ## static_var ## _annotator;\
    }
#else 

  #define ANNOTATE_UNPROTECTED_READ(x) (x)
  #define ANNOTATE_BENIGN_RACE_STATIC(static_var, description)  

#endif 

extern "C" int RunningOnValgrind();


#endif  
