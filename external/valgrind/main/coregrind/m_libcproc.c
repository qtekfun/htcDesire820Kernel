
 
/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Julian Seward 
      jseward@acm.org

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

#include "pub_core_basics.h"
#include "pub_core_machine.h"    
#include "pub_core_vki.h"
#include "pub_core_vkiscnums.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcproc.h"
#include "pub_core_libcsignal.h"
#include "pub_core_seqmatch.h"
#include "pub_core_mallocfree.h"
#include "pub_core_syscall.h"
#include "pub_core_xarray.h"
#include "pub_core_clientstate.h"

#if defined(VGO_darwin)
#include <mach/mach.h>   
#endif



Char** VG_(client_envp) = NULL;

const Char *VG_(libdir) = VG_LIBDIR;

const Char *VG_(LD_PRELOAD_var_name) =
#if defined(VGO_linux)
   "LD_PRELOAD";
#elif defined(VGO_darwin)
   "DYLD_INSERT_LIBRARIES";
#else
#  error Unknown OS
#endif

Char *VG_(getenv)(Char *varname)
{
   Int i, n;
   vg_assert( VG_(client_envp) );
   n = VG_(strlen)(varname);
   for (i = 0; VG_(client_envp)[i] != NULL; i++) {
      Char* s = VG_(client_envp)[i];
      if (VG_(strncmp)(varname, s, n) == 0 && s[n] == '=') {
         return & s[n+1];
      }
   }
   return NULL;
}

void  VG_(env_unsetenv) ( Char **env, const Char *varname )
{
   Char **from, **to;
   vg_assert(env);
   vg_assert(varname);
   to = NULL;
   Int len = VG_(strlen)(varname);

   for (from = to = env; from && *from; from++) {
      if (!(VG_(strncmp)(varname, *from, len) == 0 && (*from)[len] == '=')) {
	 *to = *from;
	 to++;
      }
   }
   *to = *from;
}

Char **VG_(env_setenv) ( Char ***envp, const Char* varname, const Char *val )
{
   Char **env = (*envp);
   Char **cpp;
   Int len = VG_(strlen)(varname);
   Char *valstr = VG_(arena_malloc)(VG_AR_CORE, "libcproc.es.1",
                                    len + VG_(strlen)(val) + 2);
   Char **oldenv = NULL;

   VG_(sprintf)(valstr, "%s=%s", varname, val);

   for (cpp = env; cpp && *cpp; cpp++) {
      if (VG_(strncmp)(varname, *cpp, len) == 0 && (*cpp)[len] == '=') {
	 *cpp = valstr;
	 return oldenv;
      }
   }

   if (env == NULL) {
      env = VG_(arena_malloc)(VG_AR_CORE, "libcproc.es.2", sizeof(Char **) * 2);
      env[0] = valstr;
      env[1] = NULL;

      *envp = env;

   }  else {
      Int envlen = (cpp-env) + 2;
      Char **newenv = VG_(arena_malloc)(VG_AR_CORE, "libcproc.es.3",
                                        envlen * sizeof(Char **));

      for (cpp = newenv; *env; )
	 *cpp++ = *env++;
      *cpp++ = valstr;
      *cpp++ = NULL;

      oldenv = *envp;

      *envp = newenv;
   }

   return oldenv;
}


static void mash_colon_env(Char *varp, const Char *remove_pattern)
{
   Char *const start = varp;
   Char *entry_start = varp;
   Char *output = varp;

   if (varp == NULL)
      return;

   while(*varp) {
      if (*varp == ':') {
	 Char prev;
	 Bool match;


	 prev = *output;
	 *output = '\0';

	 match = VG_(string_match)(remove_pattern, entry_start);

	 *output = prev;
	 
	 if (match) {
	    output = entry_start;
	    varp++;			
	 } else
	    entry_start = output+1;	
      }

      if (*varp)
         *output++ = *varp++;
   }

   
   *output = '\0';

   
   if (VG_(string_match)(remove_pattern, entry_start)) {
      output = entry_start;
      if (output > start) {
	 
	 output--;
	 vg_assert(*output == ':');
      }
   }	 

   
   while(output < varp)
      *output++ = '\0';
}


