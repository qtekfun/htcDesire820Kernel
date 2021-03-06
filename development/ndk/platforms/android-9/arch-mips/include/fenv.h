/*-
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/lib/msun/arm/fenv.h,v 1.5 2005/03/16 19:03:45 das Exp $
 */

/*
   Rewritten for Android.
*/


#ifndef _FENV_H_
#define _FENV_H_

#include <sys/types.h>

__BEGIN_DECLS

typedef __uint32_t    fenv_t;
typedef __uint32_t    fexcept_t;

#define FE_INVALID      0x40
#define FE_DIVBYZERO    0x20
#define FE_OVERFLOW     0x10
#define FE_UNDERFLOW    0x08
#define FE_INEXACT      0x04
#define FE_ALL_EXCEPT   (FE_DIVBYZERO | FE_INEXACT | \
                         FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)
#define _FCSR_CAUSE_SHIFT  10
#define _ENABLE_SHIFT 5
#define _FCSR_ENABLE_MASK (FE_ALL_EXCEPT << _ENABLE_SHIFT)

#define FE_TONEAREST    0x0000
#define FE_TOWARDZERO   0x0001
#define FE_UPWARD       0x0002
#define FE_DOWNWARD     0x0003
#define _FCSR_RMODE_SHIFT 0
#define _FCSR_RMASK       0x3
extern const fenv_t    __fe_dfl_env;
#define FE_DFL_ENV    (&__fe_dfl_env)

static __inline int fegetenv(fenv_t* __envp) {
   fenv_t _fcsr = 0;
#ifdef  __mips_hard_float
   __asm__ __volatile__("cfc1 %0,$31" : "=r" (_fcsr));
#endif
   *__envp = _fcsr;
   return 0;
}

static __inline int fesetenv(const fenv_t* __envp) {
  fenv_t _fcsr = *__envp;
#ifdef  __mips_hard_float
  __asm__ __volatile__("ctc1 %0,$31" : : "r" (_fcsr));
#endif
  return 0;
}

static __inline int feclearexcept(int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  __excepts &= FE_ALL_EXCEPT;
  __fcsr &= ~(__excepts | (__excepts << _FCSR_CAUSE_SHIFT));
  fesetenv(&__fcsr);
  return 0;
}

static __inline int fegetexceptflag(fexcept_t* __flagp, int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  *__flagp = __fcsr & __excepts & FE_ALL_EXCEPT;
  return 0;
}

static __inline int fesetexceptflag(const fexcept_t* __flagp, int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  
  __excepts &= FE_ALL_EXCEPT;
  __fcsr &= ~__excepts;
  __fcsr |= *__flagp & __excepts;
  fesetenv(&__fcsr);
  return 0;
}

static __inline int feraiseexcept(int __excepts) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  
  __excepts &= FE_ALL_EXCEPT;
  
  __fcsr |= __excepts | (__excepts << _FCSR_CAUSE_SHIFT);
  fesetenv(&__fcsr);
  return 0;
}

static __inline int fetestexcept(int __excepts) {
  fexcept_t __FCSR;
  fegetenv(&__FCSR);
  return (__FCSR & __excepts & FE_ALL_EXCEPT);
}

static __inline int fegetround(void) {
  fenv_t _fcsr;
  fegetenv(&_fcsr);
  return (_fcsr & _FCSR_RMASK);
}

static __inline int fesetround(int __round) {
  fenv_t _fcsr;
  fegetenv(&_fcsr);
  _fcsr &= ~_FCSR_RMASK;
  _fcsr |= (__round & _FCSR_RMASK ) ;
  fesetenv(&_fcsr);
  return 0;
}

static __inline int feholdexcept(fenv_t* __envp) {
  fenv_t __env;
  fegetenv(&__env);
  *__envp = __env;
  __env &= ~(FE_ALL_EXCEPT | _FCSR_ENABLE_MASK);
  fesetenv(&__env);
  return 0;
}

static __inline int feupdateenv(const fenv_t* __envp) {
  fexcept_t __fcsr;
  fegetenv(&__fcsr);
  fesetenv(__envp);
  feraiseexcept(__fcsr & FE_ALL_EXCEPT);
  return 0;
}

#if __BSD_VISIBLE

static __inline int feenableexcept(int __mask) {
  fenv_t __old_fcsr, __new_fcsr;
  fegetenv(&__old_fcsr);
  __new_fcsr = __old_fcsr | (__mask & FE_ALL_EXCEPT) << _ENABLE_SHIFT;
  fesetenv(&__new_fcsr);
  return ((__old_fcsr >> _ENABLE_SHIFT) & FE_ALL_EXCEPT);
}

static __inline int fedisableexcept(int __mask) {
  fenv_t __old_fcsr, __new_fcsr;
  fegetenv(&__old_fcsr);
  __new_fcsr = __old_fcsr & ~((__mask & FE_ALL_EXCEPT) << _ENABLE_SHIFT);
  fesetenv(&__new_fcsr);
  return ((__old_fcsr >> _ENABLE_SHIFT) & FE_ALL_EXCEPT);
}

static __inline int fegetexcept(void) {
  fenv_t __fcsr;
  fegetenv(&__fcsr);
  return ((__fcsr & _FCSR_ENABLE_MASK) >> _ENABLE_SHIFT);
}

#endif 

__END_DECLS

#endif 
