

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Julian Seward 
      jseward@acm.org
   Copyright (C) 2003-2012 Jeremy Fitzhardinge
      jeremy@goop.org

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
#include "pub_core_debuglog.h"
#include "pub_core_debuginfo.h"
#include "pub_core_libcbase.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_vki.h"
#include "pub_core_libcfile.h"
#include "pub_core_seqmatch.h"
#include "pub_core_mallocfree.h"
#include "pub_core_options.h"
#include "pub_core_oset.h"
#include "pub_core_redir.h"
#include "pub_core_trampoline.h"
#include "pub_core_transtab.h"
#include "pub_core_tooliface.h"    
#include "pub_core_machine.h"      
#include "pub_core_aspacemgr.h"    
#include "pub_core_xarray.h"
#include "pub_core_clientstate.h"  
#include "pub_core_demangle.h"     
#include "pub_core_libcproc.h"     

#include "config.h" 







typedef
   struct _Spec {
      struct _Spec* next;  
      
      HChar* from_sopatt;  
      HChar* from_fnpatt;  
      Addr   to_addr;      
      Bool   isWrap;       
      Int    becTag; 
      Int    becPrio; 
      const HChar** mandatory; 
      
      Bool   mark; 
      Bool   done; 
   }
   Spec;

typedef
   struct _TopSpec {
      struct _TopSpec* next; 
      DebugInfo* seginfo;    
      Spec*      specs;      
      Bool       mark; 
   }
   TopSpec;

static TopSpec* topSpecs = NULL;



typedef
   struct {
      Addr     from_addr;   
      Addr     to_addr;     
      TopSpec* parent_spec; 
      TopSpec* parent_sym;  
      Int      becTag;      
      Int      becPrio;     
      Bool     isWrap;      
      Bool     isIFunc;     
   }
   Active;

static OSet* activeSet = NULL;

static Addr iFuncWrapper;


static void maybe_add_active ( Active  );

static void*  dinfo_zalloc(HChar* ec, SizeT);
static void   dinfo_free(void*);
static HChar* dinfo_strdup(HChar* ec, HChar*);
static Bool   is_plausible_guest_addr(Addr);

static void   show_redir_state ( HChar* who );
static void   show_active ( HChar* left, Active* act );

static void   handle_maybe_load_notifier( const UChar* soname, 
                                                HChar* symbol, Addr addr );

static void   handle_require_text_symbols ( DebugInfo* );


static 
void generate_and_add_actives ( 
        
        Spec*    specs, 
        TopSpec* parent_spec,
	
        DebugInfo* di,
        TopSpec* parent_sym 
     );


static UChar** alloc_symname_array ( UChar* pri_name, UChar** sec_names,
                                     UChar** twoslots )
{
   if (sec_names == NULL) {
      twoslots[0] = pri_name;
      twoslots[1] = NULL;
      return twoslots;
   }
   
   Word    n_req = 1;
   UChar** pp    = sec_names;
   while (*pp) { n_req++; pp++; }
   
   UChar** arr = dinfo_zalloc( "redir.asa.1", (n_req+1) * sizeof(UChar*) );
   Word    i   = 0;
   arr[i++] = pri_name;
   pp = sec_names;
   while (*pp) { arr[i++] = *pp; pp++; }
   tl_assert(i == n_req);
   tl_assert(arr[n_req] == NULL);
   return arr;
}


static void free_symname_array ( UChar** names, UChar** twoslots )
{
   if (names != twoslots)
      dinfo_free(names);
}

static HChar const* advance_to_equal ( HChar const* c ) {
   while (*c && *c != '=') {
      ++c;
   }
   return c;
}
static HChar const* advance_to_comma ( HChar const* c ) {
   while (*c && *c != ',') {
      ++c;
   }
   return c;
}


#define N_DEMANGLED 256

