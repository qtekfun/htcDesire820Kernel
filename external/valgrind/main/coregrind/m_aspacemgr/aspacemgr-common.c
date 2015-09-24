

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2006-2012 OpenWorks LLP
      info@open-works.co.uk

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


#include "priv_aspacemgr.h"
#include "config.h"




__attribute__ ((noreturn))
void ML_(am_exit)( Int status )
{
#  if defined(VGO_linux)
   (void)VG_(do_syscall1)(__NR_exit_group, status);
#  endif
   (void)VG_(do_syscall1)(__NR_exit, status);
   
   
   *(volatile Int *)0 = 'x';
   aspacem_assert(2+2 == 5);
}

void ML_(am_barf) ( HChar* what )
{
   VG_(debugLog)(0, "aspacem", "Valgrind: FATAL: %s\n", what);
   VG_(debugLog)(0, "aspacem", "Exiting now.\n");
   ML_(am_exit)(1);
}

void ML_(am_barf_toolow) ( HChar* what )
{
   VG_(debugLog)(0, "aspacem", 
                    "Valgrind: FATAL: %s is too low.\n", what);
   VG_(debugLog)(0, "aspacem", "  Increase it and rebuild.  "
                               "Exiting now.\n");
   ML_(am_exit)(1);
}

void ML_(am_assert_fail)( const HChar* expr,
                          const Char* file,
                          Int line, 
                          const Char* fn )
{
   VG_(debugLog)(0, "aspacem", 
                    "Valgrind: FATAL: aspacem assertion failed:\n");
   VG_(debugLog)(0, "aspacem", "  %s\n", expr);
   VG_(debugLog)(0, "aspacem", "  at %s:%d (%s)\n", file,line,fn);
   VG_(debugLog)(0, "aspacem", "Exiting now.\n");
   ML_(am_exit)(1);
}

Int ML_(am_getpid)( void )
{
   SysRes sres = VG_(do_syscall0)(__NR_getpid);
   aspacem_assert(!sr_isError(sres));
   return sr_Res(sres);
}



static void local_add_to_aspacem_sprintf_buf ( HChar c, void *p )
{
   HChar** aspacem_sprintf_ptr = p;
   *(*aspacem_sprintf_ptr)++ = c;
}

static
UInt local_vsprintf ( HChar* buf, const HChar *format, va_list vargs )
{
   Int ret;
   Char *aspacem_sprintf_ptr = buf;

   ret = VG_(debugLog_vprintf)
            ( local_add_to_aspacem_sprintf_buf, 
              &aspacem_sprintf_ptr, format, vargs );
   local_add_to_aspacem_sprintf_buf('\0', &aspacem_sprintf_ptr);

   return ret;
}

UInt ML_(am_sprintf) ( HChar* buf, const HChar *format, ... )
{
   UInt ret;
   va_list vargs;

   va_start(vargs,format);
   ret = local_vsprintf(buf, format, vargs);
   va_end(vargs);

   return ret;
}




SysRes VG_(am_do_mmap_NO_NOTIFY)( Addr start, SizeT length, UInt prot, 
                                  UInt flags, Int fd, Off64T offset)
{
   SysRes res;
   aspacem_assert(VG_IS_PAGE_ALIGNED(offset));
#  if defined(VGP_x86_linux) || defined(VGP_ppc32_linux) \
      || defined(VGP_arm_linux)
   
   aspacem_assert((offset % 4096) == 0);
   res = VG_(do_syscall6)(__NR_mmap2, (UWord)start, length,
                          prot, flags, fd, offset / 4096);
#  elif defined(VGP_amd64_linux) || defined(VGP_ppc64_linux) \
        || defined(VGP_s390x_linux) || defined(VGP_mips32_linux)
   res = VG_(do_syscall6)(__NR_mmap, (UWord)start, length, 
                         prot, flags, fd, offset);
#  elif defined(VGP_x86_darwin)
   if (fd == 0  &&  (flags & VKI_MAP_ANONYMOUS)) {
       fd = -1;  
   }
   res = VG_(do_syscall7)(__NR_mmap, (UWord)start, length,
                          prot, flags, fd, offset & 0xffffffff, offset >> 32);
#  elif defined(VGP_amd64_darwin)
   if (fd == 0  &&  (flags & VKI_MAP_ANONYMOUS)) {
       fd = -1;  
   }
   res = VG_(do_syscall6)(__NR_mmap, (UWord)start, length,
                          prot, flags, (UInt)fd, offset);
#  else
#    error Unknown platform
#  endif
   return res;
}