void VG_(env_remove_valgrind_env_stuff)(Char** envp)
{

#if defined(VGO_darwin)

   
   

#endif

   Int i;
   Char* ld_preload_str = NULL;
   Char* ld_library_path_str = NULL;
   Char* dyld_insert_libraries_str = NULL;
   Char* buf;

   
   
   
   
   
   for (i = 0; envp[i] != NULL; i++) {
      if (VG_(strncmp)(envp[i], "LD_PRELOAD=", 11) == 0) {
         envp[i] = VG_(arena_strdup)(VG_AR_CORE, "libcproc.erves.1", envp[i]);
         ld_preload_str = &envp[i][11];
      }
      if (VG_(strncmp)(envp[i], "LD_LIBRARY_PATH=", 16) == 0) {
         envp[i] = VG_(arena_strdup)(VG_AR_CORE, "libcproc.erves.2", envp[i]);
         ld_library_path_str = &envp[i][16];
      }
      if (VG_(strncmp)(envp[i], "DYLD_INSERT_LIBRARIES=", 22) == 0) {
         envp[i] = VG_(arena_strdup)(VG_AR_CORE, "libcproc.erves.3", envp[i]);
         dyld_insert_libraries_str = &envp[i][22];
      }
   }

   buf = VG_(arena_malloc)(VG_AR_CORE, "libcproc.erves.4",
                           VG_(strlen)(VG_(libdir)) + 20);

   
   VG_(sprintf)(buf, "%s*/vgpreload_*.so", VG_(libdir));
   mash_colon_env(ld_preload_str, buf);
   mash_colon_env(dyld_insert_libraries_str, buf);
   VG_(sprintf)(buf, "%s*", VG_(libdir));
   mash_colon_env(ld_library_path_str, buf);

   
   VG_(env_unsetenv)(envp, VALGRIND_LAUNCHER);

   
   VG_(env_unsetenv)(envp, "DYLD_SHARED_REGION");

   

   VG_(arena_free)(VG_AR_CORE, buf);
}


Int VG_(waitpid)(Int pid, Int *status, Int options)
{
#  if defined(VGO_linux)
   SysRes res = VG_(do_syscall4)(__NR_wait4,
                                 pid, (UWord)status, options, 0);
   return sr_isError(res) ? -1 : sr_Res(res);
#  elif defined(VGO_darwin)
   SysRes res = VG_(do_syscall4)(__NR_wait4_nocancel,
                                 pid, (UWord)status, options, 0);
   return sr_isError(res) ? -1 : sr_Res(res);
#  else
#    error Unknown OS
#  endif
}

Char **VG_(env_clone) ( Char **oldenv )
{
   Char **oldenvp;
   Char **newenvp;
   Char **newenv;
   Int  envlen;

   vg_assert(oldenv);
   for (oldenvp = oldenv; oldenvp && *oldenvp; oldenvp++);

   envlen = oldenvp - oldenv + 1;
   
   newenv = VG_(arena_malloc)(VG_AR_CORE, "libcproc.ec.1",
                              envlen * sizeof(Char **));

   oldenvp = oldenv;
   newenvp = newenv;
   
   while (oldenvp && *oldenvp) {
      *newenvp++ = *oldenvp++;
   }
   
   *newenvp = *oldenvp;

   return newenv;
}

void VG_(execv) ( Char* filename, Char** argv )
{
   Char** envp;
   SysRes res;

   
   VG_(setrlimit)(VKI_RLIMIT_DATA, &VG_(client_rlimit_data));

   envp = VG_(env_clone)(VG_(client_envp));
   VG_(env_remove_valgrind_env_stuff)( envp );

   res = VG_(do_syscall3)(__NR_execve,
                          (UWord)filename, (UWord)argv, (UWord)envp);

   VG_(printf)("EXEC failed, errno = %lld\n", (Long)sr_Err(res));
}