void VG_(redir_notify_new_DebugInfo)( DebugInfo* newdi )
{
   Bool         ok, isWrap;
   Int          i, nsyms, becTag, becPrio;
   Spec*        specList;
   Spec*        spec;
   TopSpec*     ts;
   TopSpec*     newts;
   UChar*       sym_name_pri;
   UChar**      sym_names_sec;
   Addr         sym_addr, sym_toc;
   HChar        demangled_sopatt[N_DEMANGLED];
   HChar        demangled_fnpatt[N_DEMANGLED];
   Bool         check_ppcTOCs = False;
   Bool         isText;
   const UChar* newdi_soname;

#  if defined(VG_PLAT_USES_PPCTOC)
   check_ppcTOCs = True;
#  endif

   vg_assert(newdi);
   newdi_soname = VG_(DebugInfo_get_soname)(newdi);
   vg_assert(newdi_soname != NULL);

#ifdef ENABLE_INNER
   {
      const UChar* newdi_filename = VG_(DebugInfo_get_filename)(newdi);
      const UChar* newdi_basename = VG_(basename) (newdi_filename);
      if (VG_(strncmp) (newdi_basename, "vgpreload_", 10) == 0) {
         struct vg_stat newdi_stat;
         SysRes newdi_res;
         Char in_vglib_filename[VKI_PATH_MAX];
         struct vg_stat in_vglib_stat;
         SysRes in_vglib_res;

         newdi_res = VG_(stat)(newdi_filename, &newdi_stat);
         
         VG_(strncpy) (in_vglib_filename, VG_(libdir), VKI_PATH_MAX);
         VG_(strncat) (in_vglib_filename, "/", VKI_PATH_MAX);
         VG_(strncat) (in_vglib_filename, newdi_basename, VKI_PATH_MAX);
         in_vglib_res = VG_(stat)(in_vglib_filename, &in_vglib_stat);

         if (!sr_isError(in_vglib_res)
             && !sr_isError(newdi_res)
             && (newdi_stat.dev != in_vglib_stat.dev 
                 || newdi_stat.ino != in_vglib_stat.ino)) {
            if ( VG_(clo_verbosity) > 1 ) {
               VG_(message)( Vg_DebugMsg,
                             "Skipping vgpreload redir in %s"
                             " (not from VALGRIND_LIB_INNER)\n",
                             newdi_filename);
            }
            return;
         } else {
            if ( VG_(clo_verbosity) > 1 ) {
               VG_(message)( Vg_DebugMsg,
                             "Executing vgpreload redir in %s"
                             " (from VALGRIND_LIB_INNER)\n",
                             newdi_filename);
            }
         }
      }
   }
#endif


   
   for (ts = topSpecs; ts; ts = ts->next)
      vg_assert(ts->seginfo != newdi);


   specList = NULL; 

   nsyms = VG_(DebugInfo_syms_howmany)( newdi );
   for (i = 0; i < nsyms; i++) {
      VG_(DebugInfo_syms_getidx)( newdi, i, &sym_addr, &sym_toc,
                                  NULL, &sym_name_pri, &sym_names_sec,
                                  &isText, NULL );
      
      UChar*  twoslots[2];
      UChar** names_init = alloc_symname_array(sym_name_pri, sym_names_sec,
                                               &twoslots[0]);
      UChar** names;
      for (names = names_init; *names; names++) {
         ok = VG_(maybe_Z_demangle)( *names,
                                     demangled_sopatt, N_DEMANGLED,
                                     demangled_fnpatt, N_DEMANGLED,
                                     &isWrap, &becTag, &becPrio );
         
         if (!isText)
            continue;
         if (!ok) {
            handle_maybe_load_notifier( newdi_soname, *names, sym_addr );
            continue; 
         }
         if (check_ppcTOCs && sym_toc == 0) {
            continue;
         }

         if (0 == VG_(strncmp) (demangled_sopatt, 
                                VG_SO_SYN_PREFIX, VG_SO_SYN_PREFIX_LEN)) {

            if (!VG_(clo_soname_synonyms))
               continue; 

            
            SizeT const sopatt_syn_len 
               = VG_(strlen)(demangled_sopatt+VG_SO_SYN_PREFIX_LEN);
            HChar const* last = VG_(clo_soname_synonyms);
            
            while (*last) {
               HChar const* first = last;
               last = advance_to_equal(first);
               
               if ((last - first) == sopatt_syn_len
                   && 0 == VG_(strncmp)(demangled_sopatt+VG_SO_SYN_PREFIX_LEN,
                                        first,
                                        sopatt_syn_len)) {
                  
                  first = last + 1;
                  last = advance_to_comma(first);
                  VG_(strncpy)(demangled_sopatt, first, last - first);
                  demangled_sopatt[last - first] = '\0';
                  break;
               }

               last = advance_to_comma(last);
               if (*last == ',')
                  last++;
            }
            
            
            if (0 == VG_(strncmp) (demangled_sopatt, 
                                   VG_SO_SYN_PREFIX, VG_SO_SYN_PREFIX_LEN))
               continue;
         }

         spec = dinfo_zalloc("redir.rnnD.1", sizeof(Spec));
         vg_assert(spec);
         spec->from_sopatt = dinfo_strdup("redir.rnnD.2", demangled_sopatt);
         spec->from_fnpatt = dinfo_strdup("redir.rnnD.3", demangled_fnpatt);
         vg_assert(spec->from_sopatt);
         vg_assert(spec->from_fnpatt);
         spec->to_addr = sym_addr;
         spec->isWrap = isWrap;
         spec->becTag = becTag;
         spec->becPrio = becPrio;
         
         vg_assert(is_plausible_guest_addr(sym_addr));
         spec->next = specList;
         spec->mark = False; 
         spec->done = False; 
         specList = spec;
      }
      free_symname_array(names_init, &twoslots[0]);
   }

   if (check_ppcTOCs) {
      for (i = 0; i < nsyms; i++) {
         VG_(DebugInfo_syms_getidx)( newdi, i, &sym_addr, &sym_toc,
                                     NULL, &sym_name_pri, &sym_names_sec,
                                     &isText, NULL );
         UChar*  twoslots[2];
         UChar** names_init = alloc_symname_array(sym_name_pri, sym_names_sec,
                                                  &twoslots[0]);
         UChar** names;
         for (names = names_init; *names; names++) {
            ok = isText
                 && VG_(maybe_Z_demangle)( 
                       *names, demangled_sopatt, N_DEMANGLED,
                       demangled_fnpatt, N_DEMANGLED, &isWrap, NULL, NULL );
            if (!ok)
               
               continue;
            if (sym_toc != 0)
               
               continue;

            for (spec = specList; spec; spec = spec->next) 
               if (0 == VG_(strcmp)(spec->from_sopatt, demangled_sopatt)
                   && 0 == VG_(strcmp)(spec->from_fnpatt, demangled_fnpatt))
                  break;
            if (spec)
               continue;

            
            VG_(message)(Vg_DebugMsg,
                         "WARNING: no TOC ptr for redir/wrap to %s %s\n",
                         demangled_sopatt, demangled_fnpatt);
         }
         free_symname_array(names_init, &twoslots[0]);
      }
   }

   newts = dinfo_zalloc("redir.rnnD.4", sizeof(TopSpec));
   vg_assert(newts);
   newts->next    = NULL; 
   newts->seginfo = newdi;
   newts->specs   = specList;
   newts->mark    = False; 

   
   for (ts = topSpecs; ts; ts = ts->next) {
      if (ts->seginfo)
         generate_and_add_actives( specList,    newts,
                                   ts->seginfo, ts );
   }

   
   for (ts = topSpecs; ts; ts = ts->next) {
      generate_and_add_actives( ts->specs, ts, 
                                newdi,     newts );
   }

   
   generate_and_add_actives( specList, newts, 
                             newdi,    newts );

   
   newts->next = topSpecs;
   topSpecs = newts;

   if (VG_(clo_trace_redir))
      show_redir_state("after VG_(redir_notify_new_DebugInfo)");

   handle_require_text_symbols(newdi);
}

