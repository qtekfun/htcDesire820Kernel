

/*
   This file is part of Lackey, an example Valgrind tool that does
   some simple program measurement and tracing.

   Copyright (C) 2002-2011 Nicholas Nethercote
      njn@valgrind.org

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

//   fstat() you know that the passed in buffer has been written.  But it


#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_options.h"
#include "pub_tool_machine.h"     


static Bool clo_basic_counts    = True;
static Bool clo_detailed_counts = False;
static Bool clo_trace_mem       = False;
static Bool clo_trace_sbs       = False;

static Char* clo_fnname = "main";

static Bool lk_process_cmd_line_option(Char* arg)
{
   if VG_STR_CLO(arg, "--fnname", clo_fnname) {}
   else if VG_BOOL_CLO(arg, "--basic-counts",      clo_basic_counts) {}
   else if VG_BOOL_CLO(arg, "--detailed-counts",   clo_detailed_counts) {}
   else if VG_BOOL_CLO(arg, "--trace-mem",         clo_trace_mem) {}
   else if VG_BOOL_CLO(arg, "--trace-superblocks", clo_trace_sbs) {}
   else
      return False;
   
   tl_assert(clo_fnname);
   tl_assert(clo_fnname[0]);
   return True;
}

static void lk_print_usage(void)
{  
   VG_(printf)(
"    --basic-counts=no|yes     count instructions, jumps, etc. [yes]\n"
"    --detailed-counts=no|yes  count loads, stores and alu ops [no]\n"
"    --trace-mem=no|yes        trace all loads and stores [no]\n"
"    --trace-superblocks=no|yes  trace all superblock entries [no]\n"
"    --fnname=<name>           count calls to <name> (only used if\n"
"                              --basic-count=yes)  [main]\n"
   );
}

static void lk_print_debug_usage(void)
{  
   VG_(printf)(
"    (none)\n"
   );
}


static ULong n_func_calls    = 0;
static ULong n_SBs_entered   = 0;
static ULong n_SBs_completed = 0;
static ULong n_IRStmts       = 0;
static ULong n_guest_instrs  = 0;
static ULong n_Jccs          = 0;
static ULong n_Jccs_untaken  = 0;
static ULong n_IJccs         = 0;
static ULong n_IJccs_untaken = 0;

static void add_one_func_call(void)
{
   n_func_calls++;
}

static void add_one_SB_entered(void)
{
   n_SBs_entered++;
}

static void add_one_SB_completed(void)
{
   n_SBs_completed++;
}

static void add_one_IRStmt(void)
{
   n_IRStmts++;
}

static void add_one_guest_instr(void)
{
   n_guest_instrs++;
}

static void add_one_Jcc(void)
{
   n_Jccs++;
}

static void add_one_Jcc_untaken(void)
{
   n_Jccs_untaken++;
}

static void add_one_inverted_Jcc(void)
{
   n_IJccs++;
}

static void add_one_inverted_Jcc_untaken(void)
{
   n_IJccs_untaken++;
}



typedef enum { OpLoad=0, OpStore=1, OpAlu=2 } Op;

#define N_OPS 3



#define N_TYPES 10

static Int type2index ( IRType ty )
{
   switch (ty) {
      case Ity_I1:      return 0;
      case Ity_I8:      return 1;
      case Ity_I16:     return 2;
      case Ity_I32:     return 3;
      case Ity_I64:     return 4;
      case Ity_I128:    return 5;
      case Ity_F32:     return 6;
      case Ity_F64:     return 7;
      case Ity_F128:    return 8;
      case Ity_V128:    return 9;
      default: tl_assert(0);
   }
}

static HChar* nameOfTypeIndex ( Int i )
{
   switch (i) {
      case 0: return "I1";   break;
      case 1: return "I8";   break;
      case 2: return "I16";  break;
      case 3: return "I32";  break;
      case 4: return "I64";  break;
      case 5: return "I128"; break;
      case 6: return "F32";  break;
      case 7: return "F64";  break;
      case 8: return "F128";  break;
      case 9: return "V128"; break;
      default: tl_assert(0);
   }
}



static ULong detailCounts[N_OPS][N_TYPES];

static VG_REGPARM(1)
void increment_detail(ULong* detail)
{
   (*detail)++;
}

static void instrument_detail(IRSB* sb, Op op, IRType type)
{
   IRDirty* di;
   IRExpr** argv;
   const UInt typeIx = type2index(type);

   tl_assert(op < N_OPS);
   tl_assert(typeIx < N_TYPES);

   argv = mkIRExprVec_1( mkIRExpr_HWord( (HWord)&detailCounts[op][typeIx] ) );
   di = unsafeIRDirty_0_N( 1, "increment_detail",
                              VG_(fnptr_to_fnentry)( &increment_detail ), 
                              argv);
   addStmtToIRSB( sb, IRStmt_Dirty(di) );
}

static void print_details ( void )
{
   Int typeIx;
   VG_(umsg)("   Type        Loads       Stores       AluOps\n");
   VG_(umsg)("   -------------------------------------------\n");
   for (typeIx = 0; typeIx < N_TYPES; typeIx++) {
      VG_(umsg)("   %4s %'12llu %'12llu %'12llu\n",
                nameOfTypeIndex( typeIx ),
                detailCounts[OpLoad ][typeIx],
                detailCounts[OpStore][typeIx],
                detailCounts[OpAlu  ][typeIx]
      );
   }
}



#define MAX_DSIZE    512

typedef
   IRExpr 
   IRAtom;

typedef 
   enum { Event_Ir, Event_Dr, Event_Dw, Event_Dm }
   EventKind;

typedef
   struct {
      EventKind  ekind;
      IRAtom*    addr;
      Int        size;
   }
   Event;

#define N_EVENTS 4


static Event events[N_EVENTS];
static Int   events_used = 0;


static VG_REGPARM(2) void trace_instr(Addr addr, SizeT size)
{
   VG_(printf)("I  %08lx,%lu\n", addr, size);
}

static VG_REGPARM(2) void trace_load(Addr addr, SizeT size)
{
   VG_(printf)(" L %08lx,%lu\n", addr, size);
}

static VG_REGPARM(2) void trace_store(Addr addr, SizeT size)
{
   VG_(printf)(" S %08lx,%lu\n", addr, size);
}

static VG_REGPARM(2) void trace_modify(Addr addr, SizeT size)
{
   VG_(printf)(" M %08lx,%lu\n", addr, size);
}


static void flushEvents(IRSB* sb)
{
   Int        i;
   Char*      helperName;
   void*      helperAddr;
   IRExpr**   argv;
   IRDirty*   di;
   Event*     ev;

   for (i = 0; i < events_used; i++) {

      ev = &events[i];
      
      
      switch (ev->ekind) {
         case Event_Ir: helperName = "trace_instr";
                        helperAddr =  trace_instr;  break;

         case Event_Dr: helperName = "trace_load";
                        helperAddr =  trace_load;   break;

         case Event_Dw: helperName = "trace_store";
                        helperAddr =  trace_store;  break;

         case Event_Dm: helperName = "trace_modify";
                        helperAddr =  trace_modify; break;
         default:
            tl_assert(0);
      }

      
      argv = mkIRExprVec_2( ev->addr, mkIRExpr_HWord( ev->size ) );
      di   = unsafeIRDirty_0_N( 2, 
                                helperName, VG_(fnptr_to_fnentry)( helperAddr ),
                                argv );
      addStmtToIRSB( sb, IRStmt_Dirty(di) );
   }

   events_used = 0;
}

static void addEvent_Ir ( IRSB* sb, IRAtom* iaddr, UInt isize )
{
   Event* evt;
   tl_assert(clo_trace_mem);
   tl_assert( (VG_MIN_INSTR_SZB <= isize && isize <= VG_MAX_INSTR_SZB)
            || VG_CLREQ_SZB == isize );
   if (events_used == N_EVENTS)
      flushEvents(sb);
   tl_assert(events_used >= 0 && events_used < N_EVENTS);
   evt = &events[events_used];
   evt->ekind = Event_Ir;
   evt->addr  = iaddr;
   evt->size  = isize;
   events_used++;
}

static
void addEvent_Dr ( IRSB* sb, IRAtom* daddr, Int dsize )
{
   Event* evt;
   tl_assert(clo_trace_mem);
   tl_assert(isIRAtom(daddr));
   tl_assert(dsize >= 1 && dsize <= MAX_DSIZE);
   if (events_used == N_EVENTS)
      flushEvents(sb);
   tl_assert(events_used >= 0 && events_used < N_EVENTS);
   evt = &events[events_used];
   evt->ekind = Event_Dr;
   evt->addr  = daddr;
   evt->size  = dsize;
   events_used++;
}

static
void addEvent_Dw ( IRSB* sb, IRAtom* daddr, Int dsize )
{
   Event* lastEvt;
   Event* evt;
   tl_assert(clo_trace_mem);
   tl_assert(isIRAtom(daddr));
   tl_assert(dsize >= 1 && dsize <= MAX_DSIZE);

   
   lastEvt = &events[events_used-1];
   if (events_used > 0
    && lastEvt->ekind == Event_Dr
    && lastEvt->size  == dsize
    && eqIRAtom(lastEvt->addr, daddr))
   {
      lastEvt->ekind = Event_Dm;
      return;
   }

   
   if (events_used == N_EVENTS)
      flushEvents(sb);
   tl_assert(events_used >= 0 && events_used < N_EVENTS);
   evt = &events[events_used];
   evt->ekind = Event_Dw;
   evt->size  = dsize;
   evt->addr  = daddr;
   events_used++;
}



static void trace_superblock(Addr addr)
{
   VG_(printf)("SB %08lx\n", addr);
}



static void lk_post_clo_init(void)
{
   Int op, tyIx;

   if (clo_detailed_counts) {
      for (op = 0; op < N_OPS; op++)
         for (tyIx = 0; tyIx < N_TYPES; tyIx++)
            detailCounts[op][tyIx] = 0;
   }
}

static
IRSB* lk_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn, 
                      VexGuestLayout* layout, 
                      VexGuestExtents* vge,
                      IRType gWordTy, IRType hWordTy )
{
   IRDirty*   di;
   Int        i;
   IRSB*      sbOut;
   Char       fnname[100];
   IRType     type;
   IRTypeEnv* tyenv = sbIn->tyenv;
   Addr       iaddr = 0, dst;
   UInt       ilen = 0;
   Bool       condition_inverted = False;

   if (gWordTy != hWordTy) {
      
      VG_(tool_panic)("host/guest word size mismatch");
   }

   
   sbOut = deepCopyIRSBExceptStmts(sbIn);

   
   i = 0;
   while (i < sbIn->stmts_used && sbIn->stmts[i]->tag != Ist_IMark) {
      addStmtToIRSB( sbOut, sbIn->stmts[i] );
      i++;
   }

   if (clo_basic_counts) {
      
      di = unsafeIRDirty_0_N( 0, "add_one_SB_entered", 
                                 VG_(fnptr_to_fnentry)( &add_one_SB_entered ),
                                 mkIRExprVec_0() );
      addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
   }

   if (clo_trace_sbs) {
      
      di = unsafeIRDirty_0_N( 
              0, "trace_superblock", 
              VG_(fnptr_to_fnentry)( &trace_superblock ),
              mkIRExprVec_1( mkIRExpr_HWord( vge->base[0] ) ) 
           );
      addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
   }

   if (clo_trace_mem) {
      events_used = 0;
   }

   for (; i < sbIn->stmts_used; i++) {
      IRStmt* st = sbIn->stmts[i];
      if (!st || st->tag == Ist_NoOp) continue;

      if (clo_basic_counts) {
         
         di = unsafeIRDirty_0_N( 0, "add_one_IRStmt", 
                                    VG_(fnptr_to_fnentry)( &add_one_IRStmt ), 
                                    mkIRExprVec_0() );
         addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
      }
      
      switch (st->tag) {
         case Ist_NoOp:
         case Ist_AbiHint:
         case Ist_Put:
         case Ist_PutI:
         case Ist_MBE:
            addStmtToIRSB( sbOut, st );
            break;

         case Ist_IMark:
            if (clo_basic_counts) {
               
               iaddr = st->Ist.IMark.addr;
               ilen  = st->Ist.IMark.len;

               
               di = unsafeIRDirty_0_N( 0, "add_one_guest_instr",
                                          VG_(fnptr_to_fnentry)( &add_one_guest_instr ), 
                                          mkIRExprVec_0() );
               addStmtToIRSB( sbOut, IRStmt_Dirty(di) );

               tl_assert(clo_fnname);
               tl_assert(clo_fnname[0]);
               if (VG_(get_fnname_if_entry)(st->Ist.IMark.addr, 
                                            fnname, sizeof(fnname))
                   && 0 == VG_(strcmp)(fnname, clo_fnname)) {
                  di = unsafeIRDirty_0_N( 
                          0, "add_one_func_call", 
                             VG_(fnptr_to_fnentry)( &add_one_func_call ), 
                             mkIRExprVec_0() );
                  addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
               }
            }
            if (clo_trace_mem) {
               
               
               
               addEvent_Ir( sbOut, mkIRExpr_HWord( (HWord)st->Ist.IMark.addr ),
                            st->Ist.IMark.len );
            }
            addStmtToIRSB( sbOut, st );
            break;

         case Ist_WrTmp:
            
            if (clo_trace_mem) {
               IRExpr* data = st->Ist.WrTmp.data;
               if (data->tag == Iex_Load) {
                  addEvent_Dr( sbOut, data->Iex.Load.addr,
                               sizeofIRType(data->Iex.Load.ty) );
               }
            }
            if (clo_detailed_counts) {
               IRExpr* expr = st->Ist.WrTmp.data;
               type = typeOfIRExpr(sbOut->tyenv, expr);
               tl_assert(type != Ity_INVALID);
               switch (expr->tag) {
                  case Iex_Load:
                     instrument_detail( sbOut, OpLoad, type );
                     break;
                  case Iex_Unop:
                  case Iex_Binop:
                  case Iex_Triop:
                  case Iex_Qop:
                  case Iex_Mux0X:
                     instrument_detail( sbOut, OpAlu, type );
                     break;
                  default:
                     break;
               }
            }
            addStmtToIRSB( sbOut, st );
            break;

         case Ist_Store:
            if (clo_trace_mem) {
               IRExpr* data  = st->Ist.Store.data;
               addEvent_Dw( sbOut, st->Ist.Store.addr,
                            sizeofIRType(typeOfIRExpr(tyenv, data)) );
            }
            if (clo_detailed_counts) {
               type = typeOfIRExpr(sbOut->tyenv, st->Ist.Store.data);
               tl_assert(type != Ity_INVALID);
               instrument_detail( sbOut, OpStore, type );
            }
            addStmtToIRSB( sbOut, st );
            break;

         case Ist_Dirty: {
            if (clo_trace_mem) {
               Int      dsize;
               IRDirty* d = st->Ist.Dirty.details;
               if (d->mFx != Ifx_None) {
                  
                  tl_assert(d->mAddr != NULL);
                  tl_assert(d->mSize != 0);
                  dsize = d->mSize;
                  if (d->mFx == Ifx_Read || d->mFx == Ifx_Modify)
                     addEvent_Dr( sbOut, d->mAddr, dsize );
                  if (d->mFx == Ifx_Write || d->mFx == Ifx_Modify)
                     addEvent_Dw( sbOut, d->mAddr, dsize );
               } else {
                  tl_assert(d->mAddr == NULL);
                  tl_assert(d->mSize == 0);
               }
            }
            addStmtToIRSB( sbOut, st );
            break;
         }

         case Ist_CAS: {
            Int    dataSize;
            IRType dataTy;
            IRCAS* cas = st->Ist.CAS.details;
            tl_assert(cas->addr != NULL);
            tl_assert(cas->dataLo != NULL);
            dataTy   = typeOfIRExpr(tyenv, cas->dataLo);
            dataSize = sizeofIRType(dataTy);
            if (cas->dataHi != NULL)
               dataSize *= 2; 
            if (clo_trace_mem) {
               addEvent_Dr( sbOut, cas->addr, dataSize );
               addEvent_Dw( sbOut, cas->addr, dataSize );
            }
            if (clo_detailed_counts) {
               instrument_detail( sbOut, OpLoad, dataTy );
               if (cas->dataHi != NULL) 
                  instrument_detail( sbOut, OpLoad, dataTy );
               instrument_detail( sbOut, OpStore, dataTy );
               if (cas->dataHi != NULL) 
                  instrument_detail( sbOut, OpStore, dataTy );
            }
            addStmtToIRSB( sbOut, st );
            break;
         }

         case Ist_LLSC: {
            IRType dataTy;
            if (st->Ist.LLSC.storedata == NULL) {
               
               dataTy = typeOfIRTemp(tyenv, st->Ist.LLSC.result);
               if (clo_trace_mem)
                  addEvent_Dr( sbOut, st->Ist.LLSC.addr,
                                      sizeofIRType(dataTy) );
               if (clo_detailed_counts)
                  instrument_detail( sbOut, OpLoad, dataTy );
            } else {
               
               dataTy = typeOfIRExpr(tyenv, st->Ist.LLSC.storedata);
               if (clo_trace_mem)
                  addEvent_Dw( sbOut, st->Ist.LLSC.addr,
                                      sizeofIRType(dataTy) );
               if (clo_detailed_counts)
                  instrument_detail( sbOut, OpStore, dataTy );
            }
            addStmtToIRSB( sbOut, st );
            break;
         }

         case Ist_Exit:
            if (clo_basic_counts) {
               
               
               tl_assert(iaddr != 0);
               dst = (sizeof(Addr) == 4) ? st->Ist.Exit.dst->Ico.U32 :
                                           st->Ist.Exit.dst->Ico.U64;
               condition_inverted = (dst == iaddr + ilen);

               
               if (!condition_inverted)
                  di = unsafeIRDirty_0_N( 0, "add_one_Jcc", 
                                          VG_(fnptr_to_fnentry)( &add_one_Jcc ), 
                                          mkIRExprVec_0() );
               else
                  di = unsafeIRDirty_0_N( 0, "add_one_inverted_Jcc",
                                          VG_(fnptr_to_fnentry)(
                                             &add_one_inverted_Jcc ),
                                          mkIRExprVec_0() );

               addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
            }
            if (clo_trace_mem) {
               flushEvents(sbOut);
            }

            addStmtToIRSB( sbOut, st );      

            if (clo_basic_counts) {
               
               if (!condition_inverted)
                  di = unsafeIRDirty_0_N( 0, "add_one_Jcc_untaken", 
                                          VG_(fnptr_to_fnentry)(
                                             &add_one_Jcc_untaken ),
                                          mkIRExprVec_0() );
               else
                  di = unsafeIRDirty_0_N( 0, "add_one_inverted_Jcc_untaken",
                                          VG_(fnptr_to_fnentry)(
                                             &add_one_inverted_Jcc_untaken ),
                                          mkIRExprVec_0() );

               addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
            }
            break;

         default:
            tl_assert(0);
      }
   }

   if (clo_basic_counts) {
      
      di = unsafeIRDirty_0_N( 0, "add_one_SB_completed", 
                                 VG_(fnptr_to_fnentry)( &add_one_SB_completed ),
                                 mkIRExprVec_0() );
      addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
   }

   if (clo_trace_mem) {
      
      flushEvents(sbOut);
   }

   return sbOut;
}

static void lk_fini(Int exitcode)
{
   char percentify_buf[5]; 
   const int percentify_size = sizeof(percentify_buf) - 1;
   const int percentify_decs = 0;
   
   tl_assert(clo_fnname);
   tl_assert(clo_fnname[0]);

   if (clo_basic_counts) {
      ULong total_Jccs = n_Jccs + n_IJccs;
      ULong taken_Jccs = (n_Jccs - n_Jccs_untaken) + n_IJccs_untaken;

      VG_(umsg)("Counted %'llu call%s to %s()\n",
                n_func_calls, ( n_func_calls==1 ? "" : "s" ), clo_fnname);

      VG_(umsg)("\n");
      VG_(umsg)("Jccs:\n");
      VG_(umsg)("  total:         %'llu\n", total_Jccs);
      VG_(percentify)(taken_Jccs, (total_Jccs ? total_Jccs : 1),
         percentify_decs, percentify_size, percentify_buf);
      VG_(umsg)("  taken:         %'llu (%s)\n",
         taken_Jccs, percentify_buf);
      
      VG_(umsg)("\n");
      VG_(umsg)("Executed:\n");
      VG_(umsg)("  SBs entered:   %'llu\n", n_SBs_entered);
      VG_(umsg)("  SBs completed: %'llu\n", n_SBs_completed);
      VG_(umsg)("  guest instrs:  %'llu\n", n_guest_instrs);
      VG_(umsg)("  IRStmts:       %'llu\n", n_IRStmts);
      
      VG_(umsg)("\n");
      VG_(umsg)("Ratios:\n");
      tl_assert(n_SBs_entered); 
      VG_(umsg)("  guest instrs : SB entered  = %'llu : 10\n",
         10 * n_guest_instrs / n_SBs_entered);
      VG_(umsg)("       IRStmts : SB entered  = %'llu : 10\n",
         10 * n_IRStmts / n_SBs_entered);
      tl_assert(n_guest_instrs); 
      VG_(umsg)("       IRStmts : guest instr = %'llu : 10\n",
         10 * n_IRStmts / n_guest_instrs);
   }

   if (clo_detailed_counts) {
      VG_(umsg)("\n");
      VG_(umsg)("IR-level counts by type:\n");
      print_details();
   }

   if (clo_basic_counts) {
      VG_(umsg)("\n");
      VG_(umsg)("Exit code:       %d\n", exitcode);
   }
}

static void lk_pre_clo_init(void)
{
   VG_(details_name)            ("Lackey");
   VG_(details_version)         (NULL);
   VG_(details_description)     ("an example Valgrind tool");
   VG_(details_copyright_author)(
      "Copyright (C) 2002-2011, and GNU GPL'd, by Nicholas Nethercote.");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);
   VG_(details_avg_translation_sizeB) ( 200 );

   VG_(basic_tool_funcs)          (lk_post_clo_init,
                                   lk_instrument,
                                   lk_fini);
   VG_(needs_command_line_options)(lk_process_cmd_line_option,
                                   lk_print_usage,
                                   lk_print_debug_usage);
}

VG_DETERMINE_INTERFACE_VERSION(lk_pre_clo_init)

