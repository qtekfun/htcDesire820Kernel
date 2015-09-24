/* Copyright (c) 2008, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Kostya Serebryany
 */


#ifndef BASE_DYNAMIC_ANNOTATIONS_H_
#define BASE_DYNAMIC_ANNOTATIONS_H_

#ifndef DYNAMIC_ANNOTATIONS_ENABLED
# define DYNAMIC_ANNOTATIONS_ENABLED 0
#endif

#if DYNAMIC_ANNOTATIONS_ENABLED != 0


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

  
  #define ANNOTATE_UNPUBLISH_MEMORY_RANGE(pointer, size) \
    AnnotateUnpublishMemoryRange(__FILE__, __LINE__, pointer, size)

  
  #define ANNOTATE_SWAP_MEMORY_RANGE(pointer, size)   \
    do {                                              \
      ANNOTATE_UNPUBLISH_MEMORY_RANGE(pointer, size); \
      ANNOTATE_PUBLISH_MEMORY_RANGE(pointer, size);   \
    } while (0)

  #define ANNOTATE_PURE_HAPPENS_BEFORE_MUTEX(mu) \
    AnnotateMutexIsUsedAsCondVar(__FILE__, __LINE__, mu)

  
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


  #define ANNOTATE_BENIGN_RACE(pointer, description) \
    AnnotateBenignRaceSized(__FILE__, __LINE__, pointer, \
                            sizeof(*(pointer)), description)

  #define ANNOTATE_BENIGN_RACE_SIZED(address, size, description) \
    AnnotateBenignRaceSized(__FILE__, __LINE__, address, size, description)

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

  #define ANNOTATE_ENABLE_RACE_DETECTION(enable) \
    AnnotateEnableRaceDetection(__FILE__, __LINE__, enable)


  
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


  #define ANNOTATE_BARRIER_INIT(barrier, count, reinitialization_allowed) \
    AnnotateBarrierInit(__FILE__, __LINE__, barrier, count, \
                        reinitialization_allowed)

  
  #define ANNOTATE_BARRIER_WAIT_BEFORE(barrier) \
    AnnotateBarrierWaitBefore(__FILE__, __LINE__, barrier)

  
  #define ANNOTATE_BARRIER_WAIT_AFTER(barrier) \
    AnnotateBarrierWaitAfter(__FILE__, __LINE__, barrier)

  
  #define ANNOTATE_BARRIER_DESTROY(barrier) \
    AnnotateBarrierDestroy(__FILE__, __LINE__, barrier)


  #define ANNOTATE_EXPECT_RACE(address, description) \
    AnnotateExpectRace(__FILE__, __LINE__, address, description)

  
  #define ANNOTATE_NO_OP(arg) \
    AnnotateNoOp(__FILE__, __LINE__, arg)

  #define ANNOTATE_FLUSH_STATE() \
    AnnotateFlushState(__FILE__, __LINE__)


#else  

  #define ANNOTATE_RWLOCK_CREATE(lock) 
  #define ANNOTATE_RWLOCK_DESTROY(lock) 
  #define ANNOTATE_RWLOCK_ACQUIRED(lock, is_w) 
  #define ANNOTATE_RWLOCK_RELEASED(lock, is_w) 
  #define ANNOTATE_BARRIER_INIT(barrier, count, reinitialization_allowed) 
  #define ANNOTATE_BARRIER_WAIT_BEFORE(barrier) 
  #define ANNOTATE_BARRIER_WAIT_AFTER(barrier) 
  #define ANNOTATE_BARRIER_DESTROY(barrier) 
  #define ANNOTATE_CONDVAR_LOCK_WAIT(cv, lock) 
  #define ANNOTATE_CONDVAR_WAIT(cv) 
  #define ANNOTATE_CONDVAR_SIGNAL(cv) 
  #define ANNOTATE_CONDVAR_SIGNAL_ALL(cv) 
  #define ANNOTATE_HAPPENS_BEFORE(obj) 
  #define ANNOTATE_HAPPENS_AFTER(obj) 
  #define ANNOTATE_PUBLISH_MEMORY_RANGE(address, size) 
  #define ANNOTATE_UNPUBLISH_MEMORY_RANGE(address, size)  
  #define ANNOTATE_SWAP_MEMORY_RANGE(address, size)  
  #define ANNOTATE_PCQ_CREATE(pcq) 
  #define ANNOTATE_PCQ_DESTROY(pcq) 
  #define ANNOTATE_PCQ_PUT(pcq) 
  #define ANNOTATE_PCQ_GET(pcq) 
  #define ANNOTATE_NEW_MEMORY(address, size) 
  #define ANNOTATE_EXPECT_RACE(address, description) 
  #define ANNOTATE_BENIGN_RACE(address, description) 
  #define ANNOTATE_BENIGN_RACE_SIZED(address, size, description) 
  #define ANNOTATE_PURE_HAPPENS_BEFORE_MUTEX(mu) 
  #define ANNOTATE_MUTEX_IS_USED_AS_CONDVAR(mu) 
  #define ANNOTATE_TRACE_MEMORY(arg) 
  #define ANNOTATE_THREAD_NAME(name) 
  #define ANNOTATE_IGNORE_READS_BEGIN() 
  #define ANNOTATE_IGNORE_READS_END() 
  #define ANNOTATE_IGNORE_WRITES_BEGIN() 
  #define ANNOTATE_IGNORE_WRITES_END() 
  #define ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN() 
  #define ANNOTATE_IGNORE_READS_AND_WRITES_END() 
  #define ANNOTATE_ENABLE_RACE_DETECTION(enable) 
  #define ANNOTATE_NO_OP(arg) 
  #define ANNOTATE_FLUSH_STATE() 

