/* Copyright (c) 2005-2008, Google Inc.
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
 * Author: Markus Gutschke
 */

#ifndef SYS_LINUX_SYSCALL_SUPPORT_H
#define SYS_LINUX_SYSCALL_SUPPORT_H

#if (defined(__i386__) || defined(__x86_64__) || defined(__arm__) || \
     defined(__mips__) || defined(__PPC__)) && defined(__linux)

#ifndef SYS_CPLUSPLUS
#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <endian.h>

#ifdef __mips__
#include <sgidefs.h>
#endif

#endif


struct kernel_dirent64 {
  unsigned long long d_ino;
  long long          d_off;
  unsigned short     d_reclen;
  unsigned char      d_type;
  char               d_name[256];
};

struct kernel_dirent {
  long               d_ino;
  long               d_off;
  unsigned short     d_reclen;
  char               d_name[256];
};

struct kernel_timespec {
  long               tv_sec;
  long               tv_nsec;
};

struct kernel_timeval {
  long               tv_sec;
  long               tv_usec;
};

struct kernel_rusage {
  struct kernel_timeval ru_utime;
  struct kernel_timeval ru_stime;
  long               ru_maxrss;
  long               ru_ixrss;
  long               ru_idrss;
  long               ru_isrss;
  long               ru_minflt;
  long               ru_majflt;
  long               ru_nswap;
  long               ru_inblock;
  long               ru_oublock;
  long               ru_msgsnd;
  long               ru_msgrcv;
  long               ru_nsignals;
  long               ru_nvcsw;
  long               ru_nivcsw;
};

#if defined(__i386__) || defined(__arm__) || defined(__PPC__)

struct kernel_old_sigaction {
  union {
    void             (*sa_handler_)(int);
    void             (*sa_sigaction_)(int, siginfo_t *, void *);
  };
  unsigned long      sa_mask;
  unsigned long      sa_flags;
  void               (*sa_restorer)(void);
} __attribute__((packed,aligned(4)));
#elif (defined(__mips__) && _MIPS_SIM == _MIPS_SIM_ABI32)
  #define kernel_old_sigaction kernel_sigaction
#endif

#ifdef __mips__
#define KERNEL_NSIG 128
#else
#define KERNEL_NSIG  64
#endif

struct kernel_sigset_t {
  unsigned long sig[(KERNEL_NSIG + 8*sizeof(unsigned long) - 1)/
                    (8*sizeof(unsigned long))];
};

struct kernel_sigaction {
#ifdef __mips__
  unsigned long      sa_flags;
  union {
    void             (*sa_handler_)(int);
    void             (*sa_sigaction_)(int, siginfo_t *, void *);
  };
  struct kernel_sigset_t sa_mask;
#else
  union {
    void             (*sa_handler_)(int);
    void             (*sa_sigaction_)(int, siginfo_t *, void *);
  };
  unsigned long      sa_flags;
  void               (*sa_restorer)(void);
  struct kernel_sigset_t sa_mask;
#endif
};