static
SysRes local_do_mprotect_NO_NOTIFY(Addr start, SizeT length, UInt prot)
{
   return VG_(do_syscall3)(__NR_mprotect, (UWord)start, length, prot );
}

SysRes ML_(am_do_munmap_NO_NOTIFY)(Addr start, SizeT length)
{
   return VG_(do_syscall2)(__NR_munmap, (UWord)start, length );
}

#if HAVE_MREMAP

SysRes ML_(am_do_extend_mapping_NO_NOTIFY)( 
          Addr  old_addr, 
          SizeT old_len,
          SizeT new_len 
       )
{
#  if defined(VGO_linux)
   return VG_(do_syscall5)(
             __NR_mremap, 
             old_addr, old_len, new_len, 
             0,
             0
          );
#  else
#    error Unknown OS
#  endif
}

SysRes ML_(am_do_relocate_nooverlap_mapping_NO_NOTIFY)( 
          Addr old_addr, Addr old_len, 
          Addr new_addr, Addr new_len 
       )
{
#  if defined(VGO_linux)
   return VG_(do_syscall5)(
             __NR_mremap, 
             old_addr, old_len, new_len, 
             VKI_MREMAP_MAYMOVE|VKI_MREMAP_FIXED,
             new_addr
          );
#  else
#    error Unknown OS
#  endif
}

#endif


SysRes ML_(am_open) ( const Char* pathname, Int flags, Int mode )
{  
   SysRes res = VG_(do_syscall3)(__NR_open, (UWord)pathname, flags, mode);
   return res;
}

Int ML_(am_read) ( Int fd, void* buf, Int count)
{
   SysRes res = VG_(do_syscall3)(__NR_read, fd, (UWord)buf, count);
   return sr_isError(res) ? -1 : sr_Res(res);
}

void ML_(am_close) ( Int fd )
{
   (void)VG_(do_syscall1)(__NR_close, fd);
}

Int ML_(am_readlink)(HChar* path, HChar* buf, UInt bufsiz)
{
   SysRes res;
   res = VG_(do_syscall3)(__NR_readlink, (UWord)path, (UWord)buf, bufsiz);
   return sr_isError(res) ? -1 : sr_Res(res);
}

Int ML_(am_fcntl) ( Int fd, Int cmd, Addr arg )
{
#  if defined(VGO_linux)
   SysRes res = VG_(do_syscall3)(__NR_fcntl, fd, cmd, arg);
#  elif defined(VGO_darwin)
   SysRes res = VG_(do_syscall3)(__NR_fcntl_nocancel, fd, cmd, arg);
#  else
#  error "Unknown OS"
#  endif
   return sr_isError(res) ? -1 : sr_Res(res);
}

Bool ML_(am_get_fd_d_i_m)( Int fd, 
                           ULong* dev, 
                           ULong* ino, UInt* mode )
{
   SysRes          res;
   struct vki_stat buf;
#  if defined(VGO_linux) && defined(__NR_fstat64)
   struct vki_stat64 buf64;
   res = VG_(do_syscall2)(__NR_fstat64, fd, (UWord)&buf64);
   if (!sr_isError(res)) {
      *dev  = (ULong)buf64.st_dev;
      *ino  = (ULong)buf64.st_ino;
      *mode = (UInt) buf64.st_mode;
      return True;
   }
#  endif
   res = VG_(do_syscall2)(__NR_fstat, fd, (UWord)&buf);
   if (!sr_isError(res)) {
      *dev  = (ULong)buf.st_dev;
      *ino  = (ULong)buf.st_ino;
      *mode = (UInt) buf.st_mode;
      return True;
   }
   return False;
}