#undef N_DEMANGLED


void VG_(redir_add_ifunc_target)( Addr old_from, Addr new_from )
{
    Active *old, new;

    old = VG_(OSetGen_Lookup)(activeSet, &old_from);
    vg_assert(old);
    vg_assert(old->isIFunc);

    new = *old;
    new.from_addr = new_from;
    new.isIFunc = False;
    maybe_add_active (new);

    if (VG_(clo_trace_redir)) {
       VG_(message)( Vg_DebugMsg,
                     "Adding redirect for indirect function "
                     "0x%llx from 0x%llx -> 0x%llx\n",
                     (ULong)old_from, (ULong)new_from, (ULong)new.to_addr );
    }
}


static 
void generate_and_add_actives ( 
        
        Spec*    specs, 
        TopSpec* parent_spec,
	
        DebugInfo* di,
        TopSpec* parent_sym 
     )
{
   Spec*   sp;
   Bool    anyMark, isText, isIFunc;
   Active  act;
   Int     nsyms, i;
   Addr    sym_addr;
   UChar*  sym_name_pri;
   UChar** sym_names_sec;

   anyMark = False;
   for (sp = specs; sp; sp = sp->next) {
      sp->done = False;
      sp->mark = VG_(string_match)( sp->from_sopatt, 
                                    VG_(DebugInfo_get_soname)(di) );
      anyMark = anyMark || sp->mark;
   }

   
   if (!anyMark)
      return;

   nsyms = VG_(DebugInfo_syms_howmany)( di );
   for (i = 0; i < nsyms; i++) {
      VG_(DebugInfo_syms_getidx)( di, i, &sym_addr, NULL,
                                  NULL, &sym_name_pri, &sym_names_sec,
                                  &isText, &isIFunc );
      UChar*  twoslots[2];
      UChar** names_init = alloc_symname_array(sym_name_pri, sym_names_sec,
                                               &twoslots[0]);
      UChar** names;
      for (names = names_init; *names; names++) {

         
         if (!isText)
            continue;

         for (sp = specs; sp; sp = sp->next) {
            if (!sp->mark)
               continue; 
            if (VG_(string_match)( sp->from_fnpatt, *names )) {
               
               act.from_addr   = sym_addr;
               act.to_addr     = sp->to_addr;
               act.parent_spec = parent_spec;
               act.parent_sym  = parent_sym;
               act.becTag      = sp->becTag;
               act.becPrio     = sp->becPrio;
               act.isWrap      = sp->isWrap;
               act.isIFunc     = isIFunc;
               sp->done = True;
               maybe_add_active( act );
            }
         } 

      } 
      free_symname_array(names_init, &twoslots[0]);
   } 

   for (sp = specs; sp; sp = sp->next) {
      if (!sp->mark)
         continue;
      if (sp->mark && (!sp->done) && sp->mandatory)
         break;
   }
   if (sp) {
      const HChar** strp;
      HChar* v = "valgrind:  ";
      vg_assert(sp->mark);
      vg_assert(!sp->done);
      vg_assert(sp->mandatory);
      VG_(printf)("\n");
      VG_(printf)(
      "%sFatal error at startup: a function redirection\n", v);
      VG_(printf)(
      "%swhich is mandatory for this platform-tool combination\n", v);
      VG_(printf)(
      "%scannot be set up.  Details of the redirection are:\n", v);
      VG_(printf)(
      "%s\n", v);
      VG_(printf)(
      "%sA must-be-redirected function\n", v);
      VG_(printf)(
      "%swhose name matches the pattern:      %s\n", v, sp->from_fnpatt);
      VG_(printf)(
      "%sin an object with soname matching:   %s\n", v, sp->from_sopatt);
      VG_(printf)(
      "%swas not found whilst processing\n", v);
      VG_(printf)(
      "%ssymbols from the object with soname: %s\n",
      v, VG_(DebugInfo_get_soname)(di));
      VG_(printf)(
      "%s\n", v);

      for (strp = sp->mandatory; *strp; strp++)
         VG_(printf)(
         "%s%s\n", v, *strp);

      VG_(printf)(
      "%s\n", v);
      VG_(printf)(
      "%sCannot continue -- exiting now.  Sorry.\n", v);
      VG_(printf)("\n");
      VG_(exit)(1);
   }
}