#ifdef __mips__
#if _MIPS_SIM == _MIPS_SIM_ABI64
struct kernel_stat {
#else
struct kernel_stat64 {
#endif
  unsigned           st_dev;
  unsigned           __pad0[3];
  unsigned long long st_ino;
  unsigned           st_mode;
  unsigned           st_nlink;
  unsigned           st_uid;
  unsigned           st_gid;
  unsigned           st_rdev;
  unsigned           __pad1[3];
  long long          st_size;
  unsigned           st_atime_;
  unsigned           st_atime_nsec_;
  unsigned           st_mtime_;
  unsigned           st_mtime_nsec_;
  unsigned           st_ctime_;
  unsigned           st_ctime_nsec_;
  unsigned           st_blksize;
  unsigned           __pad2;
  unsigned long long st_blocks;
};
#elif defined __PPC__
struct kernel_stat64 {
  unsigned long long st_dev;
  unsigned long long st_ino;
  unsigned           st_mode;
  unsigned           st_nlink;
  unsigned           st_uid;
  unsigned           st_gid;
  unsigned long long st_rdev;
  unsigned short int __pad2;
  long long          st_size;
  long               st_blksize;
  long long          st_blocks;
  long               st_atime_;
  unsigned long      st_atime_nsec_;
  long               st_mtime_;
  unsigned long      st_mtime_nsec_;
  long               st_ctime_;
  unsigned long      st_ctime_nsec_;
  unsigned long      __unused4;
  unsigned long      __unused5;
};
#else
struct kernel_stat64 {
  unsigned long long st_dev;
  unsigned char      __pad0[4];
  unsigned           __st_ino;
  unsigned           st_mode;
  unsigned           st_nlink;
  unsigned           st_uid;
  unsigned           st_gid;
  unsigned long long st_rdev;
  unsigned char      __pad3[4];
  long long          st_size;
  unsigned           st_blksize;
  unsigned long long st_blocks;
  unsigned           st_atime_;
  unsigned           st_atime_nsec_;
  unsigned           st_mtime_;
  unsigned           st_mtime_nsec_;
  unsigned           st_ctime_;
  unsigned           st_ctime_nsec_;
  unsigned long long st_ino;
};
#endif

#if defined(__i386__) || defined(__arm__)
struct kernel_stat {
  unsigned short     st_dev;
  short              pad1;
  unsigned           st_ino;
  unsigned short     st_mode;
  unsigned short     st_nlink;
  unsigned short     st_uid;
  unsigned short     st_gid;
  unsigned short     st_rdev;
  short              pad2;
  unsigned           st_size;
  unsigned           st_blksize;
  unsigned           st_blocks;
  unsigned           st_atime_;
  unsigned           st_atime_nsec_;
  unsigned           st_mtime_;
  unsigned           st_mtime_nsec_;
  unsigned           st_ctime_;
  unsigned           st_ctime_nsec_;
  unsigned           __unused4;
  unsigned           __unused5;
};
#elif defined(__x86_64__)
struct kernel_stat {
  unsigned long      st_dev;
  unsigned long      st_ino;
  unsigned long      st_nlink;
  unsigned           st_mode;
  unsigned           st_uid;
  unsigned           st_gid;
  unsigned           __pad0;
  unsigned long      st_rdev;
  long               st_size;
  long               st_blksize;
  long               st_blocks;
  unsigned long      st_atime_;
  unsigned long      st_atime_nsec_;
  unsigned long      st_mtime_;
  unsigned long      st_mtime_nsec_;
  unsigned long      st_ctime_;
  unsigned long      st_ctime_nsec_;
  long               __unused[3];
};
#elif defined(__PPC__)
struct kernel_stat {
  unsigned           st_dev;
  unsigned long      st_ino;      
  unsigned long      st_mode;     
  unsigned short     st_nlink;    
  unsigned           st_uid;      
  unsigned           st_gid;      
  unsigned           st_rdev;
  long               st_size;     
  unsigned long      st_blksize;
  unsigned long      st_blocks;
  unsigned long      st_atime_;
  unsigned long      st_atime_nsec_;
  unsigned long      st_mtime_;
  unsigned long      st_mtime_nsec_;
  unsigned long      st_ctime_;
  unsigned long      st_ctime_nsec_;
  unsigned long      __unused4;
  unsigned long      __unused5;
};
#elif (defined(__mips__) && _MIPS_SIM != _MIPS_SIM_ABI64)
struct kernel_stat {
  unsigned           st_dev;
  int                st_pad1[3];
  unsigned           st_ino;
  unsigned           st_mode;
  unsigned           st_nlink;
  unsigned           st_uid;
  unsigned           st_gid;
  unsigned           st_rdev;
  int                st_pad2[2];
  long               st_size;
  int                st_pad3;
  long               st_atime_;
  long               st_atime_nsec_;
  long               st_mtime_;
  long               st_mtime_nsec_;
  long               st_ctime_;
  long               st_ctime_nsec_;
  int                st_blksize;
  int                st_blocks;
  int                st_pad4[14];
};
#endif


#ifndef O_DIRECTORY
#if defined(__arm__)
#define O_DIRECTORY             0040000
#else
#define O_DIRECTORY             0200000
#endif
#endif
#ifndef PR_GET_DUMPABLE
#define PR_GET_DUMPABLE         3
#endif
#ifndef PR_SET_DUMPABLE
#define PR_SET_DUMPABLE         4
#endif
#ifndef AT_FDCWD
#define AT_FDCWD                (-100)
#endif
#ifndef AT_SYMLINK_NOFOLLOW
#define AT_SYMLINK_NOFOLLOW     0x100
#endif
#ifndef AT_REMOVEDIR
#define AT_REMOVEDIR            0x200
#endif
#ifndef MREMAP_FIXED
#define MREMAP_FIXED            2
#endif
#ifndef SA_RESTORER
#define SA_RESTORER             0x04000000
#endif

#if defined(__i386__)
#ifndef __NR_rt_sigaction
#define __NR_rt_sigaction       174
#define __NR_rt_sigprocmask     175
#endif
#ifndef __NR_stat64
#define __NR_stat64             195
#endif
#ifndef __NR_fstat64
#define __NR_fstat64            197
#endif
#ifndef __NR_getdents64
#define __NR_getdents64         220
#endif
#ifndef __NR_gettid
#define __NR_gettid             224
#endif
#ifndef __NR_futex
#define __NR_futex              240
#endif
#ifndef __NR_openat
#define __NR_openat             295
#endif
#ifndef __NR_getcpu
#define __NR_getcpu             318
#endif
#elif defined(__arm__)
#ifndef __syscall
#if defined(__thumb__) || defined(__ARM_EABI__)
#define __SYS_REG(name) register long __sysreg __asm__("r6") = __NR_##name;
#define __SYS_REG_LIST(regs...) [sysreg] "r" (__sysreg) , ##regs
#define __syscall(name) "swi\t0"
#define __syscall_safe(name)                     \
  "push  {r7}\n"                                 \
  "mov   r7,%[sysreg]\n"                         \
  __syscall(name)"\n"                            \
  "pop   {r7}"
#else
#define __SYS_REG(name)
#define __SYS_REG_LIST(regs...) regs
#define __syscall(name) "swi\t" __sys1(__NR_##name) ""
#define __syscall_safe(name) __syscall(name)
#endif
#endif
#ifndef __NR_rt_sigaction
#define __NR_rt_sigaction       (__NR_SYSCALL_BASE + 174)
#define __NR_rt_sigprocmask     (__NR_SYSCALL_BASE + 175)
#endif
#ifndef __NR_stat64
#define __NR_stat64             (__NR_SYSCALL_BASE + 195)
#endif
#ifndef __NR_fstat64
#define __NR_fstat64            (__NR_SYSCALL_BASE + 197)
#endif
#ifndef __NR_getdents64
#define __NR_getdents64         (__NR_SYSCALL_BASE + 217)
#endif
#ifndef __NR_gettid
#define __NR_gettid             (__NR_SYSCALL_BASE + 224)
#endif
#ifndef __NR_futex
#define __NR_futex              (__NR_SYSCALL_BASE + 240)
#endif
#elif defined(__x86_64__)
#ifndef __NR_gettid
#define __NR_gettid             186
#endif
#ifndef __NR_futex
#define __NR_futex              202
#endif
#ifndef __NR_getdents64
#define __NR_getdents64         217
#endif
#ifndef __NR_openat
#define __NR_openat             257
#endif
#elif defined(__mips__)
#if _MIPS_SIM == _MIPS_SIM_ABI32
#ifndef __NR_rt_sigaction
#define __NR_rt_sigaction       (__NR_Linux + 194)
#define __NR_rt_sigprocmask     (__NR_Linux + 195)
#endif
#ifndef __NR_stat64
#define __NR_stat64             (__NR_Linux + 213)
#endif
#ifndef __NR_fstat64
#define __NR_fstat64            (__NR_Linux + 215)
#endif
#ifndef __NR_getdents64
#define __NR_getdents64         (__NR_Linux + 219)
#endif
#ifndef __NR_gettid
#define __NR_gettid             (__NR_Linux + 222)
#endif
#ifndef __NR_futex
#define __NR_futex              (__NR_Linux + 238)
#endif
#ifndef __NR_openat
#define __NR_openat             (__NR_Linux + 288)
#endif
#ifndef __NR_fstatat
#define __NR_fstatat            (__NR_Linux + 293)
#endif
#ifndef __NR_getcpu
#define __NR_getcpu             (__NR_Linux + 312)
#endif
#elif  _MIPS_SIM == _MIPS_SIM_ABI64
#ifndef __NR_gettid
#define __NR_gettid             (__NR_Linux + 178)
#endif
#ifndef __NR_futex
#define __NR_futex              (__NR_Linux + 194)
#endif
#ifndef __NR_openat
#define __NR_openat             (__NR_Linux + 247)
#endif
#ifndef __NR_fstatat
#define __NR_fstatat            (__NR_Linux + 252)
#endif
#ifndef __NR_getcpu
#define __NR_getcpu             (__NR_Linux + 271)
#endif
#else
#ifndef __NR_gettid
#define __NR_gettid             (__NR_Linux + 178)
#endif
#ifndef __NR_futex
#define __NR_futex              (__NR_Linux + 194)
#endif
#ifndef __NR_openat
#define __NR_openat             (__NR_Linux + 251)
#endif
#ifndef __NR_fstatat
#define __NR_fstatat            (__NR_Linux + 256)
#endif
#ifndef __NR_getcpu
#define __NR_getcpu             (__NR_Linux + 275)
#endif
#endif
#elif defined(__PPC__)
#ifndef __NR_rt_sigaction
#define __NR_rt_sigaction       173
#define __NR_rt_sigprocmask     174
#endif
#ifndef __NR_stat64
#define __NR_stat64             195
#endif
#ifndef __NR_fstat64
#define __NR_fstat64            197
#endif
#ifndef __NR_getdents64
#define __NR_getdents64         202
#endif
#ifndef __NR_gettid
#define __NR_gettid             207
#endif
#ifndef __NR_futex
#define __NR_futex              221
#endif
#ifndef __NR_openat
#define __NR_openat             286
#endif
#ifndef __NR_getcpu
#define __NR_getcpu             302
#endif
#endif


#if __BOUNDED_POINTERS__
  #error "Need to port invocations of syscalls for bounded ptrs"
#else
  #undef LSS_ERRNO
  #ifdef SYS_ERRNO
    #define LSS_ERRNO SYS_ERRNO
  #else
    #define LSS_ERRNO errno
  #endif

  #undef LSS_INLINE
  #ifdef SYS_INLINE
    #define LSS_INLINE SYS_INLINE
  #else
    #define LSS_INLINE static inline
  #endif

  #undef LSS_NAME
  #ifndef SYS_PREFIX
    #define LSS_NAME(name) sys_##name
  #elif SYS_PREFIX < 0
    #define LSS_NAME(name) name
  #elif SYS_PREFIX == 0
    #define LSS_NAME(name) sys0_##name
  #elif SYS_PREFIX == 1
    #define LSS_NAME(name) sys1_##name
  #elif SYS_PREFIX == 2
    #define LSS_NAME(name) sys2_##name
  #elif SYS_PREFIX == 3
    #define LSS_NAME(name) sys3_##name
  #elif SYS_PREFIX == 4
    #define LSS_NAME(name) sys4_##name
  #elif SYS_PREFIX == 5
    #define LSS_NAME(name) sys5_##name
  #elif SYS_PREFIX == 6
    #define LSS_NAME(name) sys6_##name
  #elif SYS_PREFIX == 7
    #define LSS_NAME(name) sys7_##name
  #elif SYS_PREFIX == 8
    #define LSS_NAME(name) sys8_##name
  #elif SYS_PREFIX == 9
    #define LSS_NAME(name) sys9_##name
  #endif

  #undef  LSS_RETURN
  #if (defined(__i386__) || defined(__x86_64__) || defined(__arm__))
  #define LSS_RETURN(type, res)                                               \
    do {                                                                      \
      if ((unsigned long)(res) >= (unsigned long)(-4095)) {                   \
        LSS_ERRNO = -(res);                                                   \
        res = -1;                                                             \
      }                                                                       \
      return (type) (res);                                                    \
    } while (0)
  #elif defined(__mips__)
  #define LSS_RETURN(type, res, err)                                          \
    do {                                                                      \
      if (err) {                                                              \
        LSS_ERRNO = (res);                                                    \
        res = -1;                                                             \
      }                                                                       \
      return (type) (res);                                                    \
    } while (0)
  #elif defined(__PPC__)
  #define LSS_RETURN(type, res, err)                                          \
   do {                                                                       \
     if (err & 0x10000000 ) {                                                 \
       LSS_ERRNO = (res);                                                     \
       res = -1;                                                              \
     }                                                                        \
     return (type) (res);                                                     \
   } while (0)
  #endif
  #if defined(__i386__)
    #if defined(NO_FRAME_POINTER) && (100 * __GNUC__ + __GNUC_MINOR__ >= 404)
      #define CFI_ADJUST_CFA_OFFSET(adjust)                                   \
                  ".cfi_adjust_cfa_offset " #adjust "\n"
    #else
      #define CFI_ADJUST_CFA_OFFSET(adjust) 
    #endif