Bool ML_(am_resolve_filename) ( Int fd, HChar* buf, Int nbuf )
{
#if defined(VGO_linux)
   Int i;
   HChar tmp[64];
   for (i = 0; i < nbuf; i++) buf[i] = 0;
   ML_(am_sprintf)(tmp, "/proc/self/fd/%d", fd);
   if (ML_(am_readlink)(tmp, buf, nbuf) > 0 && buf[0] == '/')
      return True;
   else
      return False;

#elif defined(VGO_darwin)
   HChar tmp[VKI_MAXPATHLEN+1];
   if (0 == ML_(am_fcntl)(fd, VKI_F_GETPATH, (UWord)tmp)) {
      if (nbuf > 0) {
         VG_(strncpy)( buf, tmp, nbuf < sizeof(tmp) ? nbuf : sizeof(tmp) );
         buf[nbuf-1] = 0;
      }
      if (tmp[0] == '/') return True;
   }
   return False;

#  else
#     error Unknown OS
#  endif
}






VgStack* VG_(am_alloc_VgStack)( Addr* initial_sp )
{
   Int      szB;
   SysRes   sres;
   VgStack* stack;
   UInt*    p;
   Int      i;

   
   szB = VG_STACK_GUARD_SZB 
         + VG_STACK_ACTIVE_SZB + VG_STACK_GUARD_SZB;

   sres = VG_(am_mmap_anon_float_valgrind)( szB );
   if (sr_isError(sres))
      return NULL;

   stack = (VgStack*)(AddrH)sr_Res(sres);

   aspacem_assert(VG_IS_PAGE_ALIGNED(szB));
   aspacem_assert(VG_IS_PAGE_ALIGNED(stack));

   
   sres = local_do_mprotect_NO_NOTIFY( 
             (Addr) &stack[0], 
             VG_STACK_GUARD_SZB, VKI_PROT_NONE 
          );
   if (sr_isError(sres)) goto protect_failed;
   VG_(am_notify_mprotect)( 
      (Addr) &stack->bytes[0], 
      VG_STACK_GUARD_SZB, VKI_PROT_NONE 
   );

   sres = local_do_mprotect_NO_NOTIFY( 
             (Addr) &stack->bytes[VG_STACK_GUARD_SZB + VG_STACK_ACTIVE_SZB], 
             VG_STACK_GUARD_SZB, VKI_PROT_NONE 
          );
   if (sr_isError(sres)) goto protect_failed;
   VG_(am_notify_mprotect)( 
      (Addr) &stack->bytes[VG_STACK_GUARD_SZB + VG_STACK_ACTIVE_SZB],
      VG_STACK_GUARD_SZB, VKI_PROT_NONE 
   );


   p = (UInt*)&stack->bytes[VG_STACK_GUARD_SZB];
   for (i = 0; i < VG_STACK_ACTIVE_SZB/sizeof(UInt); i++)
      p[i] = 0xDEADBEEF;

   *initial_sp = (Addr)&stack->bytes[VG_STACK_GUARD_SZB + VG_STACK_ACTIVE_SZB];
   *initial_sp -= 8;
   *initial_sp &= ~((Addr)0x1F); 

   VG_(debugLog)( 1,"aspacem","allocated thread stack at 0x%llx size %d\n",
                  (ULong)(Addr)stack, szB);
   ML_(am_do_sanity_check)();
   return stack;

  protect_failed:
   (void)ML_(am_do_munmap_NO_NOTIFY)( (Addr)stack, szB );
   ML_(am_do_sanity_check)();
   return NULL;
}



SizeT VG_(am_get_VgStack_unused_szB)( VgStack* stack, SizeT limit )
{
   SizeT i;
   UInt* p;

   p = (UInt*)&stack->bytes[VG_STACK_GUARD_SZB];
   for (i = 0; i < VG_STACK_ACTIVE_SZB/sizeof(UInt); i++) {
      if (p[i] != 0xDEADBEEF)
         break;
      if (i * sizeof(UInt) >= limit)
         break;
   }

   return i * sizeof(UInt);
}


