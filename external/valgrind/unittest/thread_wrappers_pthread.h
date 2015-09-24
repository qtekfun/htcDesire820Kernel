/*
  This file is part of ThreadSanitizer, a dynamic data race detector.

  Copyright (C) 2008-2009 Google Inc
     opensource@google.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307, USA.

  The GNU General Public License is contained in the file COPYING.
*/


#ifndef THREAD_WRAPPERS_PTHREADS_H_
#define THREAD_WRAPPERS_PTHREADS_H_

#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>  
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define NOINLINE   __attribute__ ((noinline))
#define ALIGNED(X) __attribute__ ((aligned (X)))

static const bool kMallocUsesMutex = false;

#ifndef __APPLE__
  
  #include <malloc.h> 

  #ifdef ANDROID
  #define NO_BARRIER
  #define NO_SPINLOCK
  #endif

  
  #if !defined(ANDROID) || defined(__ANDROID__)
  static int AtomicIncrement(volatile int *value, int increment) {
    return __sync_add_and_fetch(value, increment);
  }
  #else
  static int AtomicIncrement(volatile int *value, int increment) {
    static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    ANNOTATE_NOT_HAPPENS_BEFORE_MUTEX(&mu);
    pthread_mutex_lock(&mu);
    int result = *value += increment;
    pthread_mutex_unlock(&mu);
    return result;
  }
  #endif


  #ifdef ANDROID
    #undef TLS
  #else
    #define TLS __thread
  #endif

#else
  
  #include <libkern/OSAtomic.h>
  #define NO_BARRIER
  #define NO_UNNAMED_SEM
  #undef TLS
  #define NO_SPINLOCK

  static int AtomicIncrement(volatile int *value, int increment) {
    return OSAtomicAdd32(increment, value);
  }

  
  #define memalign(A,B) malloc(B)
#ifndef OS_darwin_10
  
  static int posix_memalign(void **out, size_t al, size_t size) {
    *out = memalign(al, size);
    return (*out == 0);
  }
#endif
#endif


static int GetTimeInMs() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return (int)(now.tv_sec * 1000 + now.tv_usec / 1000);
}

static inline void timeval2timespec(timeval *const tv,
                                     timespec *ts,
                                     int64_t offset_milli) {
  const int64_t ten_9 = 1000000000LL;
  const int64_t ten_6 = 1000000LL;
  const int64_t ten_3 = 1000LL;
  int64_t now_nsec = (int64_t)tv->tv_sec * ten_9;
  now_nsec += (int64_t)tv->tv_usec * ten_3;
  int64_t then_nsec = now_nsec + offset_milli * ten_6;
  ts->tv_sec  = then_nsec / ten_9;
  ts->tv_nsec = then_nsec % ten_9;
}

class Mutex {
  friend class CondVar;
 public:
  Mutex() {
    CHECK(0 == pthread_mutex_init(&mu_, NULL));
    CHECK(0 == pthread_cond_init(&cv_, NULL));
    signal_at_unlock_ = false;
  }
  ~Mutex() {
    CHECK(0 == pthread_cond_destroy(&cv_));
    CHECK(0 == pthread_mutex_destroy(&mu_));
  }
  void Lock()          { CHECK(0 == pthread_mutex_lock(&mu_));}
  bool TryLock()       { return (0 == pthread_mutex_trylock(&mu_));}
  void Unlock() {
    ANNOTATE_HAPPENS_BEFORE(this);
    if (signal_at_unlock_) {
      CHECK(0 == pthread_cond_signal(&cv_));
    }
    CHECK(0 == pthread_mutex_unlock(&mu_));
  }
  void ReaderLock()    { Lock(); }
  bool ReaderTryLock() { return TryLock();}
  void ReaderUnlock()  { Unlock(); }

  void LockWhen(Condition cond)            { Lock(); WaitLoop(cond); }
  void ReaderLockWhen(Condition cond)      { Lock(); WaitLoop(cond); }
  void Await(Condition cond)               { WaitLoop(cond); }

  bool ReaderLockWhenWithTimeout(Condition cond, int millis)
    { Lock(); return WaitLoopWithTimeout(cond, millis); }
  bool LockWhenWithTimeout(Condition cond, int millis)
    { Lock(); return WaitLoopWithTimeout(cond, millis); }
  bool AwaitWithTimeout(Condition cond, int millis)
    { return WaitLoopWithTimeout(cond, millis); }

 private:

  void WaitLoop(Condition cond) {
    signal_at_unlock_ = true;
    while(cond.Eval() == false) {
      pthread_cond_wait(&cv_, &mu_);
    }
    ANNOTATE_HAPPENS_AFTER(this);
  }

  bool WaitLoopWithTimeout(Condition cond, int millis) {
    struct timeval now;
    struct timespec timeout;
    int retcode = 0;
    gettimeofday(&now, NULL);
    timeval2timespec(&now, &timeout, millis);

    signal_at_unlock_ = true;

    while (cond.Eval() == false && retcode == 0) {
      retcode = pthread_cond_timedwait(&cv_, &mu_, &timeout);
    }
    if(retcode == 0) {
      ANNOTATE_HAPPENS_AFTER(this);
    }
    return cond.Eval();
  }

  pthread_mutex_t mu_;  
  pthread_cond_t  cv_;
  bool signal_at_unlock_;  
};

