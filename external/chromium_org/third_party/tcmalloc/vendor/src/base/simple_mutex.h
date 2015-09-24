// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLE_MUTEX_H_
#define GOOGLE_MUTEX_H_

#include <config.h>

#if defined(NO_THREADS)
  typedef int MutexType;      
#elif defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__)
# ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN  
# endif
  
  
  
# ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0400
# endif
# include <windows.h>
  typedef CRITICAL_SECTION MutexType;
#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)
  
  
  
  
# ifdef __linux__
#   define _XOPEN_SOURCE 500  
# endif
# include <pthread.h>
  typedef pthread_rwlock_t MutexType;
#elif defined(HAVE_PTHREAD)
# include <pthread.h>
  typedef pthread_mutex_t MutexType;
#else
# error Need to implement mutex.h for your architecture, or #define NO_THREADS
#endif

#include <assert.h>
#include <stdlib.h>      

#define MUTEX_NAMESPACE perftools_mutex_namespace

namespace MUTEX_NAMESPACE {

class Mutex {
 public:
  
  enum LinkerInitialized { LINKER_INITIALIZED };

  
  
  inline Mutex();
  
  
  
  
  inline Mutex(LinkerInitialized);

  
  inline ~Mutex();

  inline void Lock();    
  inline void Unlock();  
  inline bool TryLock(); 
  
  
  
  
  inline void ReaderLock();   
  inline void ReaderUnlock(); 
  inline void WriterLock() { Lock(); }     
  inline void WriterUnlock() { Unlock(); } 

 private:
  MutexType mutex_;
  
  
  
  volatile bool is_safe_;
  
  bool destroy_;

  inline void SetIsSafe() { is_safe_ = true; }

  
  Mutex(Mutex* ) {}
  
  Mutex(const Mutex&);
  void operator=(const Mutex&);
};

#if defined(NO_THREADS)


Mutex::Mutex() : mutex_(0) { }
Mutex::Mutex(Mutex::LinkerInitialized) : mutex_(0) { }
Mutex::~Mutex()            { assert(mutex_ == 0); }
void Mutex::Lock()         { assert(--mutex_ == -1); }
void Mutex::Unlock()       { assert(mutex_++ == -1); }
bool Mutex::TryLock()      { if (mutex_) return false; Lock(); return true; }
void Mutex::ReaderLock()   { assert(++mutex_ > 0); }
void Mutex::ReaderUnlock() { assert(mutex_-- > 0); }

#elif defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__)

Mutex::Mutex() : destroy_(true) {
  InitializeCriticalSection(&mutex_);
  SetIsSafe();
}
Mutex::Mutex(LinkerInitialized) : destroy_(false) {
  InitializeCriticalSection(&mutex_);
  SetIsSafe();
}
Mutex::~Mutex()            { if (destroy_) DeleteCriticalSection(&mutex_); }
void Mutex::Lock()         { if (is_safe_) EnterCriticalSection(&mutex_); }
void Mutex::Unlock()       { if (is_safe_) LeaveCriticalSection(&mutex_); }
bool Mutex::TryLock()      { return is_safe_ ?
                                 TryEnterCriticalSection(&mutex_) != 0 : true; }
void Mutex::ReaderLock()   { Lock(); }      
void Mutex::ReaderUnlock() { Unlock(); }

#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)

#define SAFE_PTHREAD(fncall)  do {     \
  if (is_safe_ && fncall(&mutex_) != 0) abort();                           \
} while (0)

Mutex::Mutex() : destroy_(true) {
  SetIsSafe();
  if (is_safe_ && pthread_rwlock_init(&mutex_, NULL) != 0) abort();
}
Mutex::Mutex(Mutex::LinkerInitialized) : destroy_(false) {
  SetIsSafe();
  if (is_safe_ && pthread_rwlock_init(&mutex_, NULL) != 0) abort();
}
Mutex::~Mutex()       { if (destroy_) SAFE_PTHREAD(pthread_rwlock_destroy); }
void Mutex::Lock()         { SAFE_PTHREAD(pthread_rwlock_wrlock); }
void Mutex::Unlock()       { SAFE_PTHREAD(pthread_rwlock_unlock); }
bool Mutex::TryLock()      { return is_safe_ ?
                               pthread_rwlock_trywrlock(&mutex_) == 0 : true; }
void Mutex::ReaderLock()   { SAFE_PTHREAD(pthread_rwlock_rdlock); }
void Mutex::ReaderUnlock() { SAFE_PTHREAD(pthread_rwlock_unlock); }
#undef SAFE_PTHREAD

#elif defined(HAVE_PTHREAD)

#define SAFE_PTHREAD(fncall)  do {     \
  if (is_safe_ && fncall(&mutex_) != 0) abort();                           \
} while (0)

Mutex::Mutex() : destroy_(true) {
  SetIsSafe();
  if (is_safe_ && pthread_mutex_init(&mutex_, NULL) != 0) abort();
}
Mutex::Mutex(Mutex::LinkerInitialized) : destroy_(false) {
  SetIsSafe();
  if (is_safe_ && pthread_mutex_init(&mutex_, NULL) != 0) abort();
}
Mutex::~Mutex()       { if (destroy_) SAFE_PTHREAD(pthread_mutex_destroy); }
void Mutex::Lock()         { SAFE_PTHREAD(pthread_mutex_lock); }
void Mutex::Unlock()       { SAFE_PTHREAD(pthread_mutex_unlock); }
bool Mutex::TryLock()      { return is_safe_ ?
                                 pthread_mutex_trylock(&mutex_) == 0 : true; }
void Mutex::ReaderLock()   { Lock(); }
void Mutex::ReaderUnlock() { Unlock(); }
#undef SAFE_PTHREAD

#endif


class MutexLock {
 public:
  explicit MutexLock(Mutex *mu) : mu_(mu) { mu_->Lock(); }
  ~MutexLock() { mu_->Unlock(); }
 private:
  Mutex * const mu_;
  
  MutexLock(const MutexLock&);
  void operator=(const MutexLock&);
};

class ReaderMutexLock {
 public:
  explicit ReaderMutexLock(Mutex *mu) : mu_(mu) { mu_->ReaderLock(); }
  ~ReaderMutexLock() { mu_->ReaderUnlock(); }
 private:
  Mutex * const mu_;
  
  ReaderMutexLock(const ReaderMutexLock&);
  void operator=(const ReaderMutexLock&);
};

class WriterMutexLock {
 public:
  explicit WriterMutexLock(Mutex *mu) : mu_(mu) { mu_->WriterLock(); }
  ~WriterMutexLock() { mu_->WriterUnlock(); }
 private:
  Mutex * const mu_;
  
  WriterMutexLock(const WriterMutexLock&);
  void operator=(const WriterMutexLock&);
};

#define MutexLock(x) COMPILE_ASSERT(0, mutex_lock_decl_missing_var_name)
#define ReaderMutexLock(x) COMPILE_ASSERT(0, rmutex_lock_decl_missing_var_name)
#define WriterMutexLock(x) COMPILE_ASSERT(0, wmutex_lock_decl_missing_var_name)

}  

using namespace MUTEX_NAMESPACE;

#undef MUTEX_NAMESPACE

#endif  