static void maybe_add_active ( Active act )
{
   HChar*  what    = NULL;
   Active* old     = NULL;
   Bool    add_act = False;

   if (!is_plausible_guest_addr(act.from_addr)
#      if defined(VGP_amd64_linux)
       && act.from_addr != 0xFFFFFFFFFF600000ULL
       && act.from_addr != 0xFFFFFFFFFF600400ULL
       && act.from_addr != 0xFFFFFFFFFF600800ULL
#      endif
      ) {
      what = "redirection from-address is in non-executable area";
      goto bad;
   }

   old = VG_(OSetGen_Lookup)( activeSet, &act.from_addr );
   if (old) {
      
      vg_assert(old->from_addr == act.from_addr);
      if (old->to_addr != act.to_addr) {
         vg_assert(old->becTag  >= 0 && old->becTag  <= 9999);
         vg_assert(old->becPrio >= 0 && old->becPrio <= 9);
         vg_assert(act.becTag   >= 0 && act.becTag   <= 9999);
         vg_assert(act.becPrio  >= 0 && act.becPrio  <= 9);
         if (old->becTag == 0)
            vg_assert(old->becPrio == 0);
         if (act.becTag == 0)
            vg_assert(act.becPrio == 0);

         if (old->becTag == 0 || act.becTag == 0 || old->becTag != act.becTag) {
            what = "new redirection conflicts with existing -- ignoring it";
            goto bad;
         }
         if (act.becPrio <= old->becPrio) {
            if (VG_(clo_verbosity) > 2) {
               VG_(message)(Vg_UserMsg, "Ignoring %s redirection:\n",
                            act.becPrio < old->becPrio ? "lower priority" 
                                                       : "duplicate");
               show_active(             "    old: ", old);
               show_active(             "    new: ", &act);
            }
         } else {
            if (VG_(clo_verbosity) > 1) {
               VG_(message)(Vg_UserMsg, 
                           "Preferring higher priority redirection:\n");
               show_active(             "    old: ", old);
               show_active(             "    new: ", &act);
            }
            add_act = True;
            void* oldNd = VG_(OSetGen_Remove)( activeSet, &act.from_addr );
            vg_assert(oldNd == old);
            VG_(OSetGen_FreeNode)( activeSet, old );
            old = NULL;
         }
      } else {
         
      }

   } else {
      add_act = True;
   }

   
   if (add_act) {
      Active* a = VG_(OSetGen_AllocNode)(activeSet, sizeof(Active));
      vg_assert(a);
      *a = act;
      VG_(OSetGen_Insert)(activeSet, a);
      VG_(discard_translations)( (Addr64)act.from_addr, 1,
                                 "redir_new_DebugInfo(from_addr)");
      VG_(discard_translations)( (Addr64)act.to_addr, 1,
                                 "redir_new_DebugInfo(to_addr)");
      if (VG_(clo_verbosity) > 2) {
         VG_(message)(Vg_UserMsg, "Adding active redirection:\n");
         show_active(             "    new: ", &act);
      }
   }
   return;

  bad:
   vg_assert(what);
   vg_assert(!add_act);
   if (VG_(clo_verbosity) > 1) {
      VG_(message)(Vg_UserMsg, "WARNING: %s\n", what);
      if (old) {
         show_active(             "    old: ", old);
      }
      show_active(             "    new: ", &act);
   }
}