Int VG_(system) ( Char* cmd )
{
   Int pid;
   if (cmd == NULL)
      return 1;
   pid = VG_(fork)();
   if (pid < 0)
      return -1;
   if (pid == 0) {
      
      Char* argv[4] = { "/bin/sh", "-c", cmd, 0 };
      VG_(execv)(argv[0], argv);

      
      VG_(exit)(1);
   } else {
      
      Int ir, zzz;
      vki_sigaction_toK_t sa, sa2;
      vki_sigaction_fromK_t saved_sa;
      VG_(memset)( &sa, 0, sizeof(sa) );
      VG_(sigemptyset)(&sa.sa_mask);
      sa.ksa_handler = VKI_SIG_DFL;
      sa.sa_flags    = 0;
      ir = VG_(sigaction)(VKI_SIGCHLD, &sa, &saved_sa);
      vg_assert(ir == 0);

      zzz = VG_(waitpid)(pid, NULL, 0);

      VG_(convert_sigaction_fromK_to_toK)( &saved_sa, &sa2 );
      ir = VG_(sigaction)(VKI_SIGCHLD, &sa2, NULL);
      vg_assert(ir == 0);
      return zzz == -1 ? -1 : 0;
   }
}


Int VG_(getrlimit) (Int resource, struct vki_rlimit *rlim)
{
   SysRes res = VG_(mk_SysRes_Error)(VKI_ENOSYS);
   
#  ifdef __NR_ugetrlimit
   res = VG_(do_syscall2)(__NR_ugetrlimit, resource, (UWord)rlim);
#  endif
   if (sr_isError(res) && sr_Err(res) == VKI_ENOSYS)
      res = VG_(do_syscall2)(__NR_getrlimit, resource, (UWord)rlim);
   return sr_isError(res) ? -1 : sr_Res(res);
}


Int VG_(setrlimit) (Int resource, const struct vki_rlimit *rlim)
{
   SysRes res;
   
   res = VG_(do_syscall2)(__NR_setrlimit, resource, (UWord)rlim);
   return sr_isError(res) ? -1 : sr_Res(res);
}

Int VG_(prctl) (Int option, 
                ULong arg2, ULong arg3, ULong arg4, ULong arg5)
{
   SysRes res = VG_(mk_SysRes_Error)(VKI_ENOSYS);
#  if defined(VGO_linux)
   
   res = VG_(do_syscall5)(__NR_prctl, (UWord) option,
                          (UWord) arg2, (UWord) arg3, (UWord) arg4,
                          (UWord) arg5);
#  endif

   return sr_isError(res) ? -1 : sr_Res(res);
}


Int VG_(gettid)(void)
{
#  if defined(VGO_linux)
   SysRes res = VG_(do_syscall0)(__NR_gettid);

   if (sr_isError(res) && sr_Res(res) == VKI_ENOSYS) {
      Char pid[16];      

      res = VG_(do_syscall3)(__NR_readlink, (UWord)"/proc/self",
                             (UWord)pid, sizeof(pid));
      if (!sr_isError(res) && sr_Res(res) > 0) {
         Char* s;
         pid[sr_Res(res)] = '\0';
         res = VG_(mk_SysRes_Success)(  VG_(strtoll10)(pid, &s) );
         if (*s != '\0') {
            VG_(message)(Vg_DebugMsg, 
               "Warning: invalid file name linked to by /proc/self: %s\n",
               pid);
         }
      }
   }

   return sr_Res(res);

#  elif defined(VGO_darwin)
   
   
   return mach_thread_self();

#  else
#    error "Unknown OS"
#  endif
}