    #undef  LSS_BODY
    #define LSS_BODY(type,args...)                                            \
      long __res;                                                             \
      __asm__ __volatile__("push %%ebx\n"                                     \
                           CFI_ADJUST_CFA_OFFSET(4)                           \
                           "movl %2,%%ebx\n"                                  \
                           "int $0x80\n"                                      \
                           "pop %%ebx\n"                                      \
                           CFI_ADJUST_CFA_OFFSET(-4)                          \
                           args                                               \
                           : "esp", "memory");                                \
      LSS_RETURN(type,__res)
    #undef  _syscall0
    #define _syscall0(type,name)                                              \
      type LSS_NAME(name)(void) {                                             \
        long __res;                                                           \
        __asm__ volatile("int $0x80"                                          \
                         : "=a" (__res)                                       \
                         : "0" (__NR_##name)                                  \
                         : "memory");                                         \
        LSS_RETURN(type,__res);                                               \
      }
    #undef  _syscall1
    #define _syscall1(type,name,type1,arg1)                                   \
      type LSS_NAME(name)(type1 arg1) {                                       \
        LSS_BODY(type,                                                        \
             : "=a" (__res)                                                   \
             : "0" (__NR_##name), "ri" ((long)(arg1)));                       \
      }
    #undef  _syscall2
    #define _syscall2(type,name,type1,arg1,type2,arg2)                        \
      type LSS_NAME(name)(type1 arg1,type2 arg2) {                            \
        LSS_BODY(type,                                                        \
             : "=a" (__res)                                                   \
             : "0" (__NR_##name),"ri" ((long)(arg1)), "c" ((long)(arg2)));    \
      }
    #undef  _syscall3
    #define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)             \
      type LSS_NAME(name)(type1 arg1,type2 arg2,type3 arg3) {                 \
        LSS_BODY(type,                                                        \
             : "=a" (__res)                                                   \
             : "0" (__NR_##name), "ri" ((long)(arg1)), "c" ((long)(arg2)),    \
               "d" ((long)(arg3)));                                           \
      }
    #undef  _syscall4
    #define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {   \
        LSS_BODY(type,                                                        \
             : "=a" (__res)                                                   \
             : "0" (__NR_##name), "ri" ((long)(arg1)), "c" ((long)(arg2)),    \
               "d" ((long)(arg3)),"S" ((long)(arg4)));                        \
      }
    #undef  _syscall5
    #define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5)                                             \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5) {                                       \
        long __res;                                                           \
        __asm__ __volatile__("push %%ebx\n"                                   \
                             "movl %2,%%ebx\n"                                \
                             "movl %1,%%eax\n"                                \
                             "int  $0x80\n"                                   \
                             "pop  %%ebx"                                     \
                             : "=a" (__res)                                   \
                             : "i" (__NR_##name), "ri" ((long)(arg1)),        \
                               "c" ((long)(arg2)), "d" ((long)(arg3)),        \
                               "S" ((long)(arg4)), "D" ((long)(arg5))         \
                             : "esp", "memory");                              \
        LSS_RETURN(type,__res);                                               \
      }
    #undef  _syscall6
    #define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5,type6,arg6)                                  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5, type6 arg6) {                           \
        long __res;                                                           \
        struct { long __a1; long __a6; } __s = { (long)arg1, (long) arg6 };   \
        __asm__ __volatile__("push %%ebp\n"                                   \
                             "push %%ebx\n"                                   \
                             "movl 4(%2),%%ebp\n"                             \
                             "movl 0(%2), %%ebx\n"                            \
                             "movl %1,%%eax\n"                                \
                             "int  $0x80\n"                                   \
                             "pop  %%ebx\n"                                   \
                             "pop  %%ebp"                                     \
                             : "=a" (__res)                                   \
                             : "i" (__NR_##name),  "0" ((long)(&__s)),        \
                               "c" ((long)(arg2)), "d" ((long)(arg3)),        \
                               "S" ((long)(arg4)), "D" ((long)(arg5))         \
                             : "esp", "memory");                              \
        LSS_RETURN(type,__res);                                               \
      }
    LSS_INLINE int LSS_NAME(clone)(int (*fn)(void *), void *child_stack,
                                   int flags, void *arg, int *parent_tidptr,
                                   void *newtls, int *child_tidptr) {
      long __res;
      __asm__ __volatile__(
                           "movl   %3,%%ecx\n"
                           "jecxz  1f\n"

                           "movl   %4,%%ecx\n"
                           "jecxz  1f\n"

                           "andl   $-16,%%ecx\n"
                           "subl   $20,%%ecx\n"

                           "movl   %6,%%eax\n"
                           "movl   %%eax,4(%%ecx)\n"
                           "movl   %3,%%eax\n"
                           "movl   %%eax,(%%ecx)\n"

                           "movl   %8,%%esi\n"
                           "movl   %7,%%edx\n"
                           "movl   %5,%%eax\n"
                           "movl   %9,%%edi\n"
                           "pushl  %%ebx\n"
                           "movl   %%eax,%%ebx\n"
                           "movl   %2,%%eax\n"
                           "int    $0x80\n"

                           "popl   %%ebx\n"

                           "test   %%eax,%%eax\n"
                           "jnz    1f\n"

                           "movl   $0,%%ebp\n"

                           "call   *%%ebx\n"

                           "movl   %%eax,%%ebx\n"
                           "movl   $1,%%eax\n"
                           "int    $0x80\n"

                         "1:\n"
                           : "=a" (__res)
                           : "0"(-EINVAL), "i"(__NR_clone),
                             "m"(fn), "m"(child_stack), "m"(flags), "m"(arg),
                             "m"(parent_tidptr), "m"(newtls), "m"(child_tidptr)
                           : "esp", "memory", "ecx", "edx", "esi", "edi");
      LSS_RETURN(int, __res);
    }

    LSS_INLINE void (*LSS_NAME(restore_rt)(void))(void) {
      void (*res)(void);
      __asm__ __volatile__("call   2f\n"
                         "0:.align 16\n"
                         "1:movl   %1,%%eax\n"
                           "int    $0x80\n"
                         "2:popl   %0\n"
                           "addl   $(1b-0b),%0\n"
                           : "=a" (res)
                           : "i"  (__NR_rt_sigreturn));
      return res;
    }
    LSS_INLINE void (*LSS_NAME(restore)(void))(void) {
      void (*res)(void);
      __asm__ __volatile__("call   2f\n"
                         "0:.align 16\n"
                         "1:pop    %%eax\n"
                           "movl   %1,%%eax\n"
                           "int    $0x80\n"
                         "2:popl   %0\n"
                           "addl   $(1b-0b),%0\n"
                           : "=a" (res)
                           : "i"  (__NR_sigreturn));
      return res;
    }
  #elif defined(__x86_64__)
    #undef  LSS_BODY
    #define LSS_BODY(type,name, ...)                                          \
          long __res;                                                         \
          __asm__ __volatile__("syscall" : "=a" (__res) : "0" (__NR_##name),  \
            ##__VA_ARGS__ : "r11", "rcx", "memory");                          \
          LSS_RETURN(type, __res)
    #undef _syscall0
    #define _syscall0(type,name)                                              \
      type LSS_NAME(name)() {                                                 \
        LSS_BODY(type, name);                                                 \
      }
    #undef _syscall1
    #define _syscall1(type,name,type1,arg1)                                   \
      type LSS_NAME(name)(type1 arg1) {                                       \
        LSS_BODY(type, name, "D" ((long)(arg1)));                             \
      }
    #undef _syscall2
    #define _syscall2(type,name,type1,arg1,type2,arg2)                        \
      type LSS_NAME(name)(type1 arg1, type2 arg2) {                           \
        LSS_BODY(type, name, "D" ((long)(arg1)), "S" ((long)(arg2)));         \
      }
    #undef _syscall3
    #define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)             \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3) {               \
        LSS_BODY(type, name, "D" ((long)(arg1)), "S" ((long)(arg2)),          \
                             "d" ((long)(arg3)));                             \
      }
    #undef _syscall4
    #define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {   \
          long __res;                                                         \
          __asm__ __volatile__("movq %5,%%r10; syscall" :                     \
            "=a" (__res) : "0" (__NR_##name),                                 \
            "D" ((long)(arg1)), "S" ((long)(arg2)), "d" ((long)(arg3)),       \
            "r" ((long)(arg4)) : "r10", "r11", "rcx", "memory");              \
          LSS_RETURN(type, __res);                                            \
      }
    #undef _syscall5
    #define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5)                                             \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5) {                                       \
          long __res;                                                         \
          __asm__ __volatile__("movq %5,%%r10; movq %6,%%r8; syscall" :       \
            "=a" (__res) : "0" (__NR_##name),                                 \
            "D" ((long)(arg1)), "S" ((long)(arg2)), "d" ((long)(arg3)),       \
            "r" ((long)(arg4)), "r" ((long)(arg5)) :                          \
            "r8", "r10", "r11", "rcx", "memory");                             \
          LSS_RETURN(type, __res);                                            \
      }
    #undef _syscall6
    #define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5,type6,arg6)                                  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5, type6 arg6) {                           \
          long __res;                                                         \
          __asm__ __volatile__("movq %5,%%r10; movq %6,%%r8; movq %7,%%r9;"   \
                               "syscall" :                                    \
            "=a" (__res) : "0" (__NR_##name),                                 \
            "D" ((long)(arg1)), "S" ((long)(arg2)), "d" ((long)(arg3)),       \
            "r" ((long)(arg4)), "r" ((long)(arg5)), "r" ((long)(arg6)) :      \
            "r8", "r9", "r10", "r11", "rcx", "memory");                       \
          LSS_RETURN(type, __res);                                            \
      }
    LSS_INLINE int LSS_NAME(clone)(int (*fn)(void *), void *child_stack,
                                   int flags, void *arg, int *parent_tidptr,
                                   void *newtls, int *child_tidptr) {
      long __res;
      {
        __asm__ __volatile__(
                             "testq  %4,%4\n"
                             "jz     1f\n"

                             "testq  %5,%5\n"
                             "jz     1f\n"

                             "andq   $-16,%5\n"
                             "subq   $16,%5\n"

                             "movq   %7,8(%5)\n"
                             "movq   %4,0(%5)\n"

                             "movq   %2,%%rax\n"
                             "movq   %9,%%r8\n"
                             "movq   %10,%%r10\n"
                             "syscall\n"

                             "testq  %%rax,%%rax\n"
                             "jnz    1f\n"

                             "xorq   %%rbp,%%rbp\n"

                             "popq   %%rax\n"
                             "popq   %%rdi\n"
                             "call   *%%rax\n"

                             "movq   %%rax,%%rdi\n"
                             "movq   %3,%%rax\n"
                             "syscall\n"

                           "1:\n"
                             : "=a" (__res)
                             : "0"(-EINVAL), "i"(__NR_clone), "i"(__NR_exit),
                               "r"(fn), "S"(child_stack), "D"(flags), "r"(arg),
                               "d"(parent_tidptr), "g"(newtls), "g"(child_tidptr)
                             : "rsp", "memory", "r8", "r10", "r11", "rcx");
      }
      LSS_RETURN(int, __res);
    }

    LSS_INLINE void (*LSS_NAME(restore_rt)(void))(void) {
      void (*res)(void);
      __asm__ __volatile__("call   2f\n"
                         "0:.align 16\n"
                         "1:movq   %1,%%rax\n"
                           "syscall\n"
                         "2:popq   %0\n"
                           "addq   $(1b-0b),%0\n"
                           : "=a" (res)
                           : "i"  (__NR_rt_sigreturn));
      return res;
    }
  #elif defined(__arm__)
    #undef LSS_REG
    #define LSS_REG(r,a) register long __r##r __asm__("r"#r) = (long)a

    #undef LSS_SAVE_ARG
    #define LSS_SAVE_ARG(r,a) long __tmp##r = (long)a
    #undef LSS_LOAD_ARG
    #define LSS_LOAD_ARG(r) register long __r##r __asm__("r"#r) = __tmp##r

    #undef  LSS_BODY
    #define LSS_BODY(type, name, args...)                                     \
          register long __res_r0 __asm__("r0");                               \
          long __res;                                                         \
          __SYS_REG(name)                                                     \
          __asm__ __volatile__ (__syscall_safe(name)                          \
                                : "=r"(__res_r0)                              \
                                : __SYS_REG_LIST(args)                        \
                                : "lr", "memory");                            \
          __res = __res_r0;                                                   \
          LSS_RETURN(type, __res)
    #undef _syscall0
    #define _syscall0(type, name)                                             \
      type LSS_NAME(name)() {                                                 \
        LSS_BODY(type, name);                                                 \
      }
    #undef _syscall1
    #define _syscall1(type, name, type1, arg1)                                \
      type LSS_NAME(name)(type1 arg1) {                                       \
                            \
        LSS_REG(0, arg1);                                                     \
        LSS_BODY(type, name, "r"(__r0));                                      \
      }
    #undef _syscall2
    #define _syscall2(type, name, type1, arg1, type2, arg2)                   \
      type LSS_NAME(name)(type1 arg1, type2 arg2) {                           \
        LSS_SAVE_ARG(0, arg1);                                                \
        LSS_SAVE_ARG(1, arg2);                                                \
        LSS_LOAD_ARG(0);                                                      \
        LSS_LOAD_ARG(1);                                                      \
        LSS_BODY(type, name, "r"(__r0), "r"(__r1));                           \
      }
    #undef _syscall3
    #define _syscall3(type, name, type1, arg1, type2, arg2, type3, arg3)      \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3) {               \
        LSS_SAVE_ARG(0, arg1);                                                \
        LSS_SAVE_ARG(1, arg2);                                                \
        LSS_SAVE_ARG(2, arg3);                                                \
        LSS_LOAD_ARG(0);                                                      \
        LSS_LOAD_ARG(1);                                                      \
        LSS_LOAD_ARG(2);                                                      \
        LSS_BODY(type, name, "r"(__r0), "r"(__r1), "r"(__r2));                \
      }
    #undef _syscall4
    #define _syscall4(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                      type4, arg4)                                            \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {   \
        LSS_SAVE_ARG(0, arg1);                                                \
        LSS_SAVE_ARG(1, arg2);                                                \
        LSS_SAVE_ARG(2, arg3);                                                \
        LSS_SAVE_ARG(3, arg4);                                                \
        LSS_LOAD_ARG(0);                                                      \
        LSS_LOAD_ARG(1);                                                      \
        LSS_LOAD_ARG(2);                                                      \
        LSS_LOAD_ARG(3);                                                      \
        LSS_BODY(type, name, "r"(__r0), "r"(__r1), "r"(__r2), "r"(__r3));     \
      }
    #undef _syscall5
    #define _syscall5(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                      type4, arg4, type5, arg5)                               \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5) {                                       \
        LSS_SAVE_ARG(0, arg1);                                                \
        LSS_SAVE_ARG(1, arg2);                                                \
        LSS_SAVE_ARG(2, arg3);                                                \
        LSS_SAVE_ARG(3, arg4);                                                \
        LSS_REG(4, arg5);                                                     \
        LSS_LOAD_ARG(0);                                                      \
        LSS_LOAD_ARG(1);                                                      \
        LSS_LOAD_ARG(2);                                                      \
        LSS_LOAD_ARG(3);                                                      \
        LSS_BODY(type, name, "r"(__r0), "r"(__r1), "r"(__r2), "r"(__r3),      \
                             "r"(__r4));                                      \
      }
    #undef _syscall6
    #define _syscall6(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                      type4, arg4, type5, arg5, type6, arg6)                  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5, type6 arg6) {                           \
        LSS_SAVE_ARG(0, arg1);                                                \
        LSS_SAVE_ARG(1, arg2);                                                \
        LSS_SAVE_ARG(2, arg3);                                                \
        LSS_SAVE_ARG(3, arg4);                                                \
        LSS_REG(4, arg5);                                                     \
        LSS_REG(5, arg6);                                                     \
        LSS_LOAD_ARG(0);                                                      \
        LSS_LOAD_ARG(1);                                                      \
        LSS_LOAD_ARG(2);                                                      \
        LSS_LOAD_ARG(3);                                                      \
        LSS_BODY(type, name, "r"(__r0), "r"(__r1), "r"(__r2), "r"(__r3),      \
                             "r"(__r4), "r"(__r5));                           \
      }
    LSS_INLINE int LSS_NAME(clone)(int (*fn)(void *), void *child_stack,
                                   int flags, void *arg, int *parent_tidptr,
                                   void *newtls, int *child_tidptr) {
      register long __res __asm__("r5");
      {
        if (fn == NULL || child_stack == NULL) {
            __res = -EINVAL;
            goto clone_exit;
        }

        int    tmp_flags = flags;
        int  * tmp_stack = (int*) child_stack;
        void * tmp_ptid  = parent_tidptr;
        void * tmp_tls   = newtls;

        register int  *__ctid  __asm__("r4") = child_tidptr;

        *(--tmp_stack) = (int) arg;
        *(--tmp_stack) = (int) fn;

        
        register int   __flags __asm__("r0") = tmp_flags;
        register void *__stack __asm__("r1") = tmp_stack;
        register void *__ptid  __asm__("r2") = tmp_ptid;
        register void *__tls   __asm__("r3") = tmp_tls;

        __SYS_REG(clone)
        __asm__ __volatile__(
                             "push  {r7}\n"
                             "mov   r7,%1\n"
                             __syscall(clone)"\n"

                             "movs  %0,r0\n"
                             "bne   1f\n"

                             "ldr   r0,[sp, #4]\n"
                             "mov   lr,pc\n"
                             "ldr   pc,[sp]\n"

                             "mov   r7,%2\n"
                             __syscall(exit)"\n"

                           "1: pop {r7}\n"
                             : "=r" (__res)
                             : "r"(__sysreg),
                               "i"(__NR_exit), "r"(__stack), "r"(__flags),
                               "r"(__ptid), "r"(__tls), "r"(__ctid)
                             : "cc", "lr", "memory");
      }
      clone_exit:
      LSS_RETURN(int, __res);
    }
  #elif defined(__mips__)
    #undef LSS_REG
    #define LSS_REG(r,a) register unsigned long __r##r __asm__("$"#r) =       \
                                 (unsigned long)(a)

    #if _MIPS_SIM == _MIPS_SIM_ABI32
    
    
    #define MIPS_SYSCALL_CLOBBERS "$1", "$3", "$8", "$9", "$10", "$11", "$12",\
                                "$13", "$14", "$15", "$24", "$25", "memory"
    #else
    #define MIPS_SYSCALL_CLOBBERS "$1", "$3", "$10", "$11", "$12", "$13",     \
                                "$14", "$15", "$24", "$25", "memory"
    #endif

    #undef  LSS_BODY
    #define LSS_BODY(type,name,r7,...)                                        \
          register unsigned long __v0 __asm__("$2") = __NR_##name;            \
          __asm__ __volatile__ ("syscall\n"                                   \
                                : "=&r"(__v0), r7 (__r7)                      \
                                : "0"(__v0), ##__VA_ARGS__                    \
                                : MIPS_SYSCALL_CLOBBERS);                     \
          LSS_RETURN(type, __v0, __r7)
    #undef _syscall0
    #define _syscall0(type, name)                                             \
      type LSS_NAME(name)() {                                                 \
        register unsigned long __r7 __asm__("$7");                            \
        LSS_BODY(type, name, "=r");                                           \
      }
    #undef _syscall1
    #define _syscall1(type, name, type1, arg1)                                \
      type LSS_NAME(name)(type1 arg1) {                                       \
        register unsigned long __r7 __asm__("$7");                            \
        LSS_REG(4, arg1); LSS_BODY(type, name, "=r", "r"(__r4));              \
      }
    #undef _syscall2
    #define _syscall2(type, name, type1, arg1, type2, arg2)                   \
      type LSS_NAME(name)(type1 arg1, type2 arg2) {                           \
        register unsigned long __r7 __asm__("$7");                            \
        LSS_REG(4, arg1); LSS_REG(5, arg2);                                   \
        LSS_BODY(type, name, "=r", "r"(__r4), "r"(__r5));                     \
      }
    #undef _syscall3
    #define _syscall3(type, name, type1, arg1, type2, arg2, type3, arg3)      \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3) {               \
        register unsigned long __r7 __asm__("$7");                            \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_BODY(type, name, "=r", "r"(__r4), "r"(__r5), "r"(__r6));          \
      }
    #undef _syscall4
    #define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {   \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_REG(7, arg4);                                                     \
        LSS_BODY(type, name, "+r", "r"(__r4), "r"(__r5), "r"(__r6));          \
      }
    #undef _syscall5
    #if _MIPS_SIM == _MIPS_SIM_ABI32
    #define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5)                                             \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5) {                                       \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_REG(7, arg4);                                                     \
        register unsigned long __v0 __asm__("$2");                            \
        __asm__ __volatile__ (".set noreorder\n"                              \
                              "lw    $2, %6\n"                                \
                              "subu  $29, 32\n"                               \
                              "sw    $2, 16($29)\n"                           \
                              "li    $2, %2\n"                                \
                              "syscall\n"                                     \
                              "addiu $29, 32\n"                               \
                              ".set reorder\n"                                \
                              : "=&r"(__v0), "+r" (__r7)                      \
                              : "i" (__NR_##name), "r"(__r4), "r"(__r5),      \
                                "r"(__r6), "m" ((unsigned long)arg5)          \
                              : MIPS_SYSCALL_CLOBBERS);                       \
        LSS_RETURN(type, __v0, __r7);                                         \
      }
    #else
    #define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5)                                             \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5) {                                       \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_REG(7, arg4); LSS_REG(8, arg5);                                   \
        LSS_BODY(type, name, "+r", "r"(__r4), "r"(__r5), "r"(__r6),           \
                 "r"(__r8));                                                  \
      }
    #endif
    #undef _syscall6
    #if _MIPS_SIM == _MIPS_SIM_ABI32
    #define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5,type6,arg6)                                  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5, type6 arg6) {                           \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_REG(7, arg4);                                                     \
        register unsigned long __v0 __asm__("$2");                            \
        __asm__ __volatile__ (".set noreorder\n"                              \
                              "lw    $2, %6\n"                                \
                              "lw    $8, %7\n"                                \
                              "subu  $29, 32\n"                               \
                              "sw    $2, 16($29)\n"                           \
                              "sw    $8, 20($29)\n"                           \
                              "li    $2, %2\n"                                \
                              "syscall\n"                                     \
                              "addiu $29, 32\n"                               \
                              ".set reorder\n"                                \
                              : "=&r"(__v0), "+r" (__r7)                      \
                              : "i" (__NR_##name), "r"(__r4), "r"(__r5),      \
                                "r"(__r6), "r" ((unsigned long)arg5),         \
                                "r" ((unsigned long)arg6)                     \
                              : MIPS_SYSCALL_CLOBBERS);                       \
        LSS_RETURN(type, __v0, __r7);                                         \
      }
    #else
    #define _syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,  \
                      type5,arg5,type6,arg6)                                  \
      type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,     \
                          type5 arg5,type6 arg6) {                            \
        LSS_REG(4, arg1); LSS_REG(5, arg2); LSS_REG(6, arg3);                 \
        LSS_REG(7, arg4); LSS_REG(8, arg5); LSS_REG(9, arg6);                 \
        LSS_BODY(type, name, "+r", "r"(__r4), "r"(__r5), "r"(__r6),           \
                 "r"(__r8), "r"(__r9));                                       \
      }
    #endif
    LSS_INLINE int LSS_NAME(clone)(int (*fn)(void *), void *child_stack,
                                   int flags, void *arg, int *parent_tidptr,
                                   void *newtls, int *child_tidptr) {
      register unsigned long __v0 __asm__("$2");
      register unsigned long __r7 __asm__("$7") = (unsigned long)newtls;
      {
        register int   __flags __asm__("$4") = flags;
        register void *__stack __asm__("$5") = child_stack;
        register void *__ptid  __asm__("$6") = parent_tidptr;
        register int  *__ctid  __asm__("$8") = child_tidptr;
        __asm__ __volatile__(
          #if _MIPS_SIM == _MIPS_SIM_ABI32 && _MIPS_SZPTR == 32
                             "subu  $29,24\n"
          #elif _MIPS_SIM == _MIPS_SIM_NABI32
                             "sub   $29,16\n"
          #else
                             "dsubu $29,16\n"
          #endif

                             "li    %0,%2\n"
                             "beqz  %5,1f\n"
                             "beqz  %6,1f\n"

          #if _MIPS_SIM == _MIPS_SIM_ABI32 && _MIPS_SZPTR == 32
                             "subu  %6,32\n"
                             "sw    %5,0(%6)\n"
                             "sw    %8,4(%6)\n"
          #elif _MIPS_SIM == _MIPS_SIM_NABI32
                             "sub   %6,32\n"
                             "sw    %5,0(%6)\n"
                             "sw    %8,8(%6)\n"
          #else
                             "dsubu %6,32\n"
                             "sd    %5,0(%6)\n"
                             "sd    %8,8(%6)\n"
          #endif

                             "li    $2,%3\n"
                             "syscall\n"

                             "bnez  $7,1f\n"
                             "bnez  $2,1f\n"

          #if _MIPS_SIM == _MIPS_SIM_ABI32 && _MIPS_SZPTR == 32
                            "lw    $25,0($29)\n"
                            "lw    $4,4($29)\n"
          #elif _MIPS_SIM == _MIPS_SIM_NABI32
                            "lw    $25,0($29)\n"
                            "lw    $4,8($29)\n"
          #else
                            "ld    $25,0($29)\n"
                            "ld    $4,8($29)\n"
          #endif
                            "jalr  $25\n"

                            "move  $4,$2\n"
                            "li    $2,%4\n"
                            "syscall\n"

                           "1:\n"
          #if _MIPS_SIM == _MIPS_SIM_ABI32 && _MIPS_SZPTR == 32
                             "addu  $29, 24\n"
          #elif _MIPS_SIM == _MIPS_SIM_NABI32
                             "add   $29, 16\n"
          #else
                             "daddu $29,16\n"
          #endif
                             : "=&r" (__v0), "=r" (__r7)
                             : "i"(-EINVAL), "i"(__NR_clone), "i"(__NR_exit),
                               "r"(fn), "r"(__stack), "r"(__flags), "r"(arg),
                               "r"(__ptid), "r"(__r7), "r"(__ctid)
                             : "$9", "$10", "$11", "$12", "$13", "$14", "$15",
                               "$24", "memory");
      }
      LSS_RETURN(int, __v0, __r7);
    }
  #elif defined (__PPC__)
    #undef  LSS_LOADARGS_0
    #define LSS_LOADARGS_0(name, dummy...)                                    \
        __sc_0 = __NR_##name
    #undef  LSS_LOADARGS_1
    #define LSS_LOADARGS_1(name, arg1)                                        \
            LSS_LOADARGS_0(name);                                             \
            __sc_3 = (unsigned long) (arg1)
    #undef  LSS_LOADARGS_2
    #define LSS_LOADARGS_2(name, arg1, arg2)                                  \
            LSS_LOADARGS_1(name, arg1);                                       \
            __sc_4 = (unsigned long) (arg2)
    #undef  LSS_LOADARGS_3
    #define LSS_LOADARGS_3(name, arg1, arg2, arg3)                            \
            LSS_LOADARGS_2(name, arg1, arg2);                                 \
            __sc_5 = (unsigned long) (arg3)
    #undef  LSS_LOADARGS_4
    #define LSS_LOADARGS_4(name, arg1, arg2, arg3, arg4)                      \
            LSS_LOADARGS_3(name, arg1, arg2, arg3);                           \
            __sc_6 = (unsigned long) (arg4)
    #undef  LSS_LOADARGS_5
    #define LSS_LOADARGS_5(name, arg1, arg2, arg3, arg4, arg5)                \
            LSS_LOADARGS_4(name, arg1, arg2, arg3, arg4);                     \
            __sc_7 = (unsigned long) (arg5)
    #undef  LSS_LOADARGS_6
    #define LSS_LOADARGS_6(name, arg1, arg2, arg3, arg4, arg5, arg6)          \
            LSS_LOADARGS_5(name, arg1, arg2, arg3, arg4, arg5);               \
            __sc_8 = (unsigned long) (arg6)
    #undef  LSS_ASMINPUT_0
    #define LSS_ASMINPUT_0 "0" (__sc_0)
    #undef  LSS_ASMINPUT_1
    #define LSS_ASMINPUT_1 LSS_ASMINPUT_0, "1" (__sc_3)
    #undef  LSS_ASMINPUT_2
    #define LSS_ASMINPUT_2 LSS_ASMINPUT_1, "2" (__sc_4)
    #undef  LSS_ASMINPUT_3
    #define LSS_ASMINPUT_3 LSS_ASMINPUT_2, "3" (__sc_5)
    #undef  LSS_ASMINPUT_4
    #define LSS_ASMINPUT_4 LSS_ASMINPUT_3, "4" (__sc_6)
    #undef  LSS_ASMINPUT_5
    #define LSS_ASMINPUT_5 LSS_ASMINPUT_4, "5" (__sc_7)
    #undef  LSS_ASMINPUT_6
    #define LSS_ASMINPUT_6 LSS_ASMINPUT_5, "6" (__sc_8)
    #undef  LSS_BODY
    #define LSS_BODY(nr, type, name, args...)                                 \
        long __sc_ret, __sc_err;                                              \
        {                                                                     \
                        register unsigned long __sc_0 __asm__ ("r0");         \
                        register unsigned long __sc_3 __asm__ ("r3");         \
                        register unsigned long __sc_4 __asm__ ("r4");         \
                        register unsigned long __sc_5 __asm__ ("r5");         \
                        register unsigned long __sc_6 __asm__ ("r6");         \
                        register unsigned long __sc_7 __asm__ ("r7");         \
                        register unsigned long __sc_8 __asm__ ("r8");         \
                                                                              \
            LSS_LOADARGS_##nr(name, args);                                    \
            __asm__ __volatile__                                              \
                ("sc\n\t"                                                     \
                 "mfcr %0"                                                    \
                 : "=&r" (__sc_0),                                            \
                   "=&r" (__sc_3), "=&r" (__sc_4),                            \
                   "=&r" (__sc_5), "=&r" (__sc_6),                            \
                   "=&r" (__sc_7), "=&r" (__sc_8)                             \
                 : LSS_ASMINPUT_##nr                                          \
                 : "cr0", "ctr", "memory",                                    \
                   "r9", "r10", "r11", "r12");                                \
            __sc_ret = __sc_3;                                                \
            __sc_err = __sc_0;                                                \
        }                                                                     \
        LSS_RETURN(type, __sc_ret, __sc_err)
    #undef _syscall0
    #define _syscall0(type, name)                                             \
       type LSS_NAME(name)(void) {                                            \
          LSS_BODY(0, type, name);                                            \
       }
    #undef _syscall1
    #define _syscall1(type, name, type1, arg1)                                \
       type LSS_NAME(name)(type1 arg1) {                                      \
          LSS_BODY(1, type, name, arg1);                                      \
       }
    #undef _syscall2
    #define _syscall2(type, name, type1, arg1, type2, arg2)                   \
       type LSS_NAME(name)(type1 arg1, type2 arg2) {                          \
          LSS_BODY(2, type, name, arg1, arg2);                                \
       }
    #undef _syscall3
    #define _syscall3(type, name, type1, arg1, type2, arg2, type3, arg3)      \
       type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3) {              \
          LSS_BODY(3, type, name, arg1, arg2, arg3);                          \
       }
    #undef _syscall4
    #define _syscall4(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                                  type4, arg4)                                \
       type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {  \
          LSS_BODY(4, type, name, arg1, arg2, arg3, arg4);                    \
       }
    #undef _syscall5
    #define _syscall5(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                                  type4, arg4, type5, arg5)                   \
       type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,    \
                                               type5 arg5) {                  \
          LSS_BODY(5, type, name, arg1, arg2, arg3, arg4, arg5);              \
       }
    #undef _syscall6
    #define _syscall6(type, name, type1, arg1, type2, arg2, type3, arg3,      \
                                  type4, arg4, type5, arg5, type6, arg6)      \
       type LSS_NAME(name)(type1 arg1, type2 arg2, type3 arg3, type4 arg4,    \
                                               type5 arg5, type6 arg6) {      \
          LSS_BODY(6, type, name, arg1, arg2, arg3, arg4, arg5, arg6);        \
       }
    
    LSS_INLINE int LSS_NAME(clone)(int (*fn)(void *), void *child_stack,
                                   int flags, void *arg, int *parent_tidptr,
                                   void *newtls, int *child_tidptr) {
      long __ret, __err;
      {
        register int (*__fn)(void *)    __asm__ ("r8")  = fn;
        register void *__cstack                 __asm__ ("r4")  = child_stack;
        register int __flags                    __asm__ ("r3")  = flags;
        register void * __arg                   __asm__ ("r9")  = arg;
        register int * __ptidptr                __asm__ ("r5")  = parent_tidptr;
        register void * __newtls                __asm__ ("r6")  = newtls;
        register int * __ctidptr                __asm__ ("r7")  = child_tidptr;
        __asm__ __volatile__(
            "cmpwi cr0, %6, 0\n\t"
            "cmpwi cr1, %7, 0\n\t"
            "cror cr0*4+eq, cr1*4+eq, cr0*4+eq\n\t"
            "beq- cr0, 1f\n\t"

            
            "clrrwi %7, %7, 4\n\t"
            "li 0, 0\n\t"
            "stwu 0, -16(%7)\n\t"

            
            "mr 28, %6\n\t"
            "mr 29, %7\n\t"
            "mr 27, %9\n\t"

            
            "li 0, %4\n\t"
            "sc\n\t"

            
            "cmpwi cr1, 3, 0\n\t"
            "crandc cr1*4+eq, cr1*4+eq, cr0*4+so\n\t"
            "bne- cr1, 1f\n\t"

            
            "mtctr 28\n\t"
            "mr 3, 27\n\t"
            "bctrl\n\t"

            
            "li 0, %5\n\t"
            "sc\n\t"

            
            "1:\n"
            "mfcr %1\n\t"
            "mr %0, 3\n\t"
              : "=r" (__ret), "=r" (__err)
              : "0" (-1), "1" (EINVAL),
                "i" (__NR_clone), "i" (__NR_exit),
                "r" (__fn), "r" (__cstack), "r" (__flags),
                "r" (__arg), "r" (__ptidptr), "r" (__newtls),
                "r" (__ctidptr)
              : "cr0", "cr1", "memory", "ctr",
                "r0", "r29", "r27", "r28");
      }
      LSS_RETURN(int, __ret, __err);
    }
  #endif
  #define __NR__exit   __NR_exit
  #define __NR__gettid __NR_gettid
  #define __NR__mremap __NR_mremap
  LSS_INLINE _syscall1(int,     close,           int,         f)
  LSS_INLINE _syscall1(int,     _exit,           int,         e)
  LSS_INLINE _syscall3(int,     fcntl,           int,         f,
                       int,            c, long,   a)
  LSS_INLINE _syscall2(int,     fstat,           int,         f,
                      struct kernel_stat*,   b)
  LSS_INLINE _syscall4(int,     futex,           int*,        a,
                       int,            o, int,    v,
                      struct kernel_timespec*, t)
  LSS_INLINE _syscall3(int,     getdents,        int,         f,
                      struct kernel_dirent*, d, int,    c)