void VG_(redir_notify_delete_DebugInfo)( DebugInfo* delsi )
{
   TopSpec* ts;
   TopSpec* tsPrev;
   Spec*    sp;
   Spec*    sp_next;
   OSet*    tmpSet;
   Active*  act;
   Bool     delMe;
   Addr     addr;

   vg_assert(delsi);

   tsPrev = NULL;
   ts     = topSpecs;
   while (True) {
     if (ts == NULL) break;
     if (ts->seginfo == delsi) break;
     tsPrev = ts;
     ts = ts->next;
   }

   vg_assert(ts); 
   vg_assert(ts->seginfo == delsi);

   tmpSet = VG_(OSetWord_Create)(dinfo_zalloc, "redir.rndD.1", dinfo_free);

   ts->mark = True;

   VG_(OSetGen_ResetIter)( activeSet );
   while ( (act = VG_(OSetGen_Next)(activeSet)) ) {
      delMe = act->parent_spec != NULL
              && act->parent_sym != NULL
              && act->parent_spec->seginfo != NULL
              && act->parent_sym->seginfo != NULL
              && (act->parent_spec->mark || act->parent_sym->mark);

      if ( (!delMe)
           && act->parent_spec != NULL
           && act->parent_sym  != NULL ) {
         if (!is_plausible_guest_addr(act->from_addr))
            delMe = True;
         if (!is_plausible_guest_addr(act->to_addr))
            delMe = True;
      }

      if (delMe) {
         VG_(OSetWord_Insert)( tmpSet, act->from_addr );
         VG_(discard_translations)( (Addr64)act->from_addr, 1,
                                    "redir_del_DebugInfo(from_addr)");
         VG_(discard_translations)( (Addr64)act->to_addr, 1,
                                    "redir_del_DebugInfo(to_addr)");
      }
   }

   
   VG_(OSetWord_ResetIter)( tmpSet );
   while ( VG_(OSetWord_Next)(tmpSet, &addr) ) {
      act = VG_(OSetGen_Remove)( activeSet, &addr );
      vg_assert(act);
      VG_(OSetGen_FreeNode)( activeSet, act );
   }

   VG_(OSetWord_Destroy)( tmpSet );

   for (sp = ts->specs; sp; sp = sp_next) {
      if (sp->from_sopatt) dinfo_free(sp->from_sopatt);
      if (sp->from_fnpatt) dinfo_free(sp->from_fnpatt);
      sp_next = sp->next;
      dinfo_free(sp);
   }

   if (tsPrev == NULL) {
      
      topSpecs = ts->next;
   } else {
      tsPrev->next = ts->next;
   }
   dinfo_free(ts);

   if (VG_(clo_trace_redir))
      show_redir_state("after VG_(redir_notify_delete_DebugInfo)");
}



Addr VG_(redir_do_lookup) ( Addr orig, Bool* isWrap )
{
   Active* r = VG_(OSetGen_Lookup)(activeSet, &orig);
   if (r == NULL)
      return orig;

   vg_assert(r->to_addr != 0);
   if (isWrap)
      *isWrap = r->isWrap || r->isIFunc;
   if (r->isIFunc) {
      vg_assert(iFuncWrapper);
      return iFuncWrapper;
   }
   return r->to_addr;
}




__attribute__((unused)) 
static void add_hardwired_active ( Addr from, Addr to )
{
   Active act;
   act.from_addr   = from;
   act.to_addr     = to;
   act.parent_spec = NULL;
   act.parent_sym  = NULL;
   act.becTag      = 0; 
   act.becPrio     = 0; 
   act.isWrap      = False;
   act.isIFunc     = False;
   maybe_add_active( act );
}



__attribute__((unused)) 
static void add_hardwired_spec ( HChar* sopatt, HChar* fnpatt, 
                                 Addr   to_addr,
                                 const HChar** mandatory )
{
   Spec* spec = dinfo_zalloc("redir.ahs.1", sizeof(Spec));
   vg_assert(spec);

   if (topSpecs == NULL) {
      topSpecs = dinfo_zalloc("redir.ahs.2", sizeof(TopSpec));
      vg_assert(topSpecs);
      
   }

   vg_assert(topSpecs != NULL);
   vg_assert(topSpecs->next == NULL);
   vg_assert(topSpecs->seginfo == NULL);
   
   spec->from_sopatt = sopatt;
   spec->from_fnpatt = fnpatt;
   spec->to_addr     = to_addr;
   spec->isWrap      = False;
   spec->mandatory   = mandatory;
   
   spec->mark        = False; 
   spec->done        = False; 

   spec->next = topSpecs->specs;
   topSpecs->specs = spec;
}


