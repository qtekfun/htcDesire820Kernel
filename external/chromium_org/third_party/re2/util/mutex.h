// Copyright 2007 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_UTIL_MUTEX_H_
#define RE2_UTIL_MUTEX_H_

namespace re2 {

#ifndef WIN32
#define HAVE_PTHREAD 1
#define HAVE_RWLOCK 1
#endif

#if defined(NO_THREADS)
  typedef int MutexType;      
#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)
  
  
  
  
# ifdef __linux__
#   undef _XOPEN_SOURCE
#   define _XOPEN_SOURCE 500  
# endif
# include <pthread.h>
  typedef pthread_rwlock_t MutexType;
#elif defined(HAVE_PTHREAD)
# include <pthread.h>
  typedef pthread_mutex_t MutexType;
#elif defined(WIN32)
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN  
# endif
# ifdef GMUTEX_TRYLOCK
  
  
  
#   ifndef _WIN32_WINNT
#     define _WIN32_WINNT 0x0400
#   endif
# endif
# include <windows.h>
  typedef CRITICAL_SECTION MutexType;
#else
# error Need to implement mutex.h for your architecture, or #define NO_THREADS
#endif

class Mutex {
 public:
  
  inline Mutex();

  
  inline ~Mutex();

  inline void Lock();    
  inline void Unlock();  
  inline bool TryLock(); 
  
  
  
  
  inline void ReaderLock();   
  inline void ReaderUnlock(); 
  inline void WriterLock() { Lock(); }     
  inline void WriterUnlock() { Unlock(); } 
  inline void AssertHeld() { }

 private:
  MutexType mutex_;

  
  Mutex(Mutex *ignored);
  
  Mutex(const Mutex&);
  void operator=(const Mutex&);
};

#if defined(NO_THREADS)

#include <assert.h>

Mutex::Mutex() : mutex_(0) { }
Mutex::~Mutex()            { assert(mutex_ == 0); }
void Mutex::Lock()         { assert(--mutex_ == -1); }
void Mutex::Unlock()       { assert(mutex_++ == -1); }
bool Mutex::TryLock()      { if (mutex_) return false; Lock(); return true; }
void Mutex::ReaderLock()   { assert(++mutex_ > 0); }
void Mutex::ReaderUnlock() { assert(mutex_-- > 0); }

#elif defined(HAVE_PTHREAD) && defined(HAVE_RWLOCK)

#include <stdlib.h>      
#define SAFE_PTHREAD(fncall)  do { if ((fncall) != 0) abort(); } while (0)

Mutex::Mutex()             { SAFE_PTHREAD(pthread_rwlock_init(&mutex_, NULL)); }
Mutex::~Mutex()            { SAFE_PTHREAD(pthread_rwlock_destroy(&mutex_)); }
void Mutex::Lock()         { SAFE_PTHREAD(pthread_rwlock_wrlock(&mutex_)); }
void Mutex::Unlock()       { SAFE_PTHREAD(pthread_rwlock_unlock(&mutex_)); }
bool Mutex::TryLock()      { return pthread_rwlock_trywrlock(&mutex_) == 0; }
void Mutex::ReaderLock()   { SAFE_PTHREAD(pthread_rwlock_rdlock(&mutex_)); }
void Mutex::ReaderUnlock() { SAFE_PTHREAD(pthread_rwlock_unlock(&mutex_)); }

#undef SAFE_PTHREAD

#elif defined(HAVE_PTHREAD)

#include <stdlib.h>      
#define SAFE_PTHREAD(fncall)  do { if ((fncall) != 0) abort(); } while (0)

Mutex::Mutex()             { SAFE_PTHREAD(pthread_mutex_init(&mutex_, NULL)); }
Mutex::~Mutex()            { SAFE_PTHREAD(pthread_mutex_destroy(&mutex_)); }
void Mutex::Lock()         { SAFE_PTHREAD(pthread_mutex_lock(&mutex_)); }
void Mutex::Unlock()       { SAFE_PTHREAD(pthread_mutex_unlock(&mutex_)); }
bool Mutex::TryLock()      { return pthread_mutex_trylock(&mutex_) == 0; }
void Mutex::ReaderLock()   { Lock(); }      
void Mutex::ReaderUnlock() { Unlock(); }
#undef SAFE_PTHREAD

#elif defined(WIN32)

Mutex::Mutex()             { InitializeCriticalSection(&mutex_); }
Mutex::~Mutex()            { DeleteCriticalSection(&mutex_); }
void Mutex::Lock()         { EnterCriticalSection(&mutex_); }
void Mutex::Unlock()       { LeaveCriticalSection(&mutex_); }
bool Mutex::TryLock()      { return TryEnterCriticalSection(&mutex_) != 0; }
void Mutex::ReaderLock()   { Lock(); }      
void Mutex::ReaderUnlock() { Unlock(); }

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

#ifdef HAVE_PTHREAD

#define GLOBAL_MUTEX(name) \
	static pthread_mutex_t (name) = PTHREAD_MUTEX_INITIALIZER
#define GLOBAL_MUTEX_LOCK(name) \
	pthread_mutex_lock(&(name))
#define GLOBAL_MUTEX_UNLOCK(name) \
	pthread_mutex_unlock(&(name))

#else

#define GLOBAL_MUTEX(name) \
	static Mutex name
#define GLOBAL_MUTEX_LOCK(name) \
	name.Lock()
#define GLOBAL_MUTEX_UNLOCK(name) \
	name.Unlock()

#endif

}  

#endif  
