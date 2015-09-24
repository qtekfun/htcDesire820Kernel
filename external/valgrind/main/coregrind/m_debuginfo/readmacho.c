

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2005-2012 Apple Inc.
      Greg Parker gparker@apple.com

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

#if defined(VGO_darwin)

#include "pub_core_basics.h"
#include "pub_core_vki.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcprint.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcfile.h"
#include "pub_core_libcproc.h"
#include "pub_core_aspacemgr.h"    
#include "pub_core_machine.h"      
#include "pub_core_options.h"
#include "pub_core_oset.h"
#include "pub_core_tooliface.h"    
#include "pub_core_xarray.h"
#include "pub_core_clientstate.h"
#include "pub_core_debuginfo.h"

#include "priv_d3basics.h"
#include "priv_misc.h"
#include "priv_tytypes.h"
#include "priv_storage.h"
#include "priv_readmacho.h"
#include "priv_readdwarf.h"
#include "priv_readdwarf3.h"
#include "priv_readstabs.h"

#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>

#if VG_WORDSIZE == 4
# define MAGIC MH_MAGIC
# define MACH_HEADER mach_header
# define LC_SEGMENT_CMD LC_SEGMENT
# define SEGMENT_COMMAND segment_command
# define SECTION section
# define NLIST nlist
#else
# define MAGIC MH_MAGIC_64
# define MACH_HEADER mach_header_64
# define LC_SEGMENT_CMD LC_SEGMENT_64
# define SEGMENT_COMMAND segment_command_64
# define SECTION section_64
# define NLIST nlist_64
#endif



typedef
   struct {
      UChar* img;
      SizeT  img_szB;
      UChar* macho_img;
      SizeT  macho_img_szB;
   }
   ImageInfo;


Bool ML_(is_macho_object_file)( const void* buf, SizeT szB )
{


   const struct fat_header*  fh_be = buf;
   const struct MACH_HEADER* mh    = buf;

   vg_assert(buf);
   if (szB < sizeof(struct fat_header))
      return False;
   if (VG_(ntohl)(fh_be->magic) == FAT_MAGIC)
      return True;

   if (szB < sizeof(struct MACH_HEADER))
      return False;
   if (mh->magic == MAGIC)
      return True;

   return False;
}


static void unmap_image ( ImageInfo* ii )
{
   SysRes sres;
   vg_assert(ii->img);
   vg_assert(ii->img_szB > 0);
   sres = VG_(am_munmap_valgrind)( (Addr)ii->img, ii->img_szB );
   vg_assert( !sr_isError(sres) );
   VG_(memset)(ii, 0, sizeof(*ii));
}