#ifdef __NR_getdents64
  LSS_INLINE _syscall3(int,     getdents64,      int,         f,
                      struct kernel_dirent64*, d, int,    c)
#endif
  LSS_INLINE _syscall0(pid_t,   getpid)
  LSS_INLINE _syscall0(pid_t,   getppid)
  LSS_INLINE _syscall0(pid_t,   _gettid)
  LSS_INLINE _syscall2(int,     kill,            pid_t,       p,
                       int,            s)
  LSS_INLINE _syscall3(off_t,   lseek,           int,         f,
                       off_t,          o, int,    w)
  LSS_INLINE _syscall2(int,     munmap,          void*,       s,
                       size_t,         l)
  LSS_INLINE _syscall5(void*,   _mremap,         void*,       o,
                       size_t,         os,       size_t,      ns,
                       unsigned long,  f, void *, a)
  LSS_INLINE _syscall3(int,     open,            const char*, p,
                       int,            f, int,    m)
  LSS_INLINE _syscall2(int,     prctl,           int,         o,
                       long,           a)
  LSS_INLINE _syscall4(long,    ptrace,          int,         r,
                       pid_t,          p, void *, a, void *, d)
  LSS_INLINE _syscall3(ssize_t, read,            int,         f,
                       void *,         b, size_t, c)
  LSS_INLINE _syscall4(int,     rt_sigaction,    int,         s,
                       const struct kernel_sigaction*, a,
                       struct kernel_sigaction*, o, size_t,   c)
  LSS_INLINE _syscall4(int, rt_sigprocmask,      int,         h,
                       const struct kernel_sigset_t*,  s,
                       struct kernel_sigset_t*,        o, size_t, c);
  LSS_INLINE _syscall0(int,     sched_yield)
  LSS_INLINE _syscall2(int,     sigaltstack,     const stack_t*, s,
                       const stack_t*, o)
  LSS_INLINE _syscall2(int,     stat,            const char*, f,
                      struct kernel_stat*,   b)
  LSS_INLINE _syscall3(ssize_t, write,            int,        f,
                       const void *,   b, size_t, c)
  #if defined(__NR_getcpu)
    LSS_INLINE _syscall3(long, getcpu, unsigned *, cpu,
                         unsigned *, node, void *, unused);
  #endif
  #if defined(__x86_64__) ||                                                  \
     (defined(__mips__) && _MIPS_SIM != _MIPS_SIM_ABI32)
    LSS_INLINE _syscall3(int, socket,             int,   d,
                         int,                     t, int,       p)
  #endif
  #if defined(__x86_64__)
    LSS_INLINE _syscall6(void*, mmap,              void*, s,
                         size_t,                   l, int,               p,
                         int,                      f, int,               d,
                         __off64_t,                o)

    LSS_INLINE int LSS_NAME(sigaction)(int signum,
                                       const struct kernel_sigaction *act,
                                       struct kernel_sigaction *oldact) {
      if (act != NULL && !(act->sa_flags & SA_RESTORER)) {
        struct kernel_sigaction a = *act;
        a.sa_flags   |= SA_RESTORER;
        a.sa_restorer = LSS_NAME(restore_rt)();
        return LSS_NAME(rt_sigaction)(signum, &a, oldact,
                                      (KERNEL_NSIG+7)/8);
      } else {
        return LSS_NAME(rt_sigaction)(signum, act, oldact,
                                      (KERNEL_NSIG+7)/8);
      }
    }

    LSS_INLINE int LSS_NAME(sigprocmask)(int how,
                                         const struct kernel_sigset_t *set,
                                         struct kernel_sigset_t *oldset) {
      return LSS_NAME(rt_sigprocmask)(how, set, oldset, (KERNEL_NSIG+7)/8);
    }
  #endif
  #if defined(__x86_64__) || \
      defined(__arm__) || \
     (defined(__mips__) && _MIPS_SIM != _MIPS_SIM_ABI32)
    LSS_INLINE _syscall4(pid_t, wait4,            pid_t, p,
                         int*,                    s, int,       o,
                         struct kernel_rusage*,   r)
    LSS_INLINE pid_t LSS_NAME(waitpid)(pid_t pid, int *status, int options){
      return LSS_NAME(wait4)(pid, status, options, 0);
    }
   #endif
  #if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
    LSS_INLINE _syscall4(int, openat, int, d, const char *, p, int, f, int, m)
  #endif
  LSS_INLINE int LSS_NAME(sigemptyset)(struct kernel_sigset_t *set) {
    memset(&set->sig, 0, sizeof(set->sig));
    return 0;
  }

  LSS_INLINE int LSS_NAME(sigfillset)(struct kernel_sigset_t *set) {
    memset(&set->sig, -1, sizeof(set->sig));
    return 0;
  }

  LSS_INLINE int LSS_NAME(sigaddset)(struct kernel_sigset_t *set,
                                     int signum) {
    if (signum < 1 || signum > (int)(8*sizeof(set->sig))) {
      LSS_ERRNO = EINVAL;
      return -1;
    } else {
      set->sig[(signum - 1)/(8*sizeof(set->sig[0]))]
          |= 1UL << ((signum - 1) % (8*sizeof(set->sig[0])));
      return 0;
    }
  }

  LSS_INLINE int LSS_NAME(sigdelset)(struct kernel_sigset_t *set,
                                        int signum) {
    if (signum < 1 || signum > (int)(8*sizeof(set->sig))) {
      LSS_ERRNO = EINVAL;
      return -1;
    } else {
      set->sig[(signum - 1)/(8*sizeof(set->sig[0]))]
          &= ~(1UL << ((signum - 1) % (8*sizeof(set->sig[0]))));
      return 0;
    }
  }

  #if defined(__i386__) || \
      defined(__arm__) || \
     (defined(__mips__) && _MIPS_SIM == _MIPS_SIM_ABI32) || defined(__PPC__)
    #define __NR__sigaction   __NR_sigaction
    #define __NR__sigprocmask __NR_sigprocmask
    LSS_INLINE _syscall2(int, fstat64,             int, f,
                         struct kernel_stat64 *, b)
    LSS_INLINE _syscall5(int, _llseek,     uint, fd, ulong, hi, ulong, lo,
                         loff_t *, res, uint, wh)