__attribute__((unused)) 
static const HChar* complain_about_stripped_glibc_ldso[]
= { "Possible fixes: (1, short term): install glibc's debuginfo",
    "package on this machine.  (2, longer term): ask the packagers",
    "for your Linux distribution to please in future ship a non-",
    "stripped ld.so (or whatever the dynamic linker .so is called)",
    "that exports the above-named function using the standard",
    "calling conventions for this platform.  The package you need",
    "to install for fix (1) is called",
    "",
    "  On Debian, Ubuntu:                 libc6-dbg",
    "  On SuSE, openSuSE, Fedora, RHEL:   glibc-debuginfo",
    NULL
  };



void VG_(redir_initialise) ( void )
{
   
   vg_assert( VG_(next_DebugInfo)(NULL) == NULL );

   
   activeSet = VG_(OSetGen_Create)(offsetof(Active, from_addr),
                                   NULL,     
                                   dinfo_zalloc,
                                   "redir.ri.1", 
                                   dinfo_free);

   

#  if defined(VGP_x86_linux)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {
      const HChar** mandatory;
#     if defined(GLIBC_2_2) || defined(GLIBC_2_3) || defined(GLIBC_2_4) \
         || defined(GLIBC_2_5) || defined(GLIBC_2_6) || defined(GLIBC_2_7) \
         || defined(GLIBC_2_8) || defined(GLIBC_2_9) \
         || defined(GLIBC_2_10) || defined(GLIBC_2_11)
      mandatory = NULL;
#     else
      mandatory = complain_about_stripped_glibc_ldso;
#     endif
      add_hardwired_spec(
         "ld-linux.so.2", "index",
         (Addr)&VG_(x86_linux_REDIR_FOR_index), mandatory);
      add_hardwired_spec(
         "ld-linux.so.2", "strlen",
         (Addr)&VG_(x86_linux_REDIR_FOR_strlen), mandatory);
   }

#  elif defined(VGP_amd64_linux)
   
   add_hardwired_active(
      0xFFFFFFFFFF600000ULL,
      (Addr)&VG_(amd64_linux_REDIR_FOR_vgettimeofday)
   );
   add_hardwired_active(
      0xFFFFFFFFFF600400ULL,
      (Addr)&VG_(amd64_linux_REDIR_FOR_vtime)
   );
   add_hardwired_active(
      0xFFFFFFFFFF600800ULL,
      (Addr)&VG_(amd64_linux_REDIR_FOR_vgetcpu)
   );

   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {

      add_hardwired_spec(
         "ld-linux-x86-64.so.2", "strlen",
         (Addr)&VG_(amd64_linux_REDIR_FOR_strlen),
#        if defined(GLIBC_2_2) || defined(GLIBC_2_3) || defined(GLIBC_2_4) \
            || defined(GLIBC_2_5) || defined(GLIBC_2_6) || defined(GLIBC_2_7) \
            || defined(GLIBC_2_8) || defined(GLIBC_2_9)
         NULL
#        else
         complain_about_stripped_glibc_ldso
#        endif
      );   
   }

#  elif defined(VGP_ppc32_linux)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {

      
      add_hardwired_spec(
         "ld.so.1", "strlen",
         (Addr)&VG_(ppc32_linux_REDIR_FOR_strlen),
         complain_about_stripped_glibc_ldso
      );   
      add_hardwired_spec(
         "ld.so.1", "strcmp",
         (Addr)&VG_(ppc32_linux_REDIR_FOR_strcmp),
         NULL 
         
      );
      add_hardwired_spec(
         "ld.so.1", "index",
         (Addr)&VG_(ppc32_linux_REDIR_FOR_strchr),
         NULL 
         
      );
   }

#  elif defined(VGP_ppc64_linux)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {

      
      add_hardwired_spec(
         "ld64.so.1", "strlen",
         (Addr)VG_(fnptr_to_fnentry)( &VG_(ppc64_linux_REDIR_FOR_strlen) ),
         complain_about_stripped_glibc_ldso
      );

      add_hardwired_spec(
         "ld64.so.1", "index",
         (Addr)VG_(fnptr_to_fnentry)( &VG_(ppc64_linux_REDIR_FOR_strchr) ),
         NULL 
         
      );
   }

#  elif defined(VGP_arm_linux)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {
      add_hardwired_spec(
         "ld-linux.so.3", "strlen",
         (Addr)&VG_(arm_linux_REDIR_FOR_strlen),
         complain_about_stripped_glibc_ldso
      );
      
      
      
      
      
      add_hardwired_spec(
         "ld-linux.so.3", "memcpy",
         (Addr)&VG_(arm_linux_REDIR_FOR_memcpy),
         complain_about_stripped_glibc_ldso
      );
   }
   