static Bool map_image_aboard ( DebugInfo* di, 
                               ImageInfo* ii, UChar* filename )
{
   VG_(memset)(ii, 0, sizeof(*ii));

   
   { SizeT  size;
     SysRes fd, sres;
     struct vg_stat stat_buf;

     fd = VG_(stat)(filename, &stat_buf);
     if (sr_isError(fd)) {
        ML_(symerr)(di, True, "Can't stat image (to determine its size)?!");
        return False;
     }
     size = stat_buf.size;

     fd = VG_(open)(filename, VKI_O_RDONLY, 0);
     if (sr_isError(fd)) {
       ML_(symerr)(di, True, "Can't open image to read symbols?!");
        return False;
     }

     sres = VG_(am_mmap_file_float_valgrind)
               ( size, VKI_PROT_READ, sr_Res(fd), 0 );
     if (sr_isError(sres)) {
        ML_(symerr)(di, True, "Can't mmap image to read symbols?!");
        return False;
     }

     VG_(close)(sr_Res(fd));

     ii->img     = (UChar*)sr_Res(sres);
     ii->img_szB = size;
   }

   { struct fat_header*  fh_be;
     struct fat_header   fh;
     struct MACH_HEADER* mh;
     
     
     
     ii->macho_img     = ii->img;
     ii->macho_img_szB = ii->img_szB;

     
     if (ii->img_szB < sizeof(struct fat_header)) {
        ML_(symerr)(di, True, "Invalid Mach-O file (0 too small).");
        goto unmap_and_fail;
     }

     
     fh_be = (struct fat_header *)ii->img;
     fh.magic = VG_(ntohl)(fh_be->magic);
     fh.nfat_arch = VG_(ntohl)(fh_be->nfat_arch);
     if (fh.magic == FAT_MAGIC) {
        
        struct fat_arch *arch_be;
        struct fat_arch arch;
        Int f;
        if (ii->img_szB < sizeof(struct fat_header)
                          + fh.nfat_arch * sizeof(struct fat_arch)) {
           ML_(symerr)(di, True, "Invalid Mach-O file (1 too small).");
           goto unmap_and_fail;
        }
        for (f = 0, arch_be = (struct fat_arch *)(fh_be+1); 
             f < fh.nfat_arch;
             f++, arch_be++) {
           Int cputype;
#          if defined(VGA_ppc)
           cputype = CPU_TYPE_POWERPC;
#          elif defined(VGA_ppc64)
           cputype = CPU_TYPE_POWERPC64;
#          elif defined(VGA_x86)
           cputype = CPU_TYPE_X86;
#          elif defined(VGA_amd64)
           cputype = CPU_TYPE_X86_64;
#          else
#            error "unknown architecture"
#          endif
           arch.cputype    = VG_(ntohl)(arch_be->cputype);
           arch.cpusubtype = VG_(ntohl)(arch_be->cpusubtype);
           arch.offset     = VG_(ntohl)(arch_be->offset);
           arch.size       = VG_(ntohl)(arch_be->size);
           if (arch.cputype == cputype) {
              if (ii->img_szB < arch.offset + arch.size) {
                 ML_(symerr)(di, True, "Invalid Mach-O file (2 too small).");
                 goto unmap_and_fail;
              }
              ii->macho_img     = ii->img + arch.offset;
              ii->macho_img_szB = arch.size;
              break;
           }
        }
        if (f == fh.nfat_arch) {
           ML_(symerr)(di, True,
                       "No acceptable architecture found in fat file.");
           goto unmap_and_fail;
        }
     }

     

     
     vg_assert(ii->img_szB >= sizeof(struct fat_header));

     if (ii->macho_img_szB < sizeof(struct MACH_HEADER)) {
        ML_(symerr)(di, True, "Invalid Mach-O file (3 too small).");
        goto unmap_and_fail;
     }

     if (ii->macho_img_szB > ii->img_szB) {
        ML_(symerr)(di, True, "Invalid Mach-O file (thin bigger than fat).");
        goto unmap_and_fail;
     }

     if (ii->macho_img >= ii->img
         && ii->macho_img + ii->macho_img_szB <= ii->img + ii->img_szB) {
        
     } else {
        ML_(symerr)(di, True, "Invalid Mach-O file (thin not inside fat).");
        goto unmap_and_fail;
     }

     mh = (struct MACH_HEADER *)ii->macho_img;
     if (mh->magic != MAGIC) {
        ML_(symerr)(di, True, "Invalid Mach-O file (bad magic).");
        goto unmap_and_fail;
     }

     if (ii->macho_img_szB < sizeof(struct MACH_HEADER) + mh->sizeofcmds) {
        ML_(symerr)(di, True, "Invalid Mach-O file (4 too small).");
        goto unmap_and_fail;
     }
   }

   vg_assert(ii->img);
   vg_assert(ii->macho_img);
   vg_assert(ii->img_szB > 0);
   vg_assert(ii->macho_img_szB > 0);
   vg_assert(ii->macho_img >= ii->img);
   vg_assert(ii->macho_img + ii->macho_img_szB <= ii->img + ii->img_szB);
   return True;  
   

  unmap_and_fail:
   unmap_image(ii);
   return False; 
}