#ifdef __PPC64__
    LSS_INLINE _syscall6(void*, mmap,              void*, s,
                         size_t,                   l, int,               p,
                         int,                      f, int,               d,
                         off_t,                    o)
#else
    #ifndef __ARM_EABI__
    
    LSS_INLINE _syscall1(void*, mmap,              void*, a)
    #endif
    LSS_INLINE _syscall6(void*, mmap2,             void*, s,
                         size_t,                   l, int,               p,
                         int,                      f, int,               d,
                         off_t,                    o)
#endif
    LSS_INLINE _syscall3(int,   _sigaction,        int,   s,
                         const struct kernel_old_sigaction*,  a,
                         struct kernel_old_sigaction*,        o)
    LSS_INLINE _syscall3(int,   _sigprocmask,      int,   h,
                         const unsigned long*,     s,
                         unsigned long*,           o)
    LSS_INLINE _syscall2(int, stat64,              const char *, p,
                         struct kernel_stat64 *, b)

    LSS_INLINE int LSS_NAME(sigaction)(int signum,
                                       const struct kernel_sigaction *act,
                                       struct kernel_sigaction *oldact) {
      int old_errno = LSS_ERRNO;
      int rc;
      struct kernel_sigaction a;
      if (act != NULL) {
        a             = *act;
        #ifdef __i386__
        if (!(a.sa_flags & SA_RESTORER)) {
          a.sa_flags   |= SA_RESTORER;
          a.sa_restorer = (a.sa_flags & SA_SIGINFO)
                          ? LSS_NAME(restore_rt)() : LSS_NAME(restore)();
        }
        #endif
      }
      rc = LSS_NAME(rt_sigaction)(signum, act ? &a : act, oldact,
                                  (KERNEL_NSIG+7)/8);
      if (rc < 0 && LSS_ERRNO == ENOSYS) {
        struct kernel_old_sigaction oa, ooa, *ptr_a = &oa, *ptr_oa = &ooa;
        if (!act) {
          ptr_a            = NULL;
        } else {
          oa.sa_handler_   = act->sa_handler_;
          memcpy(&oa.sa_mask, &act->sa_mask, sizeof(oa.sa_mask));
          #ifndef __mips__
          oa.sa_restorer   = act->sa_restorer;
          #endif
          oa.sa_flags      = act->sa_flags;
        }
        if (!oldact) {
          ptr_oa           = NULL;
        }
        LSS_ERRNO = old_errno;
        rc = LSS_NAME(_sigaction)(signum, ptr_a, ptr_oa);
        if (rc == 0 && oldact) {
          if (act) {
            memcpy(oldact, act, sizeof(*act));
          } else {
            memset(oldact, 0, sizeof(*oldact));
          }
          oldact->sa_handler_    = ptr_oa->sa_handler_;
          oldact->sa_flags       = ptr_oa->sa_flags;
          memcpy(&oldact->sa_mask, &ptr_oa->sa_mask, sizeof(ptr_oa->sa_mask));
          #ifndef __mips__
          oldact->sa_restorer    = ptr_oa->sa_restorer;
          #endif
        }
      }
      return rc;
    }

    LSS_INLINE int LSS_NAME(sigprocmask)(int how,
                                         const struct kernel_sigset_t *set,
                                         struct kernel_sigset_t *oldset) {
      int olderrno = LSS_ERRNO;
      int rc = LSS_NAME(rt_sigprocmask)(how, set, oldset, (KERNEL_NSIG+7)/8);
      if (rc < 0 && LSS_ERRNO == ENOSYS) {
        LSS_ERRNO = olderrno;
        if (oldset) {
          LSS_NAME(sigemptyset)(oldset);
        }
        rc = LSS_NAME(_sigprocmask)(how,
                                    set ? &set->sig[0] : NULL,
                                    oldset ? &oldset->sig[0] : NULL);
      }
      return rc;
    }
  #endif
  #if defined(__PPC__)
    #undef LSS_SC_LOADARGS_0
    #define LSS_SC_LOADARGS_0(dummy...)
    #undef LSS_SC_LOADARGS_1
    #define LSS_SC_LOADARGS_1(arg1)                                           \
        __sc_4  = (unsigned long) (arg1)
    #undef LSS_SC_LOADARGS_2
    #define LSS_SC_LOADARGS_2(arg1, arg2)                                     \
        LSS_SC_LOADARGS_1(arg1);                                              \
        __sc_5  = (unsigned long) (arg2)
    #undef LSS_SC_LOADARGS_3
    #define LSS_SC_LOADARGS_3(arg1, arg2, arg3)                               \
        LSS_SC_LOADARGS_2(arg1, arg2);                                        \
        __sc_6  = (unsigned long) (arg3)
    #undef LSS_SC_LOADARGS_4
    #define LSS_SC_LOADARGS_4(arg1, arg2, arg3, arg4)                         \
        LSS_SC_LOADARGS_3(arg1, arg2, arg3);                                  \
        __sc_7  = (unsigned long) (arg4)
    #undef LSS_SC_LOADARGS_5
    #define LSS_SC_LOADARGS_5(arg1, arg2, arg3, arg4, arg5)                   \
        LSS_SC_LOADARGS_4(arg1, arg2, arg3, arg4);                            \
        __sc_8  = (unsigned long) (arg5)
    #undef LSS_SC_BODY
    #define LSS_SC_BODY(nr, type, opt, args...)                               \
        long __sc_ret, __sc_err;                                              \
        {                                                                     \
          register unsigned long __sc_0 __asm__ ("r0") = __NR_socketcall;     \
          register unsigned long __sc_3 __asm__ ("r3") = opt;                 \
          register unsigned long __sc_4 __asm__ ("r4");                       \
          register unsigned long __sc_5 __asm__ ("r5");                       \
          register unsigned long __sc_6 __asm__ ("r6");                       \
          register unsigned long __sc_7 __asm__ ("r7");                       \
          register unsigned long __sc_8 __asm__ ("r8");                       \
          LSS_SC_LOADARGS_##nr(args);                                         \
          __asm__ __volatile__                                                \
              ("stwu 1, -48(1)\n\t"                                           \
               "stw 4, 20(1)\n\t"                                             \
               "stw 5, 24(1)\n\t"                                             \
               "stw 6, 28(1)\n\t"                                             \
               "stw 7, 32(1)\n\t"                                             \
               "stw 8, 36(1)\n\t"                                             \
               "addi 4, 1, 20\n\t"                                            \
               "sc\n\t"                                                       \
               "mfcr %0"                                                      \
                 : "=&r" (__sc_0),                                            \
                   "=&r" (__sc_3), "=&r" (__sc_4),                            \
                   "=&r" (__sc_5), "=&r" (__sc_6),                            \
                   "=&r" (__sc_7), "=&r" (__sc_8)                             \
                 : LSS_ASMINPUT_##nr                                          \
                 : "cr0", "ctr", "memory");                                   \
          __sc_ret = __sc_3;                                                  \
          __sc_err = __sc_0;                                                  \
        }                                                                     \
        LSS_RETURN(type, __sc_ret, __sc_err)

    LSS_INLINE int LSS_NAME(socket)(int domain, int type, int protocol) {
      LSS_SC_BODY(3, int, 1, domain, type, protocol);
    }
  #endif
  #if defined(__i386__) || \
      (defined(__arm__) && !defined(__ARM_EABI__)) || \
      (defined(__mips__) && _MIPS_SIM == _MIPS_SIM_ABI32)

    LSS_INLINE _syscall2(long, socketcall, int, c, unsigned long*, a)

    LSS_INLINE int LSS_NAME(socket)(int domain, int type, int protocol) {
      unsigned long args[3] = {
        (unsigned long) domain,
        (unsigned long) type,
        (unsigned long) protocol
      };
      return LSS_NAME(socketcall)(1, args);
    }
  #elif defined(__ARM_EABI__)
    LSS_INLINE _syscall3(int, socket,             int,   d,
                         int,                     t, int,       p)
  #endif
  #if defined(__i386__) || defined(__PPC__) ||                                \
     (defined(__mips__) && _MIPS_SIM == _MIPS_SIM_ABI32)
    LSS_INLINE _syscall3(pid_t, waitpid,          pid_t, p,
                         int*,              s,    int,   o)
  #endif
  #if defined(__mips__)
    LSS_INLINE int LSS_NAME(pipe)(int *p) {
      register unsigned long __v0 __asm__("$2") = __NR_pipe;
      register unsigned long __v1 __asm__("$3");
      register unsigned long __r7 __asm__("$7");
      __asm__ __volatile__ ("syscall\n"
                            : "=&r"(__v0), "=&r"(__v1), "+r" (__r7)
                            : "0"(__v0)
                            : "$8", "$9", "$10", "$11", "$12",
                              "$13", "$14", "$15", "$24", "memory");
      if (__r7) {
        LSS_ERRNO = __v0;
        return -1;
      } else {
        p[0] = __v0;
        p[1] = __v1;
        return 0;
      }
    }
  #else
    LSS_INLINE _syscall1(int,     pipe,           int *, p)
  #endif

  LSS_INLINE pid_t LSS_NAME(gettid)() {
    pid_t tid = LSS_NAME(_gettid)();
    if (tid != -1) {
      return tid;
    }
    return LSS_NAME(getpid)();
  }

  LSS_INLINE void *LSS_NAME(mremap)(void *old_address, size_t old_size,
                                    size_t new_size, int flags, ...) {
    va_list ap;
    void *new_address, *rc;
    va_start(ap, flags);
    new_address = va_arg(ap, void *);
    rc = LSS_NAME(_mremap)(old_address, old_size, new_size,
                           flags, new_address);
    va_end(ap);
    return rc;
  }

  LSS_INLINE int LSS_NAME(ptrace_detach)(pid_t pid) {
    int rc, err;
    LSS_NAME(sched_yield)();
    rc = LSS_NAME(ptrace)(PTRACE_DETACH, pid, (void *)0, (void *)0);
    err = LSS_ERRNO;
    LSS_NAME(kill)(pid, SIGCONT);
    LSS_ERRNO = err;
    return rc;
  }
#endif

#if defined(__cplusplus) && !defined(SYS_CPLUSPLUS)
}
#endif

#endif
#endif