Int VG_(getpid) ( void )
{
   
   return sr_Res( VG_(do_syscall0)(__NR_getpid) );
}

Int VG_(getpgrp) ( void )
{
   
   return sr_Res( VG_(do_syscall0)(__NR_getpgrp) );
}

Int VG_(getppid) ( void )
{
   
   return sr_Res( VG_(do_syscall0)(__NR_getppid) );
}

Int VG_(geteuid) ( void )
{
   
#  if defined(__NR_geteuid32)
   
   
   return sr_Res( VG_(do_syscall0)(__NR_geteuid32) );
#  else
   return sr_Res( VG_(do_syscall0)(__NR_geteuid) );
#  endif
}

Int VG_(getegid) ( void )
{
   
#  if defined(__NR_getegid32)
   
   
   return sr_Res( VG_(do_syscall0)(__NR_getegid32) );
#  else
   return sr_Res( VG_(do_syscall0)(__NR_getegid) );
#  endif
}

/* Get supplementary groups into list[0 .. size-1].  Returns the
   number of groups written, or -1 if error.  Note that in order to be
   portable, the groups are 32-bit unsigned ints regardless of the
   platform. */
Int VG_(getgroups)( Int size, UInt* list )
{
#  if defined(VGP_x86_linux) || defined(VGP_ppc32_linux)
   Int    i;
   SysRes sres;
   UShort list16[64];
   if (size < 0) return -1;
   if (size > 64) size = 64;
   sres = VG_(do_syscall2)(__NR_getgroups, size, (Addr)list16);
   if (sr_isError(sres))
      return -1;
   if (sr_Res(sres) > size)
      return -1;
   for (i = 0; i < sr_Res(sres); i++)
      list[i] = (UInt)list16[i];
   return sr_Res(sres);

#  elif defined(VGP_amd64_linux) || defined(VGP_ppc64_linux)  \
        || defined(VGP_arm_linux)                             \
        || defined(VGO_darwin) || defined(VGP_s390x_linux)    \
        || defined(VGP_mips32_linux)
   SysRes sres;
   sres = VG_(do_syscall2)(__NR_getgroups, size, (Addr)list);
   if (sr_isError(sres))
      return -1;
   return sr_Res(sres);

#  else
#     error "VG_(getgroups): needs implementation on this platform"
#  endif
}


Int VG_(ptrace) ( Int request, Int pid, void *addr, void *data )
{
   SysRes res;
   res = VG_(do_syscall4)(__NR_ptrace, request, pid, (UWord)addr, (UWord)data);
   if (sr_isError(res))
      return -1;
   return sr_Res(res);
}


Int VG_(fork) ( void )
{
#  if defined(VGO_linux)
   SysRes res;
   res = VG_(do_syscall0)(__NR_fork);
   if (sr_isError(res))
      return -1;
   return sr_Res(res);

#  elif defined(VGO_darwin)
   SysRes res;
   res = VG_(do_syscall0)(__NR_fork); 
   if (sr_isError(res))
      return -1;
   
   if (sr_ResHI(res) != 0) {
      return 0;  
   }
   return sr_Res(res);

#  else
#    error "Unknown OS"
#  endif
}


UInt VG_(read_millisecond_timer) ( void )
{
   
   static ULong base = 0;
   ULong  now;

#  if defined(VGO_linux)
   { SysRes res;
     struct vki_timespec ts_now;
     res = VG_(do_syscall2)(__NR_clock_gettime, VKI_CLOCK_MONOTONIC,
                            (UWord)&ts_now);
     if (sr_isError(res) == 0) {
        now = ts_now.tv_sec * 1000000ULL + ts_now.tv_nsec / 1000;
     } else {
       struct vki_timeval tv_now;
       res = VG_(do_syscall2)(__NR_gettimeofday, (UWord)&tv_now, (UWord)NULL);
       vg_assert(! sr_isError(res));
       now = tv_now.tv_sec * 1000000ULL + tv_now.tv_usec;
     }
   }

#  elif defined(VGO_darwin)
   
   
   
   
   { SysRes res;
     struct vki_timeval tv_now = { 0, 0 };
     res = VG_(do_syscall2)(__NR_gettimeofday, (UWord)&tv_now, (UWord)NULL);
     vg_assert(! sr_isError(res));
     now = sr_Res(res) * 1000000ULL + sr_ResHI(res);
   }

#  else
#    error "Unknown OS"
#  endif

     
   if (base == 0)
      base = now;

   return (now - base) / 1000;
}



