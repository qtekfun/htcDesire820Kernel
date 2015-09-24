

/*
   This file is part of MemCheck, a heavyweight Valgrind tool for
   detecting memory errors.

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

#include "pub_tool_basics.h"
#include "pub_tool_gdbserver.h"
#include "pub_tool_poolalloc.h"     
#include "pub_tool_hashtable.h"     
#include "pub_tool_libcbase.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_machine.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_options.h"
#include "pub_tool_replacemalloc.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_threadstate.h"
#include "pub_tool_debuginfo.h"     
#include "pub_tool_xarray.h"

#include "mc_include.h"



Bool MC_(any_value_errors) = False;


typedef enum {
   Block_Mallocd = 111,
   Block_Freed,
   Block_Mempool,
   Block_MempoolChunk,
   Block_UserG
} BlockKind;


typedef 
   enum { 
      Addr_Undescribed, 
      Addr_Unknown,     
      Addr_Block,       
      Addr_Stack,       
      Addr_DataSym,     
      Addr_Variable,    
      Addr_SectKind     
   }
   AddrTag;

typedef
   struct _AddrInfo
   AddrInfo;

struct _AddrInfo {
   AddrTag tag;
   union {
      
      struct { } Undescribed;

      
      struct {
         ThreadId tid;        
      } Stack;

      
      
      struct {
         BlockKind   block_kind;
         Char*       block_desc;    
         SizeT       block_szB;
         PtrdiffT    rwoffset;
         ExeContext* lastchange;
      } Block;

      
      
      struct {
         Char     name[128];
         PtrdiffT offset;
      } DataSym;

      
      struct {
         XArray*  descr1;
         XArray*  descr2;
      } Variable;

      
      
      struct {
         Char       objname[128];
         VgSectKind kind;
      } SectKind;

      
      struct { } Unknown;

   } Addr;
};


typedef 
   enum { 
      Err_Value,
      Err_Cond,
      Err_CoreMem,
      Err_Addr, 
      Err_Jump, 
      Err_RegParam,
      Err_MemParam,
      Err_User,
      Err_Free,
      Err_FreeMismatch,
      Err_Overlap,
      Err_Leak,
      Err_IllegalMempool,
   }
   MC_ErrorTag;


typedef struct _MC_Error MC_Error;

struct _MC_Error {
   
   

   union {
      
      
      
      struct {
         SizeT szB;   
         
         UInt        otag;      
         ExeContext* origin_ec; 
      } Value;

      
      struct {
         
         UInt        otag;      
         ExeContext* origin_ec; 
      } Cond;

      
      
      
      struct {
      } CoreMem;

      
      struct {
         Bool     isWrite;    
         SizeT    szB;        
         Bool     maybe_gcc;  
         AddrInfo ai;
      } Addr;

      
      struct {
         AddrInfo ai;
      } Jump;

      
      struct {
         
         UInt        otag;      
         ExeContext* origin_ec; 
      } RegParam;

      
      struct {
         Bool     isAddrErr;  
         AddrInfo ai;
         
         UInt        otag;      
         ExeContext* origin_ec; 
      } MemParam;

      
      struct {
         Bool     isAddrErr;  
         AddrInfo ai;
         
         UInt        otag;      
         ExeContext* origin_ec; 
      } User;

      
      
      struct {
         AddrInfo ai;
      } Free;

      
      
      struct {
         AddrInfo ai;
      } FreeMismatch;

      
      struct {
         Addr src;   
         Addr dst;   
         Int  szB;   
      } Overlap;

      
      struct {
         UInt        n_this_record;
         UInt        n_total_records;
         LossRecord* lr;
      } Leak;

      
      struct {
         AddrInfo ai;
      } IllegalMempool;

   } Err;
};



void MC_(before_pp_Error) ( Error* err ) {
}

static void emit_WRK ( HChar* format, va_list vargs )
{
   if (VG_(clo_xml)) {
      VG_(vprintf_xml)(format, vargs);
   } else {
      VG_(vmessage)(Vg_UserMsg, format, vargs);
   }
}
static void emit ( HChar* format, ... ) PRINTF_CHECK(1, 2);
static void emit ( HChar* format, ... )
{
   va_list vargs;
   va_start(vargs, format);
   emit_WRK(format, vargs);
   va_end(vargs);
}
static void emiN ( HChar* format, ... ) 
{
   va_list vargs;
   va_start(vargs, format);
   emit_WRK(format, vargs);
   va_end(vargs);
}


static void mc_pp_AddrInfo ( Addr a, AddrInfo* ai, Bool maybe_gcc )
{
   HChar* xpre  = VG_(clo_xml) ? "  <auxwhat>" : " ";
   HChar* xpost = VG_(clo_xml) ? "</auxwhat>"  : "";

   switch (ai->tag) {
      case Addr_Unknown:
         if (maybe_gcc) {
            emit( "%sAddress 0x%llx is just below the stack ptr.  "
                  "To suppress, use: --workaround-gcc296-bugs=yes%s\n",
                  xpre, (ULong)a, xpost );
	 } else {
            emit( "%sAddress 0x%llx "
                  "is not stack'd, malloc'd or (recently) free'd%s\n",
                  xpre, (ULong)a, xpost );
         }
         break;

      case Addr_Stack: 
         emit( "%sAddress 0x%llx is on thread %d's stack%s\n", 
               xpre, (ULong)a, ai->Addr.Stack.tid, xpost );
         break;

      case Addr_Block: {
         SizeT    block_szB = ai->Addr.Block.block_szB;
         PtrdiffT rwoffset  = ai->Addr.Block.rwoffset;
         SizeT    delta;
         const    Char* relative;

         if (rwoffset < 0) {
            delta    = (SizeT)(-rwoffset);
            relative = "before";
         } else if (rwoffset >= block_szB) {
            delta    = rwoffset - block_szB;
            relative = "after";
         } else {
            delta    = rwoffset;
            relative = "inside";
         }
         emit(
            "%sAddress 0x%lx is %'lu bytes %s a %s of size %'lu %s%s\n",
            xpre,
            a, delta, relative, ai->Addr.Block.block_desc,
            block_szB,
            ai->Addr.Block.block_kind==Block_Mallocd ? "alloc'd" 
            : ai->Addr.Block.block_kind==Block_Freed ? "free'd" 
                                                     : "client-defined",
            xpost
         );
         VG_(pp_ExeContext)(ai->Addr.Block.lastchange);
         break;
      }

      case Addr_DataSym:
         emiN( "%sAddress 0x%llx is %llu bytes "
               "inside data symbol \"%pS\"%s\n",
               xpre,
               (ULong)a,
               (ULong)ai->Addr.DataSym.offset,
               ai->Addr.DataSym.name,
               xpost );
         break;

      case Addr_Variable:
         if (ai->Addr.Variable.descr1)
            emit( "%s%s\n",
                  VG_(clo_xml) ? "  " : " ",
                  (HChar*)VG_(indexXA)(ai->Addr.Variable.descr1, 0) );
         if (ai->Addr.Variable.descr2)
            emit( "%s%s\n",
                  VG_(clo_xml) ? "  " : " ",
                  (HChar*)VG_(indexXA)(ai->Addr.Variable.descr2, 0) );
         break;

      case Addr_SectKind:
         emiN( "%sAddress 0x%llx is in the %pS segment of %pS%s\n",
               xpre,
               (ULong)a,
               VG_(pp_SectKind)(ai->Addr.SectKind.kind),
               ai->Addr.SectKind.objname,
               xpost );
         break;

      default:
         VG_(tool_panic)("mc_pp_AddrInfo");
   }
}

static const HChar* str_leak_lossmode ( Reachedness lossmode )
{
   const HChar *loss = "?";
   switch (lossmode) {
      case Unreached:    loss = "definitely lost"; break;
      case IndirectLeak: loss = "indirectly lost"; break;
      case Possible:     loss = "possibly lost"; break;
      case Reachable:    loss = "still reachable"; break;
   }
   return loss;
}

static const HChar* xml_leak_kind ( Reachedness lossmode )
{
   const HChar *loss = "?";
   switch (lossmode) {
      case Unreached:    loss = "Leak_DefinitelyLost"; break;
      case IndirectLeak: loss = "Leak_IndirectlyLost"; break;
      case Possible:     loss = "Leak_PossiblyLost"; break;
      case Reachable:    loss = "Leak_StillReachable"; break;
   }
   return loss;
}

static void mc_pp_origin ( ExeContext* ec, UInt okind )
{
   HChar* src = NULL;
   tl_assert(ec);

   switch (okind) {
      case MC_OKIND_STACK:   src = " by a stack allocation"; break;
      case MC_OKIND_HEAP:    src = " by a heap allocation"; break;
      case MC_OKIND_USER:    src = " by a client request"; break;
      case MC_OKIND_UNKNOWN: src = ""; break;
   }
   tl_assert(src); 

   if (VG_(clo_xml)) {
      emit( "  <auxwhat>Uninitialised value was created%s</auxwhat>\n",
            src);
      VG_(pp_ExeContext)( ec );
   } else {
      emit( " Uninitialised value was created%s\n", src);
      VG_(pp_ExeContext)( ec );
   }
}

char * MC_(snprintf_delta) (char * buf, Int size, 
                            SizeT current_val, SizeT old_val, 
                            LeakCheckDeltaMode delta_mode)
{
   if (delta_mode == LCD_Any)
      buf[0] = '\0';
   else if (current_val >= old_val)
      VG_(snprintf) (buf, size, " (+%'lu)", current_val - old_val);
   else
      VG_(snprintf) (buf, size, " (-%'lu)", old_val - current_val);

   return buf;
}

static void pp_LossRecord(UInt n_this_record, UInt n_total_records,
                          LossRecord* lr, Bool xml)
{
   
   
   char        d_bytes[20];
   char        d_direct_bytes[20];
   char        d_indirect_bytes[20];
   char        d_num_blocks[20];

   MC_(snprintf_delta) (d_bytes, 20, 
                        lr->szB + lr->indirect_szB, 
                        lr->old_szB + lr->old_indirect_szB,
                        MC_(detect_memory_leaks_last_delta_mode));
   MC_(snprintf_delta) (d_direct_bytes, 20,
                        lr->szB,
                        lr->old_szB,
                        MC_(detect_memory_leaks_last_delta_mode));
   MC_(snprintf_delta) (d_indirect_bytes, 20,
                        lr->indirect_szB,
                        lr->old_indirect_szB,
                        MC_(detect_memory_leaks_last_delta_mode));
   MC_(snprintf_delta) (d_num_blocks, 20,
                        (SizeT) lr->num_blocks,
                        (SizeT) lr->old_num_blocks,
                        MC_(detect_memory_leaks_last_delta_mode));

   if (xml) {
      emit("  <kind>%s</kind>\n", xml_leak_kind(lr->key.state));
      if (lr->indirect_szB > 0) {
         emit( "  <xwhat>\n" );
         emit( "    <text>%'lu%s (%'lu%s direct, %'lu%s indirect) bytes "
               "in %'u%s blocks"
               " are %s in loss record %'u of %'u</text>\n",
               lr->szB + lr->indirect_szB, d_bytes,
               lr->szB, d_direct_bytes,
               lr->indirect_szB, d_indirect_bytes,
               lr->num_blocks, d_num_blocks,
               str_leak_lossmode(lr->key.state),
               n_this_record, n_total_records );
         
         emit( "    <leakedbytes>%lu</leakedbytes>\n",
               lr->szB + lr->indirect_szB );
         emit( "    <leakedblocks>%u</leakedblocks>\n", lr->num_blocks );
         emit( "  </xwhat>\n" );
      } else {
         emit( "  <xwhat>\n" );
         emit( "    <text>%'lu%s bytes in %'u%s blocks"
               " are %s in loss record %'u of %'u</text>\n",
               lr->szB, d_direct_bytes,
               lr->num_blocks, d_num_blocks,
               str_leak_lossmode(lr->key.state), 
               n_this_record, n_total_records );
         emit( "    <leakedbytes>%ld</leakedbytes>\n", lr->szB);
         emit( "    <leakedblocks>%d</leakedblocks>\n", lr->num_blocks);
         emit( "  </xwhat>\n" );
      }
      VG_(pp_ExeContext)(lr->key.allocated_at);
   } else { 
      if (lr->indirect_szB > 0) {
         emit(
            "%'lu%s (%'lu%s direct, %'lu%s indirect) bytes in %'u%s blocks"
            " are %s in loss record %'u of %'u\n",
            lr->szB + lr->indirect_szB, d_bytes,
            lr->szB, d_direct_bytes,
            lr->indirect_szB, d_indirect_bytes,
            lr->num_blocks, d_num_blocks,
            str_leak_lossmode(lr->key.state),
            n_this_record, n_total_records
         );
      } else {
         emit(
            "%'lu%s bytes in %'u%s blocks are %s in loss record %'u of %'u\n",
            lr->szB, d_direct_bytes,
            lr->num_blocks, d_num_blocks,
            str_leak_lossmode(lr->key.state),
            n_this_record, n_total_records
         );
      }
      VG_(pp_ExeContext)(lr->key.allocated_at);
   } 
}

void MC_(pp_LossRecord)(UInt n_this_record, UInt n_total_records,
                        LossRecord* l)
{
   pp_LossRecord (n_this_record, n_total_records, l,  False);
}

void MC_(pp_Error) ( Error* err )
{
   const Bool xml  = VG_(clo_xml); 
   MC_Error* extra = VG_(get_error_extra)(err);

   switch (VG_(get_error_kind)(err)) {
      case Err_CoreMem:
         
         
         
         if (xml) {
            emit( "  <kind>CoreMemError</kind>\n" );
            emiN( "  <what>%pS contains unaddressable byte(s)</what>\n",
                  VG_(get_error_string)(err));
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
         } else {
            emit( "%s contains unaddressable byte(s)\n",
                  VG_(get_error_string)(err));
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
         }
         break;
      
      case Err_Value:
         MC_(any_value_errors) = True;
         if (xml) {
            emit( "  <kind>UninitValue</kind>\n" );
            emit( "  <what>Use of uninitialised value of size %ld</what>\n",
                  extra->Err.Value.szB );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.Value.origin_ec)
               mc_pp_origin( extra->Err.Value.origin_ec,
                            extra->Err.Value.otag & 3 );
         } else {
            emit( "Use of uninitialised value of size %ld\n",
                  extra->Err.Value.szB );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.Value.origin_ec)
               mc_pp_origin( extra->Err.Value.origin_ec,
                            extra->Err.Value.otag & 3 );
         }
         break;

      case Err_Cond:
         MC_(any_value_errors) = True;
         if (xml) {
            emit( "  <kind>UninitCondition</kind>\n" );
            emit( "  <what>Conditional jump or move depends"
                  " on uninitialised value(s)</what>\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.Cond.origin_ec)
               mc_pp_origin( extra->Err.Cond.origin_ec,
                             extra->Err.Cond.otag & 3 );
         } else {
            emit( "Conditional jump or move depends"
                  " on uninitialised value(s)\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.Cond.origin_ec)
               mc_pp_origin( extra->Err.Cond.origin_ec,
                             extra->Err.Cond.otag & 3 );
         }
         break;

      case Err_RegParam:
         MC_(any_value_errors) = True;
         if (xml) {
            emit( "  <kind>SyscallParam</kind>\n" );
            emiN( "  <what>Syscall param %pS contains "
                  "uninitialised byte(s)</what>\n",
                  VG_(get_error_string)(err) );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.RegParam.origin_ec)
               mc_pp_origin( extra->Err.RegParam.origin_ec,
                             extra->Err.RegParam.otag & 3 );
         } else {
            emit( "Syscall param %s contains uninitialised byte(s)\n",
                  VG_(get_error_string)(err) );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            if (extra->Err.RegParam.origin_ec)
               mc_pp_origin( extra->Err.RegParam.origin_ec,
                             extra->Err.RegParam.otag & 3 );
         }
         break;

      case Err_MemParam:
         if (!extra->Err.MemParam.isAddrErr)
            MC_(any_value_errors) = True;
         if (xml) {
            emit( "  <kind>SyscallParam</kind>\n" );
            emiN( "  <what>Syscall param %pS points to %s byte(s)</what>\n",
                  VG_(get_error_string)(err),
                  extra->Err.MemParam.isAddrErr 
                     ? "unaddressable" : "uninitialised" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err),
                           &extra->Err.MemParam.ai, False);
            if (extra->Err.MemParam.origin_ec 
                && !extra->Err.MemParam.isAddrErr)
               mc_pp_origin( extra->Err.MemParam.origin_ec,
                             extra->Err.MemParam.otag & 3 );
         } else {
            emit( "Syscall param %s points to %s byte(s)\n",
                  VG_(get_error_string)(err),
                  extra->Err.MemParam.isAddrErr 
                     ? "unaddressable" : "uninitialised" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err),
                           &extra->Err.MemParam.ai, False);
            if (extra->Err.MemParam.origin_ec 
                && !extra->Err.MemParam.isAddrErr)
               mc_pp_origin( extra->Err.MemParam.origin_ec,
                             extra->Err.MemParam.otag & 3 );
         }
         break;

      case Err_User:
         if (!extra->Err.User.isAddrErr)
            MC_(any_value_errors) = True;
         if (xml) { 
            emit( "  <kind>ClientCheck</kind>\n" );
            emit( "  <what>%s byte(s) found "
                  "during client check request</what>\n", 
                   extra->Err.User.isAddrErr
                      ? "Unaddressable" : "Uninitialised" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err), &extra->Err.User.ai,
                           False);
            if (extra->Err.User.origin_ec && !extra->Err.User.isAddrErr)
               mc_pp_origin( extra->Err.User.origin_ec,
                             extra->Err.User.otag & 3 );
         } else {
            emit( "%s byte(s) found during client check request\n", 
                   extra->Err.User.isAddrErr
                      ? "Unaddressable" : "Uninitialised" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err), &extra->Err.User.ai,
                           False);
            if (extra->Err.User.origin_ec && !extra->Err.User.isAddrErr)
               mc_pp_origin( extra->Err.User.origin_ec,
                             extra->Err.User.otag & 3 );
         }
         break;

      case Err_Free:
         if (xml) {
            emit( "  <kind>InvalidFree</kind>\n" );
            emit( "  <what>Invalid free() / delete / delete[]"
                  " / realloc()</what>\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.Free.ai, False );
         } else {
            emit( "Invalid free() / delete / delete[] / realloc()\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.Free.ai, False );
         }
         break;

      case Err_FreeMismatch:
         if (xml) {
            emit( "  <kind>MismatchedFree</kind>\n" );
            emit( "  <what>Mismatched free() / delete / delete []</what>\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err),
                           &extra->Err.FreeMismatch.ai, False);
         } else {
            emit( "Mismatched free() / delete / delete []\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo(VG_(get_error_address)(err),
                           &extra->Err.FreeMismatch.ai, False);
         }
         break;

      case Err_Addr:
         if (xml) {
            emit( "  <kind>Invalid%s</kind>\n",
                  extra->Err.Addr.isWrite ? "Write" : "Read"  );
            emit( "  <what>Invalid %s of size %ld</what>\n",
                  extra->Err.Addr.isWrite ? "write" : "read",
                  extra->Err.Addr.szB );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.Addr.ai,
                            extra->Err.Addr.maybe_gcc );
         } else {
            emit( "Invalid %s of size %ld\n",
                  extra->Err.Addr.isWrite ? "write" : "read",
                  extra->Err.Addr.szB );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );

            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.Addr.ai,
                            extra->Err.Addr.maybe_gcc );
         }
         break;

      case Err_Jump:
         if (xml) {
            emit( "  <kind>InvalidJump</kind>\n" );
            emit( "  <what>Jump to the invalid address stated "
                  "on the next line</what>\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err), &extra->Err.Jump.ai,
                            False );
         } else {
            emit( "Jump to the invalid address stated on the next line\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err), &extra->Err.Jump.ai,
                            False );
         }
         break;

      case Err_Overlap:
         if (xml) {
            emit( "  <kind>Overlap</kind>\n" );
            if (extra->Err.Overlap.szB == 0) {
               emiN( "  <what>Source and destination overlap "
                     "in %pS(%#lx, %#lx)\n</what>\n",
                     VG_(get_error_string)(err),
                     extra->Err.Overlap.dst, extra->Err.Overlap.src );
            } else {
               emit( "  <what>Source and destination overlap "
                     "in %s(%#lx, %#lx, %d)</what>\n",
                     VG_(get_error_string)(err),
                     extra->Err.Overlap.dst, extra->Err.Overlap.src,
                     extra->Err.Overlap.szB );
            }
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
         } else {
            if (extra->Err.Overlap.szB == 0) {
               emiN( "Source and destination overlap in %pS(%#lx, %#lx)\n",
                     VG_(get_error_string)(err),
                     extra->Err.Overlap.dst, extra->Err.Overlap.src );
            } else {
               emit( "Source and destination overlap in %s(%#lx, %#lx, %d)\n",
                     VG_(get_error_string)(err),
                     extra->Err.Overlap.dst, extra->Err.Overlap.src,
                     extra->Err.Overlap.szB );
            }
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
         }
         break;

      case Err_IllegalMempool:
         
         
         if (xml) {
            emit( "  <kind>InvalidMemPool</kind>\n" );
            emit( "  <what>Illegal memory pool address</what>\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.IllegalMempool.ai, False );
         } else {
            emit( "Illegal memory pool address\n" );
            VG_(pp_ExeContext)( VG_(get_error_where)(err) );
            mc_pp_AddrInfo( VG_(get_error_address)(err),
                            &extra->Err.IllegalMempool.ai, False );
         }
         break;

      case Err_Leak: {
         UInt        n_this_record   = extra->Err.Leak.n_this_record;
         UInt        n_total_records = extra->Err.Leak.n_total_records;
         LossRecord* lr              = extra->Err.Leak.lr;
         pp_LossRecord (n_this_record, n_total_records, lr, xml);
         break;
      }

      default: 
         VG_(printf)("Error:\n  unknown Memcheck error code %d\n",
                     VG_(get_error_kind)(err));
         VG_(tool_panic)("unknown error code in mc_pp_Error)");
   }
}


#define VG_GCC296_BUG_STACK_SLOP 1024

static Bool is_just_below_ESP( Addr esp, Addr aa )
{
   esp -= VG_STACK_REDZONE_SZB;
   if (esp > aa && (esp - aa) <= VG_GCC296_BUG_STACK_SLOP)
      return True;
   else
      return False;
}


void MC_(record_address_error) ( ThreadId tid, Addr a, Int szB,
                                 Bool isWrite )
{
   MC_Error extra;
   Bool     just_below_esp;

   if (MC_(in_ignored_range)(a)) 
      return;

   if (VG_(is_watched)( (isWrite ? write_watchpoint : read_watchpoint), a, szB))
      return;

   just_below_esp = is_just_below_ESP( VG_(get_SP)(tid), a );

   if (MC_(clo_workaround_gcc296_bugs) && just_below_esp)
      return;

   extra.Err.Addr.isWrite   = isWrite;
   extra.Err.Addr.szB       = szB;
   extra.Err.Addr.maybe_gcc = just_below_esp;
   extra.Err.Addr.ai.tag    = Addr_Undescribed;
   VG_(maybe_record_error)( tid, Err_Addr, a, NULL, &extra );
}

void MC_(record_value_error) ( ThreadId tid, Int szB, UInt otag )
{
   MC_Error extra;
   tl_assert( MC_(clo_mc_level) >= 2 );
   if (otag > 0)
      tl_assert( MC_(clo_mc_level) == 3 );
   extra.Err.Value.szB       = szB;
   extra.Err.Value.otag      = otag;
   extra.Err.Value.origin_ec = NULL;  
   VG_(maybe_record_error)( tid, Err_Value, 0, NULL, &extra );
}

void MC_(record_cond_error) ( ThreadId tid, UInt otag )
{
   MC_Error extra;
   tl_assert( MC_(clo_mc_level) >= 2 );
   if (otag > 0)
      tl_assert( MC_(clo_mc_level) == 3 );
   extra.Err.Cond.otag      = otag;
   extra.Err.Cond.origin_ec = NULL;  
   VG_(maybe_record_error)( tid, Err_Cond, 0, NULL, &extra );
}


void MC_(record_core_mem_error) ( ThreadId tid, Char* msg )
{
   VG_(maybe_record_error)( tid, Err_CoreMem, 0, msg, NULL );
}

void MC_(record_regparam_error) ( ThreadId tid, Char* msg, UInt otag )
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   if (otag > 0)
      tl_assert( MC_(clo_mc_level) == 3 );
   extra.Err.RegParam.otag      = otag;
   extra.Err.RegParam.origin_ec = NULL;  
   VG_(maybe_record_error)( tid, Err_RegParam, 0, msg, &extra );
}

void MC_(record_memparam_error) ( ThreadId tid, Addr a, 
                                  Bool isAddrErr, Char* msg, UInt otag )
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   if (!isAddrErr) 
      tl_assert( MC_(clo_mc_level) >= 2 );
   if (otag != 0) {
      tl_assert( MC_(clo_mc_level) == 3 );
      tl_assert( !isAddrErr );
   }
   extra.Err.MemParam.isAddrErr = isAddrErr;
   extra.Err.MemParam.ai.tag    = Addr_Undescribed;
   extra.Err.MemParam.otag      = otag;
   extra.Err.MemParam.origin_ec = NULL;  
   VG_(maybe_record_error)( tid, Err_MemParam, a, msg, &extra );
}

void MC_(record_jump_error) ( ThreadId tid, Addr a )
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   extra.Err.Jump.ai.tag = Addr_Undescribed;
   VG_(maybe_record_error)( tid, Err_Jump, a, NULL, &extra );
}

void MC_(record_free_error) ( ThreadId tid, Addr a ) 
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   extra.Err.Free.ai.tag = Addr_Undescribed;
   VG_(maybe_record_error)( tid, Err_Free, a, NULL, &extra );
}

void MC_(record_freemismatch_error) ( ThreadId tid, MC_Chunk* mc )
{
   MC_Error extra;
   AddrInfo* ai = &extra.Err.FreeMismatch.ai;
   tl_assert(VG_INVALID_THREADID != tid);
   ai->tag = Addr_Block;
   ai->Addr.Block.block_kind = Block_Mallocd;  
   ai->Addr.Block.block_desc = "block";
   ai->Addr.Block.block_szB  = mc->szB;
   ai->Addr.Block.rwoffset   = 0;
   ai->Addr.Block.lastchange = mc->where;
   VG_(maybe_record_error)( tid, Err_FreeMismatch, mc->data, NULL,
                            &extra );
}

void MC_(record_illegal_mempool_error) ( ThreadId tid, Addr a ) 
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   extra.Err.IllegalMempool.ai.tag = Addr_Undescribed;
   VG_(maybe_record_error)( tid, Err_IllegalMempool, a, NULL, &extra );
}

void MC_(record_overlap_error) ( ThreadId tid, Char* function,
                                 Addr src, Addr dst, SizeT szB )
{
   MC_Error extra;
   tl_assert(VG_INVALID_THREADID != tid);
   extra.Err.Overlap.src = src;
   extra.Err.Overlap.dst = dst;
   extra.Err.Overlap.szB = szB;
   VG_(maybe_record_error)( 
      tid, Err_Overlap, 0, function, &extra );
}

Bool MC_(record_leak_error) ( ThreadId tid, UInt n_this_record,
                              UInt n_total_records, LossRecord* lr,
                              Bool print_record, Bool count_error )
{
   MC_Error extra;
   extra.Err.Leak.n_this_record   = n_this_record;
   extra.Err.Leak.n_total_records = n_total_records;
   extra.Err.Leak.lr              = lr;
   return
   VG_(unique_error) ( tid, Err_Leak, 0, NULL, &extra,
                       lr->key.allocated_at, print_record,
                       False, count_error );
}

void MC_(record_user_error) ( ThreadId tid, Addr a,
                              Bool isAddrErr, UInt otag )
{
   MC_Error extra;
   if (otag != 0) {
      tl_assert(!isAddrErr);
      tl_assert( MC_(clo_mc_level) == 3 );
   }
   if (!isAddrErr) {
      tl_assert( MC_(clo_mc_level) >= 2 );
   }
   tl_assert(VG_INVALID_THREADID != tid);
   extra.Err.User.isAddrErr = isAddrErr;
   extra.Err.User.ai.tag    = Addr_Undescribed;
   extra.Err.User.otag      = otag;
   extra.Err.User.origin_ec = NULL;  
   VG_(maybe_record_error)( tid, Err_User, a, NULL, &extra );
}


Bool MC_(eq_Error) ( VgRes res, Error* e1, Error* e2 )
{
   MC_Error* extra1 = VG_(get_error_extra)(e1);
   MC_Error* extra2 = VG_(get_error_extra)(e2);

   
   tl_assert(VG_(get_error_kind)(e1) == VG_(get_error_kind)(e2));
   
   switch (VG_(get_error_kind)(e1)) {
      case Err_CoreMem: {
         Char *e1s, *e2s;
         e1s = VG_(get_error_string)(e1);
         e2s = VG_(get_error_string)(e2);
         if (e1s == e2s)                   return True;
         if (VG_STREQ(e1s, e2s))           return True;
         return False;
      }

      case Err_RegParam:
         return VG_STREQ(VG_(get_error_string)(e1), VG_(get_error_string)(e2));

      
      
      
      
      
      
      case Err_MemParam:
         if (!VG_STREQ(VG_(get_error_string)(e1),
                       VG_(get_error_string)(e2))) return False;
         
      case Err_User:
         return ( extra1->Err.User.isAddrErr == extra2->Err.User.isAddrErr
                ? True : False );

      case Err_Free:
      case Err_FreeMismatch:
      case Err_Jump:
      case Err_IllegalMempool:
      case Err_Overlap:
      case Err_Cond:
         return True;

      case Err_Addr:
         return ( extra1->Err.Addr.szB == extra2->Err.Addr.szB
                ? True : False );

      case Err_Value:
         return ( extra1->Err.Value.szB == extra2->Err.Value.szB
                ? True : False );

      case Err_Leak:
         VG_(tool_panic)("Shouldn't get Err_Leak in mc_eq_Error,\n"
                         "since it's handled with VG_(unique_error)()!");

      default: 
         VG_(printf)("Error:\n  unknown error code %d\n",
                     VG_(get_error_kind)(e1));
         VG_(tool_panic)("unknown error code in mc_eq_Error");
   }
}

static
Bool addr_is_in_MC_Chunk_default_REDZONE_SZB(MC_Chunk* mc, Addr a)
{
   return VG_(addr_is_in_block)( a, mc->data, mc->szB,
                                 MC_(Malloc_Redzone_SzB) );
}
static
Bool addr_is_in_MC_Chunk_with_REDZONE_SZB(MC_Chunk* mc, Addr a, SizeT rzB)
{
   return VG_(addr_is_in_block)( a, mc->data, mc->szB,
                                 rzB );
}

static Bool client_block_maybe_describe( Addr a, AddrInfo* ai );
static Bool mempool_block_maybe_describe( Addr a, AddrInfo* ai );


static void describe_addr ( Addr a, AddrInfo* ai )
{
   MC_Chunk*  mc;
   ThreadId   tid;
   Addr       stack_min, stack_max;
   VgSectKind sect;

   tl_assert(Addr_Undescribed == ai->tag);

   
   if (client_block_maybe_describe( a, ai )) {
      return;
   }
   
   if (mempool_block_maybe_describe( a, ai )) {
      return;
   }
   
   VG_(HT_ResetIter)(MC_(malloc_list));
   while ( (mc = VG_(HT_Next)(MC_(malloc_list))) ) {
      if (addr_is_in_MC_Chunk_default_REDZONE_SZB(mc, a)) {
         ai->tag = Addr_Block;
         ai->Addr.Block.block_kind = Block_Mallocd;
         if (MC_(get_freed_block_bracketting)( a ))
            ai->Addr.Block.block_desc = "recently re-allocated block";
         else
            ai->Addr.Block.block_desc = "block";
         ai->Addr.Block.block_szB  = mc->szB;
         ai->Addr.Block.rwoffset   = (Word)a - (Word)mc->data;
         ai->Addr.Block.lastchange = mc->where;
         return;
      }
   }
   
   mc = MC_(get_freed_block_bracketting)( a );
   if (mc) {
      ai->tag = Addr_Block;
      ai->Addr.Block.block_kind = Block_Freed;
      ai->Addr.Block.block_desc = "block";
      ai->Addr.Block.block_szB  = mc->szB;
      ai->Addr.Block.rwoffset   = (Word)a - (Word)mc->data;
      ai->Addr.Block.lastchange = mc->where;
      return;
   }
   
   ai->Addr.Variable.descr1
      = VG_(newXA)( VG_(malloc), "mc.da.descr1",
                    VG_(free), sizeof(HChar) );
   ai->Addr.Variable.descr2
      = VG_(newXA)( VG_(malloc), "mc.da.descr2",
                    VG_(free), sizeof(HChar) );

   (void) VG_(get_data_description)( ai->Addr.Variable.descr1,
                                     ai->Addr.Variable.descr2, a );
   if (0 == VG_(strlen)( VG_(indexXA)( ai->Addr.Variable.descr1, 0 ))) {
      VG_(deleteXA)( ai->Addr.Variable.descr1 );
      ai->Addr.Variable.descr1 = NULL;
   }
   if (0 == VG_(strlen)( VG_(indexXA)( ai->Addr.Variable.descr2, 0 ))) {
      VG_(deleteXA)( ai->Addr.Variable.descr2 );
      ai->Addr.Variable.descr2 = NULL;
   }
   if (ai->Addr.Variable.descr1 == NULL)
      tl_assert(ai->Addr.Variable.descr2 == NULL);
   
   if (ai->Addr.Variable.descr1 != NULL) {
      ai->tag = Addr_Variable;
      return;
   }
   VG_(memset)( &ai->Addr.DataSym.name,
                0, sizeof(ai->Addr.DataSym.name));
   if (VG_(get_datasym_and_offset)(
             a, &ai->Addr.DataSym.name[0],
             sizeof(ai->Addr.DataSym.name)-1,
             &ai->Addr.DataSym.offset )) {
      ai->tag = Addr_DataSym;
      tl_assert( ai->Addr.DataSym.name
                    [ sizeof(ai->Addr.DataSym.name)-1 ] == 0);
      return;
   }
   
   VG_(thread_stack_reset_iter)(&tid);
   while ( VG_(thread_stack_next)(&tid, &stack_min, &stack_max) ) {
      if (stack_min - VG_STACK_REDZONE_SZB <= a && a <= stack_max) {
         ai->tag            = Addr_Stack;
         ai->Addr.Stack.tid = tid;
         return;
      }
   }
   
   tl_assert( sizeof(ai->Addr.SectKind.objname) > 4 );
   VG_(memset)( &ai->Addr.SectKind.objname, 
                0, sizeof(ai->Addr.SectKind.objname));
   VG_(strcpy)( ai->Addr.SectKind.objname, "???" );
   sect = VG_(DebugInfo_sect_kind)( &ai->Addr.SectKind.objname[0],
                                    sizeof(ai->Addr.SectKind.objname)-1, a);
   if (sect != Vg_SectUnknown) {
      ai->tag = Addr_SectKind;
      ai->Addr.SectKind.kind = sect;
      tl_assert( ai->Addr.SectKind.objname
                    [ sizeof(ai->Addr.SectKind.objname)-1 ] == 0);
      return;
   }
   
   ai->tag = Addr_Unknown;
   return;
}

void MC_(pp_describe_addr) ( Addr a )
{
   AddrInfo ai;

   ai.tag = Addr_Undescribed;
   describe_addr (a, &ai);
   mc_pp_AddrInfo (a, &ai,  False);
}

static void update_origin ( ExeContext** origin_ec,
                            UInt otag )
{
   UInt ecu = otag & ~3;
   *origin_ec = NULL;
   if (VG_(is_plausible_ECU)(ecu)) {
      *origin_ec = VG_(get_ExeContext_from_ECU)( ecu );
   }
}

UInt MC_(update_Error_extra)( Error* err )
{
   MC_Error* extra = VG_(get_error_extra)(err);

   switch (VG_(get_error_kind)(err)) {
   
   
   case Err_CoreMem:
   
   
   case Err_Overlap:
   
   
   
   case Err_Leak:
      return sizeof(MC_Error);

   
   
   
   case Err_Value:
      update_origin( &extra->Err.Value.origin_ec,
                     extra->Err.Value.otag );
      return sizeof(MC_Error);
   case Err_Cond:
      update_origin( &extra->Err.Cond.origin_ec,
                     extra->Err.Cond.otag );
      return sizeof(MC_Error);
   case Err_RegParam:
      update_origin( &extra->Err.RegParam.origin_ec,
                     extra->Err.RegParam.otag );
      return sizeof(MC_Error);

   
   case Err_Addr:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.Addr.ai );
      return sizeof(MC_Error);
   case Err_MemParam:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.MemParam.ai );
      update_origin( &extra->Err.MemParam.origin_ec,
                     extra->Err.MemParam.otag );
      return sizeof(MC_Error);
   case Err_Jump:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.Jump.ai );
      return sizeof(MC_Error);
   case Err_User:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.User.ai );
      update_origin( &extra->Err.User.origin_ec,
                     extra->Err.User.otag );
      return sizeof(MC_Error);
   case Err_Free:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.Free.ai );
      return sizeof(MC_Error);
   case Err_IllegalMempool:
      describe_addr ( VG_(get_error_address)(err),
                      &extra->Err.IllegalMempool.ai );
      return sizeof(MC_Error);

   
   
   
   
   case Err_FreeMismatch: {
      tl_assert(extra && Block_Mallocd ==
                extra->Err.FreeMismatch.ai.Addr.Block.block_kind);
      (void)client_block_maybe_describe( VG_(get_error_address)(err), 
                                        &extra->Err.FreeMismatch.ai );
      return sizeof(MC_Error);
   }

   default: VG_(tool_panic)("mc_update_extra: bad errkind");
   }
}


static Bool client_block_maybe_describe( Addr a,
                                         AddrInfo* ai )
{
   UWord      i;
   CGenBlock* cgbs = NULL;
   UWord      cgb_used = 0;

   MC_(get_ClientBlock_array)( &cgbs, &cgb_used );
   if (cgbs == NULL)
      tl_assert(cgb_used == 0);

   
   for (i = 0; i < cgb_used; i++) {
      if (cgbs[i].start == 0 && cgbs[i].size == 0) 
         continue;
      
      if (VG_(addr_is_in_block)(a, cgbs[i].start, cgbs[i].size, 0)) {
         ai->tag = Addr_Block;
         ai->Addr.Block.block_kind = Block_UserG;
         ai->Addr.Block.block_desc = cgbs[i].desc;
         ai->Addr.Block.block_szB  = cgbs[i].size;
         ai->Addr.Block.rwoffset   = (Word)(a) - (Word)(cgbs[i].start);
         ai->Addr.Block.lastchange = cgbs[i].where;
         return True;
      }
   }
   return False;
}


static Bool mempool_block_maybe_describe( Addr a,
                                          AddrInfo* ai )
{
   MC_Mempool* mp;
   tl_assert( MC_(mempool_list) );

   VG_(HT_ResetIter)( MC_(mempool_list) );
   while ( (mp = VG_(HT_Next)(MC_(mempool_list))) ) {
      if (mp->chunks != NULL) {
         MC_Chunk* mc;
         VG_(HT_ResetIter)(mp->chunks);
         while ( (mc = VG_(HT_Next)(mp->chunks)) ) {
            if (addr_is_in_MC_Chunk_with_REDZONE_SZB(mc, a, mp->rzB)) {
               ai->tag = Addr_Block;
               ai->Addr.Block.block_kind = Block_MempoolChunk;
               ai->Addr.Block.block_desc = "block";
               ai->Addr.Block.block_szB  = mc->szB;
               ai->Addr.Block.rwoffset   = (Word)a - (Word)mc->data;
               ai->Addr.Block.lastchange = mc->where;
               return True;
            }
         }
      }
   }
   return False;
}



typedef 
   enum { 
      ParamSupp,     
      UserSupp,      
      CoreMemSupp,   

      
      Value1Supp, Value2Supp, Value4Supp, Value8Supp, Value16Supp,

      
      CondSupp,

      
      Addr1Supp, Addr2Supp, Addr4Supp, Addr8Supp, Addr16Supp,

      JumpSupp,      
      FreeSupp,      
      OverlapSupp,   
      LeakSupp,      
      MempoolSupp,   
   } 
   MC_SuppKind;

Bool MC_(is_recognised_suppression) ( Char* name, Supp* su )
{
   SuppKind skind;

   if      (VG_STREQ(name, "Param"))   skind = ParamSupp;
   else if (VG_STREQ(name, "User"))    skind = UserSupp;
   else if (VG_STREQ(name, "CoreMem")) skind = CoreMemSupp;
   else if (VG_STREQ(name, "Addr1"))   skind = Addr1Supp;
   else if (VG_STREQ(name, "Addr2"))   skind = Addr2Supp;
   else if (VG_STREQ(name, "Addr4"))   skind = Addr4Supp;
   else if (VG_STREQ(name, "Addr8"))   skind = Addr8Supp;
   else if (VG_STREQ(name, "Addr16"))  skind = Addr16Supp;
   else if (VG_STREQ(name, "Jump"))    skind = JumpSupp;
   else if (VG_STREQ(name, "Free"))    skind = FreeSupp;
   else if (VG_STREQ(name, "Leak"))    skind = LeakSupp;
   else if (VG_STREQ(name, "Overlap")) skind = OverlapSupp;
   else if (VG_STREQ(name, "Mempool")) skind = MempoolSupp;
   else if (VG_STREQ(name, "Cond"))    skind = CondSupp;
   else if (VG_STREQ(name, "Value0"))  skind = CondSupp; 
   else if (VG_STREQ(name, "Value1"))  skind = Value1Supp;
   else if (VG_STREQ(name, "Value2"))  skind = Value2Supp;
   else if (VG_STREQ(name, "Value4"))  skind = Value4Supp;
   else if (VG_STREQ(name, "Value8"))  skind = Value8Supp;
   else if (VG_STREQ(name, "Value16")) skind = Value16Supp;
   else 
      return False;

   VG_(set_supp_kind)(su, skind);
   return True;
}

Bool MC_(read_extra_suppression_info) ( Int fd, Char** bufpp,
                                        SizeT* nBufp, Supp *su )
{
   Bool eof;

   if (VG_(get_supp_kind)(su) == ParamSupp) {
      eof = VG_(get_line) ( fd, bufpp, nBufp, NULL );
      if (eof) return False;
      VG_(set_supp_string)(su, VG_(strdup)("mc.resi.1", *bufpp));
   }
   return True;
}

Bool MC_(error_matches_suppression) ( Error* err, Supp* su )
{
   Int       su_szB;
   MC_Error* extra = VG_(get_error_extra)(err);
   ErrorKind ekind = VG_(get_error_kind )(err);

   switch (VG_(get_supp_kind)(su)) {
      case ParamSupp:
         return ((ekind == Err_RegParam || ekind == Err_MemParam)
              && VG_STREQ(VG_(get_error_string)(err), 
                          VG_(get_supp_string)(su)));

      case UserSupp:
         return (ekind == Err_User);

      case CoreMemSupp:
         return (ekind == Err_CoreMem
              && VG_STREQ(VG_(get_error_string)(err),
                          VG_(get_supp_string)(su)));

      case Value1Supp: su_szB = 1; goto value_case;
      case Value2Supp: su_szB = 2; goto value_case;
      case Value4Supp: su_szB = 4; goto value_case;
      case Value8Supp: su_szB = 8; goto value_case;
      case Value16Supp:su_szB =16; goto value_case;
      value_case:
         return (ekind == Err_Value && extra->Err.Value.szB == su_szB);

      case CondSupp:
         return (ekind == Err_Cond);

      case Addr1Supp: su_szB = 1; goto addr_case;
      case Addr2Supp: su_szB = 2; goto addr_case;
      case Addr4Supp: su_szB = 4; goto addr_case;
      case Addr8Supp: su_szB = 8; goto addr_case;
      case Addr16Supp:su_szB =16; goto addr_case;
      addr_case:
         return (ekind == Err_Addr && extra->Err.Addr.szB == su_szB);

      case JumpSupp:
         return (ekind == Err_Jump);

      case FreeSupp:
         return (ekind == Err_Free || ekind == Err_FreeMismatch);

      case OverlapSupp:
         return (ekind == Err_Overlap);

      case LeakSupp:
         return (ekind == Err_Leak);

      case MempoolSupp:
         return (ekind == Err_IllegalMempool);

      default:
         VG_(printf)("Error:\n"
                     "  unknown suppression type %d\n",
                     VG_(get_supp_kind)(su));
         VG_(tool_panic)("unknown suppression type in "
                         "MC_(error_matches_suppression)");
   }
}

Char* MC_(get_error_name) ( Error* err )
{
   switch (VG_(get_error_kind)(err)) {
   case Err_RegParam:       return "Param";
   case Err_MemParam:       return "Param";
   case Err_User:           return "User";
   case Err_FreeMismatch:   return "Free";
   case Err_IllegalMempool: return "Mempool";
   case Err_Free:           return "Free";
   case Err_Jump:           return "Jump";
   case Err_CoreMem:        return "CoreMem";
   case Err_Overlap:        return "Overlap";
   case Err_Leak:           return "Leak";
   case Err_Cond:           return "Cond";
   case Err_Addr: {
      MC_Error* extra = VG_(get_error_extra)(err);
      switch ( extra->Err.Addr.szB ) {
      case 1:               return "Addr1";
      case 2:               return "Addr2";
      case 4:               return "Addr4";
      case 8:               return "Addr8";
      case 16:              return "Addr16";
      default:              VG_(tool_panic)("unexpected size for Addr");
      }
   }
   case Err_Value: {
      MC_Error* extra = VG_(get_error_extra)(err);
      switch ( extra->Err.Value.szB ) {
      case 1:               return "Value1";
      case 2:               return "Value2";
      case 4:               return "Value4";
      case 8:               return "Value8";
      case 16:              return "Value16";
      default:              VG_(tool_panic)("unexpected size for Value");
      }
   }
   default:                 VG_(tool_panic)("get_error_name: unexpected type");
   }
}

Bool MC_(get_extra_suppression_info) ( Error* err,
                                       Char* buf, Int nBuf )
{
   ErrorKind ekind = VG_(get_error_kind )(err);
   tl_assert(buf);
   tl_assert(nBuf >= 16); 
   if (Err_RegParam == ekind || Err_MemParam == ekind) {
      Char* errstr = VG_(get_error_string)(err);
      tl_assert(errstr);
      VG_(snprintf)(buf, nBuf-1, "%s", errstr);
      return True;
   } else {
      return False;
   }
}


