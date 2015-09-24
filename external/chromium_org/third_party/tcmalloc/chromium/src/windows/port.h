/* Copyright (c) 2007, Google Inc.
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
 * Author: Craig Silverstein
 *
 * These are some portability typedefs and defines to make it a bit
 * easier to compile this code under VC++.
 *
 * Several of these are taken from glib:
 *    http://developer.gnome.org/doc/API/glib/glib-windows-compatability-functions.html
 */

#ifndef GOOGLE_BASE_WINDOWS_H_
#define GOOGLE_BASE_WINDOWS_H_

#if !defined(GOOGLE_PERFTOOLS_WINDOWS_CONFIG_H_) && \
    !defined(GOOGLE_PERFTOOLS_WINDOWS_MINGW_H_)
# error "port.h should only be included from config.h or mingw.h"
#endif

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX             
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  
#endif
#include <windows.h>
#include <io.h>              
#include <direct.h>          
#include <process.h>         
#include <limits.h>          
#include <stdarg.h>          
#include <stdio.h>           
#include <sys/types.h>       
#include <assert.h>
#include <stdlib.h>          

#ifdef _MSC_VER
#pragma warning(disable:4018 4244 4288 4267 4290 4996 4146)
#endif

#ifndef __cplusplus
# if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#  ifdef _MSC_VER
#    define inline __inline
#  else
#    define inline static
#  endif
# endif
#endif

#ifdef __cplusplus
# define EXTERN_C  extern "C"
#else
# define EXTERN_C  extern
#endif


#ifndef HAVE_STDINT_H
#ifndef HAVE___INT64    
# error  Do not know how to set up type aliases.  Edit port.h for your system.
#endif

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#endif  

#ifdef _MSC_VER
typedef intptr_t ssize_t;
#endif


#ifndef HAVE_PTHREAD   
typedef DWORD pthread_t;
typedef DWORD pthread_key_t;
typedef LONG pthread_once_t;
enum { PTHREAD_ONCE_INIT = 0 };   

inline pthread_t pthread_self(void) {
  return GetCurrentThreadId();
}

#ifdef __cplusplus
inline bool pthread_equal(pthread_t left, pthread_t right) {
  return left == right;
}

EXTERN_C pthread_key_t PthreadKeyCreate(void (*destr_fn)(void*));  

inline int perftools_pthread_key_create(pthread_key_t *pkey,
                                        void (*destructor)(void*)) {
  pthread_key_t key = PthreadKeyCreate(destructor);
  if (key != TLS_OUT_OF_INDEXES) {
    *(pkey) = key;
    return 0;
  } else {
    return GetLastError();
  }
}

inline void* perftools_pthread_getspecific(DWORD key) {
  DWORD err = GetLastError();
  void* rv = TlsGetValue(key);
  if (err) SetLastError(err);
  return rv;
}

inline int perftools_pthread_setspecific(pthread_key_t key, const void *value) {
  if (TlsSetValue(key, (LPVOID)value))
    return 0;
  else
    return GetLastError();
}

EXTERN_C int perftools_pthread_once(pthread_once_t *once_control,
                                    void (*init_routine)(void));

#endif  
#endif  

inline void sched_yield(void) {
  Sleep(0);
}

#define __thread __declspec(thread)

#if 0
#ifdef __cplusplus
class SpinLock {
 public:
  SpinLock() : initialize_token_(PTHREAD_ONCE_INIT) {}
  
  enum StaticInitializer { LINKER_INITIALIZED };
  explicit SpinLock(StaticInitializer) : initialize_token_(PTHREAD_ONCE_INIT) {
    perftools_pthread_once(&initialize_token_, InitializeMutex);
  }

  
  
  
  
  
  
  

  void Lock() {
    
    
    
    
    
    
    
    
    
    perftools_pthread_once(&initialize_token_, InitializeMutex);
    EnterCriticalSection(&mutex_);
  }
  void Unlock() {
    LeaveCriticalSection(&mutex_);
  }

  
  inline bool IsHeld() const {
    
    
    
    return true;
  }
 private:
  void InitializeMutex() { InitializeCriticalSection(&mutex_); }

  pthread_once_t initialize_token_;
  CRITICAL_SECTION mutex_;
};

class SpinLockHolder {  
 private:
  SpinLock* lock_;
 public:
  inline explicit SpinLockHolder(SpinLock* l) : lock_(l) { l->Lock(); }
  inline ~SpinLockHolder() { lock_->Unlock(); }
};
#endif  

#define BASE_SPINLOCK_H_ 1

#endif  


#ifndef HAVE_MMAP   
#define MAP_FAILED  0
#define MREMAP_FIXED  2  
#define PROT_READ      PAGE_READWRITE
#define PROT_WRITE     PAGE_READWRITE
#define MAP_ANONYMOUS  MEM_RESERVE
#define MAP_PRIVATE    MEM_COMMIT
#define MAP_SHARED     MEM_RESERVE   