#  elif defined(VGP_x86_darwin)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {
      add_hardwired_spec("dyld", "strcmp",
                         (Addr)&VG_(x86_darwin_REDIR_FOR_strcmp), NULL);
      add_hardwired_spec("dyld", "strlen",
                         (Addr)&VG_(x86_darwin_REDIR_FOR_strlen), NULL);
      add_hardwired_spec("dyld", "strcat",
                         (Addr)&VG_(x86_darwin_REDIR_FOR_strcat), NULL);
      add_hardwired_spec("dyld", "strcpy",
                         (Addr)&VG_(x86_darwin_REDIR_FOR_strcpy), NULL);
      add_hardwired_spec("dyld", "strlcat",
                         (Addr)&VG_(x86_darwin_REDIR_FOR_strlcat), NULL);
   }

#  elif defined(VGP_amd64_darwin)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {
      add_hardwired_spec("dyld", "strcmp",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_strcmp), NULL);
      add_hardwired_spec("dyld", "strlen",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_strlen), NULL);
      add_hardwired_spec("dyld", "strcat",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_strcat), NULL);
      add_hardwired_spec("dyld", "strcpy",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_strcpy), NULL);
      add_hardwired_spec("dyld", "strlcat",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_strlcat), NULL);
      
      add_hardwired_spec("dyld", "arc4random",
                         (Addr)&VG_(amd64_darwin_REDIR_FOR_arc4random), NULL);
   }

#  elif defined(VGP_s390x_linux)
   

#  elif defined(VGP_mips32_linux)
   if (0==VG_(strcmp)("Memcheck", VG_(details).name)) {

      
      add_hardwired_spec(
         "ld.so.3", "strlen",
         (Addr)&VG_(mips32_linux_REDIR_FOR_strlen),
         complain_about_stripped_glibc_ldso
      );
   }

#  else
#    error Unknown platform
#  endif

   if (VG_(clo_trace_redir))
      show_redir_state("after VG_(redir_initialise)");
}



static void* dinfo_zalloc(HChar* ec, SizeT n) {
   void* p;
   vg_assert(n > 0);
   p = VG_(arena_malloc)(VG_AR_DINFO, ec, n);
   tl_assert(p);
   VG_(memset)(p, 0, n);
   return p;
}

static void dinfo_free(void* p) {
   tl_assert(p);
   return VG_(arena_free)(VG_AR_DINFO, p);
}

static HChar* dinfo_strdup(HChar* ec, HChar* str)
{
   return VG_(arena_strdup)(VG_AR_DINFO, ec, str);
}

static Bool is_plausible_guest_addr(Addr a)
{
   NSegment const* seg = VG_(am_find_nsegment)(a);
   return seg != NULL
          && (seg->kind == SkAnonC || seg->kind == SkFileC)
          && (seg->hasX || seg->hasR); 
}



static 
void handle_maybe_load_notifier( const UChar* soname, 
                                       HChar* symbol, Addr addr )
{
#  if defined(VGP_x86_linux)
   if (symbol && symbol[0] == '_' 
              && 0 == VG_(strcmp)(symbol, "_dl_sysinfo_int80")
              && 0 == VG_(strcmp)(soname, "ld-linux.so.2")) {
      if (VG_(client__dl_sysinfo_int80) == 0)
         VG_(client__dl_sysinfo_int80) = addr;
   }
#  endif

   vg_assert(symbol); 
   if (0 != VG_(strncmp)(symbol, VG_NOTIFY_ON_LOAD_PREFIX, 
                                 VG_NOTIFY_ON_LOAD_PREFIX_LEN))
      
      return;

   if (VG_(strcmp)(symbol, VG_STRINGIFY(VG_NOTIFY_ON_LOAD(freeres))) == 0)
      VG_(client___libc_freeres_wrapper) = addr;
   else
   if (VG_(strcmp)(symbol, VG_STRINGIFY(VG_NOTIFY_ON_LOAD(ifunc_wrapper))) == 0)
      iFuncWrapper = addr;
   else
      vg_assert2(0, "unrecognised load notification function: %s", symbol);
}