#endif  


#define ANNOTALYSIS_STATIC_INLINE
#define ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY ;
#define ANNOTALYSIS_IGNORE_READS_BEGIN
#define ANNOTALYSIS_IGNORE_READS_END
#define ANNOTALYSIS_IGNORE_WRITES_BEGIN
#define ANNOTALYSIS_IGNORE_WRITES_END
#define ANNOTALYSIS_UNPROTECTED_READ

#if defined(__GNUC__) && (!defined(SWIG)) && (!defined(__clang__)) && \
    defined(__SUPPORT_TS_ANNOTATION__) && defined(__SUPPORT_DYN_ANNOTATION__)

#if DYNAMIC_ANNOTATIONS_ENABLED == 0
#define ANNOTALYSIS_ONLY 1
#undef ANNOTALYSIS_STATIC_INLINE
#define ANNOTALYSIS_STATIC_INLINE static inline
#undef ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY
#define ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY { (void)file; (void)line; }
#endif

#if defined(__SUPPORT_TS_ANNOTATION__) && defined(__SUPPORT_DYN_ANNOTATION__)
#undef  ANNOTALYSIS_IGNORE_READS_BEGIN
#define ANNOTALYSIS_IGNORE_READS_BEGIN  __attribute__ ((ignore_reads_begin))
#undef  ANNOTALYSIS_IGNORE_READS_END
#define ANNOTALYSIS_IGNORE_READS_END    __attribute__ ((ignore_reads_end))
#undef  ANNOTALYSIS_IGNORE_WRITES_BEGIN
#define ANNOTALYSIS_IGNORE_WRITES_BEGIN __attribute__ ((ignore_writes_begin))
#undef  ANNOTALYSIS_IGNORE_WRITES_END
#define ANNOTALYSIS_IGNORE_WRITES_END   __attribute__ ((ignore_writes_end))
#undef  ANNOTALYSIS_UNPROTECTED_READ
#define ANNOTALYSIS_UNPROTECTED_READ    __attribute__ ((unprotected_read))
#endif

#endif 