#if __STDC__ && !defined(__MINGW32__)
typedef _off_t off_t;
#endif

inline void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset) {
  if (addr == NULL && fd == -1 && offset == 0 &&
      prot == (PROT_READ|PROT_WRITE) && flags == (MAP_PRIVATE|MAP_ANONYMOUS)) {
    return VirtualAlloc(0, length, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  } else {
    return NULL;
  }
}

inline int munmap(void *addr, size_t length) {
  return VirtualFree(addr, 0, MEM_RELEASE) ? 0 : -1;
}
#endif  

inline void *sbrk(intptr_t increment) {
  
  return (void*)-1;
}



#if defined(_MSC_VER) && _MSC_VER >= 1400
inline int perftools_vsnprintf(char *str, size_t size, const char *format,
                               va_list ap) {
  return vsnprintf_s(str, size, _TRUNCATE, format, ap);
}
#else
inline int perftools_vsnprintf(char *str, size_t size, const char *format,
                               va_list ap) {
  if (size == 0)        
    return -1;        
  str[size-1] = '\0';
  return _vsnprintf(str, size-1, format, ap);
}
#endif

#ifndef HAVE_SNPRINTF
inline int snprintf(char *str, size_t size, const char *format, ...) {
  va_list ap;
  int r;
  va_start(ap, format);
  r = perftools_vsnprintf(str, size, format, ap);
  va_end(ap);
  return r;
}
#endif

#define PRIx64  "I64x"
#define SCNx64  "I64x"
#define PRId64  "I64d"
#define SCNd64  "I64d"
#define PRIu64  "I64u"
#ifdef _WIN64
# define PRIuPTR "llu"
# define PRIxPTR "llx"
#else
# define PRIuPTR "lu"
# define PRIxPTR "lx"
#endif


#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#ifndef __MINGW32__
enum { STDIN_FILENO = 0, STDOUT_FILENO = 1, STDERR_FILENO = 2 };
#endif
#ifndef O_RDONLY
#define O_RDONLY  _O_RDONLY
#endif

#if __STDC__ && !defined(__MINGW32__)
inline int access(const char *pathname, int mode) {
  return _access(pathname, mode);
}
inline int open(const char *pathname, int flags, int mode = 0) {
  return _open(pathname, flags, mode);
}
inline int close(int fd) {
  return _close(fd);
}
inline ssize_t read(int fd, void *buf, size_t count) {
  return _read(fd, buf, count);
}
inline ssize_t write(int fd, const void *buf, size_t count) {
  return _write(fd, buf, count);
}
inline off_t lseek(int fd, off_t offset, int whence) {
  return _lseek(fd, offset, whence);
}
inline char *getcwd(char *buf, size_t size) {
  return _getcwd(buf, size);
}
inline int mkdir(const char *pathname, int) {
  return _mkdir(pathname);
}

inline FILE *popen(const char *command, const char *type) {
  return _popen(command, type);
}
inline int pclose(FILE *stream) {
  return _pclose(stream);
}
#endif

EXTERN_C PERFTOOLS_DLL_DECL void WriteToStderr(const char* buf, int len);


typedef int pid_t;
#if __STDC__ && !defined(__MINGW32__)
inline pid_t getpid(void) { return _getpid(); }
#endif
inline pid_t getppid(void) { return 0; }

inline int poll(struct pollfd* fds, int nfds, int timeout) {
  assert(fds == NULL);
  assert(nfds == 0);
  Sleep(timeout);
  return 0;
}

EXTERN_C int getpagesize();   


inline void srandom(unsigned int seed) { srand(seed); }
inline long random(void) { return rand(); }
inline unsigned int sleep(unsigned int seconds) {
  Sleep(seconds * 1000);
  return 0;
}

#ifndef _TIMESPEC_DEFINED
struct timespec {
  int tv_sec;
  int tv_nsec;
};
#endif

inline int nanosleep(const struct timespec *req, struct timespec *rem) {
  Sleep(req->tv_sec * 1000 + req->tv_nsec / 1000000);
  return 0;
}

#ifndef __MINGW32__
#if _MSC_VER < 1800  
inline long long int strtoll(const char *nptr, char **endptr, int base) {
    return _strtoi64(nptr, endptr, base);
}
inline unsigned long long int strtoull(const char *nptr, char **endptr,
                                       int base) {
    return _strtoui64(nptr, endptr, base);
}
inline long long int strtoq(const char *nptr, char **endptr, int base) {
    return _strtoi64(nptr, endptr, base);
}
#endif
inline unsigned long long int strtouq(const char *nptr, char **endptr,
                                      int base) {
    return _strtoui64(nptr, endptr, base);
}
inline long long atoll(const char *nptr) {
  return _atoi64(nptr);
}
#endif

#define __THROW throw()


extern void PatchWindowsFunctions();


#define GOOGLE_MAYBE_THREADS_H_ 1


#endif  

#undef inline
#undef EXTERN_C

#endif  