static
void read_symtab( XArray*  syms,
                  struct _DebugInfo* di, 
                  struct NLIST* o_symtab, UInt o_symtab_count,
                  UChar*     o_strtab, UInt o_strtab_sz )
{
   Int    i;
   Addr   sym_addr;
   DiSym  disym;
   UChar* name;

   static UChar* s_a_t_v = NULL; 

   for (i = 0; i < o_symtab_count; i++) {
      struct NLIST *nl = o_symtab+i;
      if ((nl->n_type & N_TYPE) == N_SECT) {
         sym_addr = di->text_bias + nl->n_value;
      } else {
         continue;
      }
      
      if (di->trace_symtab)
         VG_(printf)("nlist raw: avma %010lx  %s\n",
                     sym_addr, o_strtab + nl->n_un.n_strx );

      if (sym_addr <= di->text_avma
          || sym_addr >= di->text_avma+di->text_size) {
         continue;
      }

      name = o_strtab + nl->n_un.n_strx;
      if (name < o_strtab || name >= o_strtab + o_strtab_sz)
         continue;

      if (*name == 0)
         continue;

      disym.addr      = sym_addr;
      disym.tocptr    = 0;
      disym.pri_name  = ML_(addStr)(di, name, -1);
      disym.sec_names = NULL;
      disym.size      = 
                        di->text_avma+di->text_size - sym_addr;
      disym.isText    = True;
      disym.isIFunc   = False;
      
      
      
      
      
      
      if (disym.pri_name[0] == '_') {
         disym.pri_name++;
      } 
      else if (!VG_(clo_show_below_main) && VG_STREQ(disym.pri_name, "start")) {
         if (s_a_t_v == NULL)
            s_a_t_v = ML_(addStr)(di, "start_according_to_valgrind", -1);
         vg_assert(s_a_t_v);
         disym.pri_name = s_a_t_v;
      }

      vg_assert(disym.pri_name);
      VG_(addToXA)( syms, &disym );
   }
}


static Int cmp_DiSym_by_start_then_name ( void* v1, void* v2 )
{
   DiSym* s1 = (DiSym*)v1;
   DiSym* s2 = (DiSym*)v2;
   if (s1->addr < s2->addr) return -1;
   if (s1->addr > s2->addr) return 1;
   return VG_(strcmp)(s1->pri_name, s2->pri_name);
}

static void tidy_up_cand_syms ( XArray*  syms,
                                Bool trace_symtab )
{
   Word nsyms, i, j, k, m;

   nsyms = VG_(sizeXA)(syms);

   VG_(setCmpFnXA)(syms, cmp_DiSym_by_start_then_name);
   VG_(sortXA)(syms);


   for (i = 0; i < nsyms; i++) {
      for (k = i+1;
           k < nsyms
             && ((DiSym*)VG_(indexXA)(syms,i))->addr
                 == ((DiSym*)VG_(indexXA)(syms,k))->addr;
           k++)
         ;
      if (k < nsyms) {
         DiSym* next = (DiSym*)VG_(indexXA)(syms,k);
         for (m = i; m < k; m++) {
            DiSym* here = (DiSym*)VG_(indexXA)(syms,m);
            vg_assert(here->addr < next->addr);
            if (here->addr + here->size > next->addr)
               here->size = next->addr - here->addr;
         }
      }
      i = k-1;
      vg_assert(i <= nsyms);
   }

   j = 0;
   if (nsyms > 0) {
      j = 1;
      for (i = 1; i < nsyms; i++) {
         DiSym *s_j1, *s_j, *s_i;
         vg_assert(j <= i);
         s_j1 = (DiSym*)VG_(indexXA)(syms, j-1);
         s_j  = (DiSym*)VG_(indexXA)(syms, j);
         s_i  = (DiSym*)VG_(indexXA)(syms, i);
         if (s_i->addr != s_j1->addr
             || s_i->size != s_j1->size
             || 0 != VG_(strcmp)(s_i->pri_name, s_j1->pri_name)) {
            *s_j = *s_i;
            j++;
         } else {
            if (trace_symtab)
               VG_(printf)("nlist cleanup: dump duplicate avma %010lx  %s\n",
                           s_i->addr, s_i->pri_name );
         }
      }
   }
   vg_assert(j >= 0 && j <= nsyms);
   VG_(dropTailXA)(syms, nsyms - j);
}