#ifdef __cplusplus
extern "C" {
#endif
void AnnotateRWLockCreate(const char *file, int line,
                          const volatile void *lock);
void AnnotateRWLockDestroy(const char *file, int line,
                           const volatile void *lock);
void AnnotateRWLockAcquired(const char *file, int line,
                            const volatile void *lock, long is_w);
void AnnotateRWLockReleased(const char *file, int line,
                            const volatile void *lock, long is_w);
void AnnotateBarrierInit(const char *file, int line,
                         const volatile void *barrier, long count,
                         long reinitialization_allowed);
void AnnotateBarrierWaitBefore(const char *file, int line,
                               const volatile void *barrier);
void AnnotateBarrierWaitAfter(const char *file, int line,
                              const volatile void *barrier);
void AnnotateBarrierDestroy(const char *file, int line,
                            const volatile void *barrier);
void AnnotateCondVarWait(const char *file, int line,
                         const volatile void *cv,
                         const volatile void *lock);
void AnnotateCondVarSignal(const char *file, int line,
                           const volatile void *cv);
void AnnotateCondVarSignalAll(const char *file, int line,
                              const volatile void *cv);
void AnnotatePublishMemoryRange(const char *file, int line,
                                const volatile void *address,
                                long size);
void AnnotateUnpublishMemoryRange(const char *file, int line,
                                  const volatile void *address,
                                  long size);
void AnnotatePCQCreate(const char *file, int line,
                       const volatile void *pcq);
void AnnotatePCQDestroy(const char *file, int line,
                        const volatile void *pcq);
void AnnotatePCQPut(const char *file, int line,
                    const volatile void *pcq);
void AnnotatePCQGet(const char *file, int line,
                    const volatile void *pcq);
void AnnotateNewMemory(const char *file, int line,
                       const volatile void *address,
                       long size);
void AnnotateExpectRace(const char *file, int line,
                        const volatile void *address,
                        const char *description);
void AnnotateBenignRace(const char *file, int line,
                        const volatile void *address,
                        const char *description);
void AnnotateBenignRaceSized(const char *file, int line,
                        const volatile void *address,
                        long size,
                        const char *description);
void AnnotateMutexIsUsedAsCondVar(const char *file, int line,
                                  const volatile void *mu);
void AnnotateTraceMemory(const char *file, int line,
                         const volatile void *arg);
void AnnotateThreadName(const char *file, int line,
                        const char *name);
ANNOTALYSIS_STATIC_INLINE
void AnnotateIgnoreReadsBegin(const char *file, int line)
    ANNOTALYSIS_IGNORE_READS_BEGIN ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY
ANNOTALYSIS_STATIC_INLINE
void AnnotateIgnoreReadsEnd(const char *file, int line)
    ANNOTALYSIS_IGNORE_READS_END ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY
ANNOTALYSIS_STATIC_INLINE
void AnnotateIgnoreWritesBegin(const char *file, int line)
    ANNOTALYSIS_IGNORE_WRITES_BEGIN ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY
ANNOTALYSIS_STATIC_INLINE
void AnnotateIgnoreWritesEnd(const char *file, int line)
    ANNOTALYSIS_IGNORE_WRITES_END ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY
void AnnotateEnableRaceDetection(const char *file, int line, int enable);
void AnnotateNoOp(const char *file, int line,
                  const volatile void *arg);
void AnnotateFlushState(const char *file, int line);

int RunningOnValgrind(void);

double ValgrindSlowdown(void);

#ifdef __cplusplus
}
#endif

#if DYNAMIC_ANNOTATIONS_ENABLED != 0 && defined(__cplusplus)

  template <class T>
  inline T ANNOTATE_UNPROTECTED_READ(const volatile T &x)
      ANNOTALYSIS_UNPROTECTED_READ {
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
          ANNOTATE_BENIGN_RACE_SIZED(&static_var,                     \
                                      sizeof(static_var),             \
            # static_var ": " description);                           \
        }                                                             \
      };                                                              \
      static static_var ## _annotator the ## static_var ## _annotator;\
    }
#else 

  #define ANNOTATE_UNPROTECTED_READ(x) (x)
  #define ANNOTATE_BENIGN_RACE_STATIC(static_var, description)  

#endif 


#ifdef ANNOTALYSIS_ONLY

  #undef ANNOTALYSIS_ONLY

  
  #undef ANNOTATE_IGNORE_READS_BEGIN
  #define ANNOTATE_IGNORE_READS_BEGIN()           \
    AnnotateIgnoreReadsBegin(__FILE__, __LINE__)

  #undef ANNOTATE_IGNORE_READS_END
  #define ANNOTATE_IGNORE_READS_END()             \
    AnnotateIgnoreReadsEnd(__FILE__, __LINE__)

  #undef ANNOTATE_IGNORE_WRITES_BEGIN
  #define ANNOTATE_IGNORE_WRITES_BEGIN()          \
    AnnotateIgnoreWritesBegin(__FILE__, __LINE__)

  #undef ANNOTATE_IGNORE_WRITES_END
  #define ANNOTATE_IGNORE_WRITES_END()            \
    AnnotateIgnoreWritesEnd(__FILE__, __LINE__)

  #undef ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN
  #define ANNOTATE_IGNORE_READS_AND_WRITES_BEGIN()       \
    do {                                                 \
      ANNOTATE_IGNORE_READS_BEGIN();                     \
      ANNOTATE_IGNORE_WRITES_BEGIN();                    \
    }while(0)                                            \

  #undef ANNOTATE_IGNORE_READS_AND_WRITES_END
  #define ANNOTATE_IGNORE_READS_AND_WRITES_END()  \
    do {                                          \
      ANNOTATE_IGNORE_WRITES_END();               \
      ANNOTATE_IGNORE_READS_END();                \
    }while(0)                                     \

  #if defined(__cplusplus)
    #undef ANNOTATE_UNPROTECTED_READ
    template <class T>
    inline T ANNOTATE_UNPROTECTED_READ(const volatile T &x)
         ANNOTALYSIS_UNPROTECTED_READ {
      ANNOTATE_IGNORE_READS_BEGIN();
      T res = x;
      ANNOTATE_IGNORE_READS_END();
      return res;
    }
  #endif 

#endif 

#undef ANNOTALYSIS_STATIC_INLINE
#undef ANNOTALYSIS_SEMICOLON_OR_EMPTY_BODY

#endif  