struct atfork {
   vg_atfork_t  pre;
   vg_atfork_t  parent;
   vg_atfork_t  child;
};

#define VG_MAX_ATFORK 10

static struct atfork atforks[VG_MAX_ATFORK];
static Int n_atfork = 0;

void VG_(atfork)(vg_atfork_t pre, vg_atfork_t parent, vg_atfork_t child)
{
   Int i;

   for (i = 0; i < n_atfork; i++) {
      if (atforks[i].pre == pre &&
          atforks[i].parent == parent &&
          atforks[i].child == child)
         return;
   }

   if (n_atfork >= VG_MAX_ATFORK)
      VG_(core_panic)(
         "Too many VG_(atfork) handlers requested: raise VG_MAX_ATFORK");

   atforks[n_atfork].pre    = pre;
   atforks[n_atfork].parent = parent;
   atforks[n_atfork].child  = child;

   n_atfork++;
}

void VG_(do_atfork_pre)(ThreadId tid)
{
   Int i;

   for (i = 0; i < n_atfork; i++)
      if (atforks[i].pre != NULL)
         (*atforks[i].pre)(tid);
}

void VG_(do_atfork_parent)(ThreadId tid)
{
   Int i;

   for (i = 0; i < n_atfork; i++)
      if (atforks[i].parent != NULL)
         (*atforks[i].parent)(tid);
}

void VG_(do_atfork_child)(ThreadId tid)
{
   Int i;

   for (i = 0; i < n_atfork; i++)
      if (atforks[i].child != NULL)
         (*atforks[i].child)(tid);
}



void VG_(invalidate_icache) ( void *ptr, SizeT nbytes )
{
#  if defined(VGA_ppc32) || defined(VGA_ppc64)
   Addr startaddr = (Addr) ptr;
   Addr endaddr   = startaddr + nbytes;
   Addr cls;
   Addr addr;
   VexArchInfo vai;

   if (nbytes == 0) return;
   vg_assert(nbytes > 0);

   VG_(machine_get_VexArchInfo)( NULL, &vai );
   cls = vai.ppc_cache_line_szB;

   
   vg_assert(cls == 32 || cls == 64 || cls == 128);

   startaddr &= ~(cls - 1);
   for (addr = startaddr; addr < endaddr; addr += cls) {
      __asm__ __volatile__("dcbst 0,%0" : : "r" (addr));
   }
   __asm__ __volatile__("sync");
   for (addr = startaddr; addr < endaddr; addr += cls) {
      __asm__ __volatile__("icbi 0,%0" : : "r" (addr));
   }
   __asm__ __volatile__("sync; isync");

#  elif defined(VGA_x86)
   

#  elif defined(VGA_amd64)
   

#  elif defined(VGA_s390x)
   

#  elif defined(VGP_arm_linux)
   
   Addr startaddr = (Addr) ptr;
   Addr endaddr   = startaddr + nbytes;
   VG_(do_syscall2)(__NR_ARM_cacheflush, startaddr, endaddr);

#  elif defined(VGA_mips32)
   SysRes sres = VG_(do_syscall3)(__NR_cacheflush, (UWord) ptr,
                                 (UWord) nbytes, (UWord) 3);
   vg_assert( sres._isError == 0 );

#  else
#    error "Unknown ARCH"
#  endif
}