#if !defined(APPLE_DSYM_EXT_AND_SUBDIRECTORY)
#define APPLE_DSYM_EXT_AND_SUBDIRECTORY ".dSYM/Contents/Resources/DWARF/"
#endif


static Bool file_exists_p(const Char *path)
{
   struct vg_stat sbuf;
   SysRes res = VG_(stat)(path, &sbuf);
   return sr_isError(res) ? False : True;
}


static Char *
find_separate_debug_file (const Char *executable_name)
{
   Char *basename_str;
   Char *dot_ptr;
   Char *slash_ptr;
   Char *dsymfile;
    
   if (VG_(strcasestr) (executable_name, ".dSYM") == NULL)
   {
      
      basename_str = VG_(basename) (executable_name);
      dsymfile = ML_(dinfo_zalloc)("di.readmacho.dsymfile", 
                    VG_(strlen) (executable_name)
                    + VG_(strlen) (APPLE_DSYM_EXT_AND_SUBDIRECTORY)
                    + VG_(strlen) (basename_str)
                    + 1
                 );
        
      
      VG_(strcpy) (dsymfile, executable_name);
      VG_(strcat) (dsymfile, APPLE_DSYM_EXT_AND_SUBDIRECTORY);
      VG_(strcat) (dsymfile, basename_str);
        
      if (file_exists_p (dsymfile))
         return dsymfile;
        
      VG_(strcpy) (dsymfile, VG_(dirname) (executable_name));
      while ((dot_ptr = VG_(strrchr) (dsymfile, '.')))
      {
         slash_ptr = VG_(strchr) (dot_ptr, '/');
         if (slash_ptr)
         {
            *slash_ptr = '\0';
            VG_(strcat) (slash_ptr, APPLE_DSYM_EXT_AND_SUBDIRECTORY);
            VG_(strcat) (slash_ptr, basename_str);
            if (file_exists_p (dsymfile))
               return dsymfile;
         }
         
         *dot_ptr = '\0';
         VG_(strcat) (dot_ptr, APPLE_DSYM_EXT_AND_SUBDIRECTORY);
         VG_(strcat) (dot_ptr, basename_str);
         if (file_exists_p (dsymfile))
            return dsymfile;
         
         *dot_ptr = '\0';
         
         slash_ptr = VG_(strrchr) (dsymfile, '/');
         if (!slash_ptr)
            break;
         *slash_ptr = '\0';
      }
   }

   return NULL;
}


static UChar *getsectdata(UChar* base, SizeT size, 
                          Char *segname, Char *sectname,
                          Word *sect_size)
{
   struct MACH_HEADER *mh = (struct MACH_HEADER *)base;
   struct load_command *cmd;          
   Int c;

   for (c = 0, cmd = (struct load_command *)(mh+1);
        c < mh->ncmds;
        c++, cmd = (struct load_command *)(cmd->cmdsize + (Addr)cmd))
   {
      if (cmd->cmd == LC_SEGMENT_CMD) {
         struct SEGMENT_COMMAND *seg = (struct SEGMENT_COMMAND *)cmd;
         if (0 == VG_(strncmp(seg->segname, segname, sizeof(seg->segname)))) {
            struct SECTION *sects = (struct SECTION *)(seg+1);
            Int s;
            for (s = 0; s < seg->nsects; s++) {
               if (0 == VG_(strncmp(sects[s].sectname, sectname, 
                                    sizeof(sects[s].sectname)))) 
               {
                  if (sect_size) *sect_size = sects[s].size;
                  return (UChar *)(base + sects[s].offset);
               }
            }
         }
      }
   }

   if (sect_size) *sect_size = 0;
   return 0;
}