class CondVar {
 public:
  CondVar()   { CHECK(0 == pthread_cond_init(&cv_, NULL)); }
  ~CondVar()  { CHECK(0 == pthread_cond_destroy(&cv_)); }
  void Wait(Mutex *mu) { CHECK(0 == pthread_cond_wait(&cv_, &mu->mu_)); }
  bool WaitWithTimeout(Mutex *mu, int millis) {
    struct timeval now;
    struct timespec timeout;
    gettimeofday(&now, NULL);
    timeval2timespec(&now, &timeout, millis);
    return 0 != pthread_cond_timedwait(&cv_, &mu->mu_, &timeout);
  }
  void Signal() { CHECK(0 == pthread_cond_signal(&cv_)); }
  void SignalAll() { CHECK(0 == pthread_cond_broadcast(&cv_)); }
 private:
  pthread_cond_t cv_;
};

#define NEEDS_SEPERATE_RW_LOCK
class RWLock {
 public:
  RWLock() { CHECK(0 == pthread_rwlock_init(&mu_, NULL)); }
  ~RWLock() { CHECK(0 == pthread_rwlock_destroy(&mu_)); }
  void Lock() { CHECK(0 == pthread_rwlock_wrlock(&mu_)); }
  void ReaderLock() { CHECK(0 == pthread_rwlock_rdlock(&mu_)); }
  void Unlock() { CHECK(0 == pthread_rwlock_unlock(&mu_)); }
  void ReaderUnlock() { CHECK(0 == pthread_rwlock_unlock(&mu_)); }
  bool TryLock() {
    int res = pthread_rwlock_trywrlock(&mu_);
    if (res != 0) {
      CHECK(EBUSY == res);
    }
    return (res == 0);
  }
  bool ReaderTryLock() {
    int res = pthread_rwlock_tryrdlock(&mu_);
    if (res != 0) {
      CHECK(EBUSY == res);
    }
    return (res == 0);
  }
 private:
  pthread_rwlock_t mu_;
};

class ReaderLockScoped {  
 public:
  ReaderLockScoped(RWLock *mu) : mu_(mu) {
    mu_->ReaderLock();
  }
  ~ReaderLockScoped() {
    mu_->ReaderUnlock();
  }
 private:
  RWLock *mu_;
};

class WriterLockScoped {  
 public:
  WriterLockScoped(RWLock *mu) : mu_(mu) {
    mu_->Lock();
  }
  ~WriterLockScoped() {
    mu_->Unlock();
  }
 private:
  RWLock *mu_;
};

#if !defined(__APPLE__) && !defined(ANDROID)
class SpinLock {
 public:
  SpinLock() {
    CHECK(0 == pthread_spin_init(&mu_, 0));
  }
  ~SpinLock() {
    CHECK(0 == pthread_spin_destroy(&mu_));
  }
  void Lock() {
    CHECK(0 == pthread_spin_lock(&mu_));
  }
  void Unlock() {
    CHECK(0 == pthread_spin_unlock(&mu_));
  }
 private:
  pthread_spinlock_t mu_;
};

#elif defined(__APPLE__)

class SpinLock {
 public:
  
  SpinLock() : mu_(OS_SPINLOCK_INIT) {
    ANNOTATE_RWLOCK_CREATE((void*)&mu_);
  }
  ~SpinLock() {
    ANNOTATE_RWLOCK_DESTROY((void*)&mu_);
  }
  void Lock() {
    OSSpinLockLock(&mu_);
    ANNOTATE_RWLOCK_ACQUIRED((void*)&mu_, 1);
  }
  void Unlock() {
    ANNOTATE_RWLOCK_RELEASED((void*)&mu_, 1);
    OSSpinLockUnlock(&mu_);
  }
 private:
  OSSpinLock mu_;
};
#endif 

class MyThread {
 public:
  typedef void *(*worker_t)(void*);

  MyThread(worker_t worker, void *arg = NULL, const char *name = NULL)
      :w_(worker), arg_(arg), name_(name) {}
  MyThread(void (*worker)(void), void *arg = NULL, const char *name = NULL)
      :w_(reinterpret_cast<worker_t>(worker)), arg_(arg), name_(name) {}
  MyThread(void (*worker)(void *), void *arg = NULL, const char *name = NULL)
      :w_(reinterpret_cast<worker_t>(worker)), arg_(arg), name_(name) {}

  ~MyThread(){ w_ = NULL; arg_ = NULL;}
  void Start() { CHECK(0 == pthread_create(&t_, NULL, (worker_t)ThreadBody, this));}
  void Join()  { CHECK(0 == pthread_join(t_, NULL));}
  pthread_t tid() const { return t_; }
 private:
  static void ThreadBody(MyThread *my_thread) {
    if (my_thread->name_) {
      ANNOTATE_THREAD_NAME(my_thread->name_);
    }
    my_thread->w_(my_thread->arg_);
  }
  pthread_t t_;
  worker_t  w_;
  void     *arg_;
  const char *name_;
};

#ifndef NO_BARRIER
class Barrier{
 public:
  explicit Barrier(int n_threads) {CHECK(0 == pthread_barrier_init(&b_, 0, n_threads));}
  ~Barrier()                      {CHECK(0 == pthread_barrier_destroy(&b_));}
  void Block() {
    
    
    
    pthread_barrier_wait(&b_);
    
  }
 private:
  pthread_barrier_t b_;
};

#endif 

#endif  