static void handle_require_text_symbols ( DebugInfo* di )
{
   HChar* fnpatts[VG_CLO_MAX_REQ_TSYMS];
   Int    fnpatts_used = 0;
   Int    i, j;
   const HChar* di_soname = VG_(DebugInfo_get_soname)(di);
   vg_assert(di_soname); 

   VG_(memset)(&fnpatts, 0, sizeof(fnpatts));

   vg_assert(VG_(clo_n_req_tsyms) >= 0);
   vg_assert(VG_(clo_n_req_tsyms) <= VG_CLO_MAX_REQ_TSYMS);
   for (i = 0; i < VG_(clo_n_req_tsyms); i++) {
      HChar* spec = VG_(clo_req_tsyms)[i];
      vg_assert(spec && VG_(strlen)(spec) >= 4);
      
      spec = VG_(strdup)("m_redir.hrts.1", spec);
      HChar sep = spec[0];
      HChar* sopatt = &spec[1];
      HChar* fnpatt = VG_(strchr)(sopatt, sep);
      
      
      vg_assert(fnpatt && *fnpatt == sep);
      *fnpatt = 0;
      fnpatt++;
      if (VG_(string_match)(sopatt, di_soname))
         fnpatts[fnpatts_used++]
            = VG_(strdup)("m_redir.hrts.2", fnpatt);
      VG_(free)(spec);
   }

   if (fnpatts_used == 0)
      return;  


   if (0) VG_(printf)("for %s\n", di_soname);
   for (i = 0; i < fnpatts_used; i++)
      if (0) VG_(printf)("   fnpatt: %s\n", fnpatts[i]);

   for (i = 0; i < fnpatts_used; i++) {
      Bool   found  = False;
      HChar* fnpatt = fnpatts[i];
      Int    nsyms  = VG_(DebugInfo_syms_howmany)(di);
      for (j = 0; j < nsyms; j++) {
         Bool    isText        = False;
         UChar*  sym_name_pri  = NULL;
         UChar** sym_names_sec = NULL;
         VG_(DebugInfo_syms_getidx)( di, j, NULL, NULL,
                                     NULL, &sym_name_pri, &sym_names_sec,
                                     &isText, NULL );
         UChar*  twoslots[2];
         UChar** names_init = alloc_symname_array(sym_name_pri, sym_names_sec,
                                                  &twoslots[0]);
         UChar** names;
         for (names = names_init; *names; names++) {
            
            if (0) VG_(printf)("QQQ %s\n", *names);
            vg_assert(sym_name_pri);
            if (!isText)
               continue;
            if (VG_(string_match)(fnpatt, *names)) {
               found = True;
               break;
            }
         }
         free_symname_array(names_init, &twoslots[0]);
         if (found)
            break;
      }

      if (!found) {
         HChar* v = "valgrind:  ";
         VG_(printf)("\n");
         VG_(printf)(
         "%sFatal error at when loading library with soname\n", v);
         VG_(printf)(
         "%s   %s\n", v, di_soname);
         VG_(printf)(
         "%sCannot find any text symbol with a name "
         "that matches the pattern\n", v);
         VG_(printf)("%s   %s\n", v, fnpatt);
         VG_(printf)("%sas required by a --require-text-symbol= "
         "specification.\n", v);
         VG_(printf)("\n");
         VG_(printf)(
         "%sCannot continue -- exiting now.\n", v);
         VG_(printf)("\n");
         VG_(exit)(1);
      }
   }

   
   for (i = 0; i < fnpatts_used; i++)
      VG_(free)(fnpatts[i]);
}



static void show_spec ( HChar* left, Spec* spec )
{
   VG_(message)( Vg_DebugMsg, 
                 "%s%25s %30s %s-> (%04d.%d) 0x%08llx\n",
                 left,
                 spec->from_sopatt, spec->from_fnpatt,
                 spec->isWrap ? "W" : "R",
                 spec->becTag, spec->becPrio,
                 (ULong)spec->to_addr );
}

static void show_active ( HChar* left, Active* act )
{
   Bool ok;
   HChar name1[64] = "";
   HChar name2[64] = "";
   name1[0] = name2[0] = 0;
   ok = VG_(get_fnname_w_offset)(act->from_addr, name1, 64);
   if (!ok) VG_(strcpy)(name1, "???");
   ok = VG_(get_fnname_w_offset)(act->to_addr, name2, 64);
   if (!ok) VG_(strcpy)(name2, "???");

   VG_(message)(Vg_DebugMsg, "%s0x%08llx (%20s) %s-> (%04d.%d) 0x%08llx %s\n", 
                             left, 
                             (ULong)act->from_addr, name1,
                             act->isWrap ? "W" : "R",
                             act->becTag, act->becPrio,
                             (ULong)act->to_addr, name2 );
}

static void show_redir_state ( HChar* who )
{
   TopSpec* ts;
   Spec*    sp;
   Active*  act;
   VG_(message)(Vg_DebugMsg, "<<\n");
   VG_(message)(Vg_DebugMsg, "   ------ REDIR STATE %s ------\n", who);
   for (ts = topSpecs; ts; ts = ts->next) {
      if (ts->seginfo)
         VG_(message)(Vg_DebugMsg, 
                      "   TOPSPECS of soname %s filename %s\n",
                      (HChar*)VG_(DebugInfo_get_soname)(ts->seginfo),
                      (HChar*)VG_(DebugInfo_get_filename)(ts->seginfo));
      else
         VG_(message)(Vg_DebugMsg, 
                      "   TOPSPECS of soname (hardwired)\n");
         
      for (sp = ts->specs; sp; sp = sp->next)
         show_spec("     ", sp);
   }
   VG_(message)(Vg_DebugMsg, "   ------ ACTIVE ------\n");
   VG_(OSetGen_ResetIter)( activeSet );
   while ( (act = VG_(OSetGen_Next)(activeSet)) ) {
      show_active("    ", act);
   }

   VG_(message)(Vg_DebugMsg, ">>\n");
}