static Bool check_uuid_matches ( Addr imgA, Word n_img, UChar* uuid )
{
   Word   i;
   UChar* img = (UChar*)imgA;
   UChar  first = uuid[0];
   if (n_img < 16)
      return False;
   for (i = 0; i < n_img-16; i++) {
      if (img[i] != first)
         continue;
      if (0 == VG_(memcmp)( &img[i], &uuid[0], 16 ))
         return True;
   }
   return False;
}


static Bool is_systemish_library_name ( UChar* name )
{
   vg_assert(name);
   if (0 == VG_(strncasecmp)(name, "/usr/", 5)
       || 0 == VG_(strncasecmp)(name, "/bin/", 5)
       || 0 == VG_(strncasecmp)(name, "/sbin/", 6)
       || 0 == VG_(strncasecmp)(name, "/opt/", 5)
       || 0 == VG_(strncasecmp)(name, "/sw/", 4)
       || 0 == VG_(strncasecmp)(name, "/System/", 8)
       || 0 == VG_(strncasecmp)(name, "/Library/", 9)
       || 0 == VG_(strncasecmp)(name, "/Applications/", 14)) {
      return True;
   } else {
      return False;
   }
}


Bool ML_(read_macho_debug_info)( struct _DebugInfo* di )
{
   struct symtab_command *symcmd = NULL;
   struct dysymtab_command *dysymcmd = NULL;
   HChar* dsymfilename = NULL;
   Bool have_uuid = False;
   UChar uuid[16];
   ImageInfo ii;  
   ImageInfo iid; 
   Bool ok;
   Word i;
   struct _DebugInfoMapping* rx_map = NULL;
   struct _DebugInfoMapping* rw_map = NULL;


   vg_assert(di->fsm.have_rx_map);
   vg_assert(di->fsm.have_rw_map);

   for (i = 0; i < VG_(sizeXA)(di->fsm.maps); i++) {
      struct _DebugInfoMapping* map = VG_(indexXA)(di->fsm.maps, i);
      if (map->rx && !rx_map)
         rx_map = map;
      if (map->rw && !rw_map)
         rw_map = map;
      if (rx_map && rw_map)
         break;
   }
   vg_assert(rx_map);
   vg_assert(rw_map);

   if (VG_(clo_verbosity) > 1)
      VG_(message)(Vg_DebugMsg,
                   "%s (rx at %#lx, rw at %#lx)\n", di->fsm.filename,
                   rx_map->avma, rw_map->avma );

   VG_(memset)(&ii,   0, sizeof(ii));
   VG_(memset)(&iid,  0, sizeof(iid));
   VG_(memset)(&uuid, 0, sizeof(uuid));

   ok = map_image_aboard( di, &ii, di->fsm.filename );
   if (!ok) goto fail;

   vg_assert(ii.macho_img != NULL && ii.macho_img_szB > 0);

   
   
   
   
   
   

   di->text_bias = 0;

   { struct MACH_HEADER *mh = (struct MACH_HEADER *)ii.macho_img;
      struct load_command *cmd;
      Int c;

      for (c = 0, cmd = (struct load_command *)(mh+1);
           c < mh->ncmds;
           c++, cmd = (struct load_command *)(cmd->cmdsize
                                              + (unsigned long)cmd)) {
         if (cmd->cmd == LC_SYMTAB) {
            symcmd = (struct symtab_command *)cmd;
         } 
         else if (cmd->cmd == LC_DYSYMTAB) {
            dysymcmd = (struct dysymtab_command *)cmd;
         } 
         else if (cmd->cmd == LC_ID_DYLIB && mh->filetype == MH_DYLIB) {
            
            struct dylib_command *dcmd = (struct dylib_command *)cmd;
            UChar *dylibname = dcmd->dylib.name.offset + (UChar *)dcmd;
            UChar *soname = VG_(strrchr)(dylibname, '/');
            if (!soname) soname = dylibname;
            else soname++;
            di->soname = ML_(dinfo_strdup)("di.readmacho.dylibname",
                                           soname);
         }
         else if (cmd->cmd==LC_ID_DYLINKER  &&  mh->filetype==MH_DYLINKER) {
            struct dylinker_command *dcmd = (struct dylinker_command *)cmd;
            UChar *dylinkername = dcmd->name.offset + (UChar *)dcmd;
            UChar *soname = VG_(strrchr)(dylinkername, '/');
            if (!soname) soname = dylinkername;
            else soname++;
            di->soname = ML_(dinfo_strdup)("di.readmacho.dylinkername",
                                           soname);
         }

         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         else if (cmd->cmd == LC_SEGMENT_CMD) {
            struct SEGMENT_COMMAND *seg = (struct SEGMENT_COMMAND *)cmd;
            
            if (!di->text_present
                && 0 == VG_(strcmp)(seg->segname, "__TEXT")
                
                && seg->fileoff == 0 && seg->filesize != 0) {
               di->text_present = True;
               di->text_svma = (Addr)seg->vmaddr;
               di->text_avma = rx_map->avma;
               di->text_size = seg->vmsize;
               di->text_bias = di->text_avma - di->text_svma;
               di->text_debug_svma = di->text_svma;
               di->text_debug_bias = di->text_bias;
            }
            
            if (!di->data_present
                && 0 == VG_(strcmp)(seg->segname, "__DATA")
                 && seg->filesize != 0) {
               di->data_present = True;
               di->data_svma = (Addr)seg->vmaddr;
               di->data_avma = rw_map->avma;
               di->data_size = seg->vmsize;
               di->data_bias = di->data_avma - di->data_svma;
               di->data_debug_svma = di->data_svma;
               di->data_debug_bias = di->data_bias;
            }
         }
         else if (cmd->cmd == LC_UUID) {
             struct uuid_command *uuid_cmd = (struct uuid_command *)cmd;
             VG_(memcpy)(uuid, uuid_cmd->uuid, sizeof(uuid));
             have_uuid = True;
         }
      }
   }

   if (!di->soname) {
      di->soname = ML_(dinfo_strdup)("di.readmacho.noname", "NONE");
   }

   if (di->trace_symtab) {
      VG_(printf)("\n");
      VG_(printf)("SONAME = %s\n", di->soname);
      VG_(printf)("\n");
   }

   

   if (ii.macho_img && ii.macho_img_szB > 0 && symcmd && dysymcmd) {

      
      struct NLIST *syms;
      UChar *strs;
      XArray*  candSyms = NULL;
      Word nCandSyms;

      if (ii.macho_img_szB < symcmd->stroff + symcmd->strsize
          || ii.macho_img_szB < symcmd->symoff + symcmd->nsyms
                                                 * sizeof(struct NLIST)) {
         ML_(symerr)(di, False, "Invalid Mach-O file (5 too small).");
         goto fail;
      }   
      if (dysymcmd->ilocalsym + dysymcmd->nlocalsym > symcmd->nsyms
          || dysymcmd->iextdefsym + dysymcmd->nextdefsym > symcmd->nsyms) {
         ML_(symerr)(di, False, "Invalid Mach-O file (bad symbol table).");
         goto fail;
      }
      
      syms = (struct NLIST *)(ii.macho_img + symcmd->symoff);
      strs = (UChar *)(ii.macho_img + symcmd->stroff);
      
      if (VG_(clo_verbosity) > 1)
         VG_(message)(Vg_DebugMsg,
            "   reading syms   from primary file (%d %d)\n",
            dysymcmd->nextdefsym, dysymcmd->nlocalsym );

      candSyms = VG_(newXA)(
                    ML_(dinfo_zalloc), "di.readmacho.candsyms.1",
                    ML_(dinfo_free), sizeof(DiSym)
                 );
      vg_assert(candSyms);

      
      read_symtab(candSyms,
                  di, 
                  syms + dysymcmd->iextdefsym, dysymcmd->nextdefsym, 
                  strs, symcmd->strsize);
      
      read_symtab(candSyms,
                  di, 
                  syms + dysymcmd->ilocalsym, dysymcmd->nlocalsym, 
                  strs, symcmd->strsize);

      tidy_up_cand_syms( candSyms, di->trace_symtab );

      
      nCandSyms = VG_(sizeXA)( candSyms );
      for (i = 0; i < nCandSyms; i++) {
         DiSym* cand = (DiSym*) VG_(indexXA)( candSyms, i );
         vg_assert(cand->pri_name != NULL);
         vg_assert(cand->sec_names == NULL);
         if (di->trace_symtab)
            VG_(printf)("nlist final: acquire  avma %010lx-%010lx  %s\n",
                        cand->addr, cand->addr + cand->size - 1,
                        cand->pri_name );
         ML_(addSym)( di, cand );
      }
      VG_(deleteXA)( candSyms );
   }

   if (!have_uuid)
      goto success;


   dsymfilename = find_separate_debug_file( di->fsm.filename );

   
   if (dsymfilename) {
      Bool valid;

      if (VG_(clo_verbosity) > 1)
         VG_(message)(Vg_DebugMsg, "   dSYM= %s\n", dsymfilename);

      ok = map_image_aboard( di, &iid, dsymfilename );
      if (!ok) goto fail;

      
      vg_assert(have_uuid);
      valid = iid.macho_img && iid.macho_img_szB > 0 
              && check_uuid_matches( (Addr)iid.macho_img,
                                     iid.macho_img_szB, uuid );
      if (valid)
         goto read_the_dwarf;

      if (VG_(clo_verbosity) > 1)
         VG_(message)(Vg_DebugMsg, "   dSYM does not have "
                                   "correct UUID (out of date?)\n");
   }


   vg_assert(di->fsm.filename);
   if (is_systemish_library_name(di->fsm.filename))
      goto success;

   if (!VG_(clo_dsymutil)) {
      if (VG_(clo_verbosity) == 1) {
         VG_(message)(Vg_DebugMsg, "%s:\n", di->fsm.filename);
      }
      if (VG_(clo_verbosity) > 0)
         VG_(message)(Vg_DebugMsg, "%sdSYM directory %s; consider using "
                      "--dsymutil=yes\n",
                      VG_(clo_verbosity) > 1 ? "   " : "",
                      dsymfilename ? "has wrong UUID" : "is missing"); 
      goto success;
   }

   

   { Int r;
     HChar* dsymutil = "/usr/bin/dsymutil ";
     HChar* cmd = ML_(dinfo_zalloc)( "di.readmacho.tmp1", 
                                     VG_(strlen)(dsymutil)
                                     + VG_(strlen)(di->fsm.filename)
                                     + 32  );
     VG_(strcpy)(cmd, dsymutil);
     if (0) VG_(strcat)(cmd, "--verbose ");
     VG_(strcat)(cmd, "\"");
     VG_(strcat)(cmd, di->fsm.filename);
     VG_(strcat)(cmd, "\"");
     VG_(message)(Vg_DebugMsg, "run: %s\n", cmd);
     r = VG_(system)( cmd );
     if (r)
        VG_(message)(Vg_DebugMsg, "run: %s FAILED\n", dsymutil);
     ML_(dinfo_free)(cmd);
     dsymfilename = find_separate_debug_file(di->fsm.filename);
   }

   
   if (dsymfilename) {
      Bool valid;

      if (VG_(clo_verbosity) > 1)
         VG_(message)(Vg_DebugMsg, "   dsyms= %s\n", dsymfilename);

      ok = map_image_aboard( di, &iid, dsymfilename );
      if (!ok) goto fail;

      
      vg_assert(have_uuid);
      valid = iid.macho_img && iid.macho_img_szB > 0 
              && check_uuid_matches( (Addr)iid.macho_img,
                                     iid.macho_img_szB, uuid );
      if (!valid) {
         if (VG_(clo_verbosity) > 0) {
            VG_(message)(Vg_DebugMsg,
               "WARNING: did not find expected UUID %02X%02X%02X%02X"
               "-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"
               " in dSYM dir\n",
               (UInt)uuid[0], (UInt)uuid[1], (UInt)uuid[2], (UInt)uuid[3],
               (UInt)uuid[4], (UInt)uuid[5], (UInt)uuid[6], (UInt)uuid[7],
               (UInt)uuid[8], (UInt)uuid[9], (UInt)uuid[10],
               (UInt)uuid[11], (UInt)uuid[12], (UInt)uuid[13],
               (UInt)uuid[14], (UInt)uuid[15] );
            VG_(message)(Vg_DebugMsg,
                         "WARNING: for %s\n", di->fsm.filename);
         }
         unmap_image( &iid );
         goto fail;
      }
   }


  read_the_dwarf:
   if (iid.macho_img && iid.macho_img_szB > 0) {
      UChar* debug_info_img = NULL;
      Word   debug_info_sz;
      UChar* debug_abbv_img;
      Word   debug_abbv_sz;
      UChar* debug_line_img;
      Word   debug_line_sz;
      UChar* debug_str_img;
      Word   debug_str_sz;
      UChar* debug_ranges_img;
      Word   debug_ranges_sz;
      UChar* debug_loc_img;
      Word   debug_loc_sz;
      UChar* debug_name_img;
      Word   debug_name_sz;

      debug_info_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_info", &debug_info_sz);
      debug_abbv_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_abbrev", &debug_abbv_sz);
      debug_line_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_line", &debug_line_sz);
      debug_str_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_str", &debug_str_sz);
      debug_ranges_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_ranges", &debug_ranges_sz);
      debug_loc_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_loc", &debug_loc_sz);
      debug_name_img = 
          getsectdata(iid.macho_img, iid.macho_img_szB, 
                      "__DWARF", "__debug_pubnames", &debug_name_sz);
   
      if (debug_info_img) {
         if (VG_(clo_verbosity) > 1) {
            if (0)
            VG_(message)(Vg_DebugMsg,
                         "Reading dwarf3 for %s (%#lx) from %s"
                         " (%ld %ld %ld %ld %ld %ld)\n",
                         di->fsm.filename, di->text_avma, dsymfilename,
                         debug_info_sz, debug_abbv_sz, debug_line_sz, 
                         debug_str_sz, debug_ranges_sz, debug_loc_sz
                         );
            VG_(message)(Vg_DebugMsg,
               "   reading dwarf3 from dsyms file\n");
         }
         
         ML_(read_debuginfo_dwarf3) ( di,
                                      debug_info_img, debug_info_sz,
				      NULL,           0,
                                      debug_abbv_img, debug_abbv_sz,
                                      debug_line_img, debug_line_sz,
                                      debug_str_img,  debug_str_sz,
                                      NULL, 0  );

         if (VG_(needs).var_info 
             || VG_(clo_read_var_info) ) {
            ML_(new_dwarf3_reader)(
               di, debug_info_img,   debug_info_sz,
	           NULL,             0,
                   debug_abbv_img,   debug_abbv_sz,
                   debug_line_img,   debug_line_sz,
                   debug_str_img,    debug_str_sz,
                   debug_ranges_img, debug_ranges_sz,
                   debug_loc_img,    debug_loc_sz,
                   NULL, 0, 
                   NULL, 0, 
                   NULL, 0, 
                   NULL, 0  
            );
         }
      }
   }

   if (dsymfilename) ML_(dinfo_free)(dsymfilename);

  success:
   if (ii.img)
      unmap_image(&ii);
   if (iid.img)
      unmap_image(&iid);
   return True;

   

  fail:
   ML_(symerr)(di, True, "Error reading Mach-O object.");
   if (ii.img)
      unmap_image(&ii);
   if (iid.img)
      unmap_image(&iid);
   return False;
}

#endif 

