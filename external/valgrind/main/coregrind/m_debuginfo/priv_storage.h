

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

#ifndef __PRIV_STORAGE_H
#define __PRIV_STORAGE_H


typedef 
   struct { 
      Addr    addr;    
      Addr    tocptr;  
      UChar*  pri_name;  
      UChar** sec_names; 
      
      
      
      
      
      UInt    size;    
      Bool    isText;
      Bool    isIFunc; 
   }
   DiSym;


#define LINENO_OVERFLOW (1 << (sizeof(short) * 8))

#define LINENO_BITS     20
#define LOC_SIZE_BITS  (32 - LINENO_BITS)
#define MAX_LINENO     ((1 << LINENO_BITS) - 1)

#define MAX_LOC_SIZE   ((1 << LOC_SIZE_BITS) - 1)

#define OVERFLOW_DIFFERENCE     (LINENO_OVERFLOW - 5000)

typedef
   struct {
      
      Addr   addr;               
      
      UShort size:LOC_SIZE_BITS; 
      UInt   lineno:LINENO_BITS; 
      
      UChar*  filename;          
      
      UChar*  dirname;           
   }
   DiLoc;



#define CFIC_IA_SPREL     ((UChar)1)
#define CFIC_IA_BPREL     ((UChar)2)
#define CFIC_IA_EXPR      ((UChar)3)
#define CFIC_ARM_R13REL   ((UChar)4)
#define CFIC_ARM_R12REL   ((UChar)5)
#define CFIC_ARM_R11REL   ((UChar)6)
#define CFIC_ARM_R7REL    ((UChar)7)
#define CFIC_EXPR         ((UChar)8)  

#define CFIR_UNKNOWN      ((UChar)64)
#define CFIR_SAME         ((UChar)65)
#define CFIR_CFAREL       ((UChar)66)
#define CFIR_MEMCFAREL    ((UChar)67)
#define CFIR_EXPR         ((UChar)68)

#if defined(VGA_x86) || defined(VGA_amd64)
typedef
   struct {
      Addr  base;
      UInt  len;
      UChar cfa_how; 
      UChar ra_how;  
      UChar sp_how;  
      UChar bp_how;  
      Int   cfa_off;
      Int   ra_off;
      Int   sp_off;
      Int   bp_off;
   }
   DiCfSI;
#elif defined(VGA_arm)
typedef
   struct {
      Addr  base;
      UInt  len;
      UChar cfa_how; 
      UChar ra_how;  
      UChar r14_how; 
      UChar r13_how; 
      UChar r12_how; 
      UChar r11_how; 
      UChar r7_how;  
      Int   cfa_off;
      Int   ra_off;
      Int   r14_off;
      Int   r13_off;
      Int   r12_off;
      Int   r11_off;
      Int   r7_off;
   }
   DiCfSI;
#elif defined(VGA_ppc32) || defined(VGA_ppc64)
typedef
   struct {
      Addr  base;
      UInt  len;
      UChar cfa_how; 
      UChar ra_how;  
      Int   cfa_off;
      Int   ra_off;
   }
   DiCfSI;
#elif defined(VGA_s390x)
typedef
   struct {
      Addr  base;
      UInt  len;
      UChar cfa_how; 
      UChar sp_how;  
      UChar ra_how;  
      UChar fp_how;  
      Int   cfa_off;
      Int   sp_off;
      Int   ra_off;
      Int   fp_off;
   }
   DiCfSI;
#elif defined(VGA_mips32)
typedef
   struct {
      Addr  base;
      UInt  len;
      UChar cfa_how; 
      UChar ra_how;  
      UChar sp_how;  
      UChar fp_how;  
      Int   cfa_off;
      Int   ra_off;
      Int   sp_off;
      Int   fp_off;
   }
   DiCfSI;
#else
#  error "Unknown arch"
#endif


typedef
   enum {
      Cop_Add=0x321,
      Cop_Sub,
      Cop_And,
      Cop_Mul,
      Cop_Shl,
      Cop_Shr,
      Cop_Eq,
      Cop_Ge,
      Cop_Gt,
      Cop_Le,
      Cop_Lt,
      Cop_Ne
   }
   CfiOp;

typedef
   enum {
      Creg_IA_SP=0x213,
      Creg_IA_BP,
      Creg_IA_IP,
      Creg_ARM_R13,
      Creg_ARM_R12,
      Creg_ARM_R15,
      Creg_ARM_R14,
      Creg_S390_R14,
      Creg_MIPS_RA
   }
   CfiReg;

typedef
   enum {
      Cex_Undef=0x123,
      Cex_Deref,
      Cex_Const,
      Cex_Binop,
      Cex_CfiReg,
      Cex_DwReg
   }
   CfiExprTag;

typedef 
   struct {
      CfiExprTag tag;
      union {
         struct {
         } Undef;
         struct {
            Int ixAddr;
         } Deref;
         struct {
            UWord con;
         } Const;
         struct {
            CfiOp op;
            Int ixL;
            Int ixR;
         } Binop;
         struct {
            CfiReg reg;
         } CfiReg;
         struct {
            Int reg;
         } DwReg;
      }
      Cex;
   }
   CfiExpr;

extern Int ML_(CfiExpr_Undef) ( XArray* dst );
extern Int ML_(CfiExpr_Deref) ( XArray* dst, Int ixAddr );
extern Int ML_(CfiExpr_Const) ( XArray* dst, UWord con );
extern Int ML_(CfiExpr_Binop) ( XArray* dst, CfiOp op, Int ixL, Int ixR );
extern Int ML_(CfiExpr_CfiReg)( XArray* dst, CfiReg reg );
extern Int ML_(CfiExpr_DwReg) ( XArray* dst, Int reg );

extern void ML_(ppCfiExpr)( XArray* src, Int ix );


typedef
   struct _FPO_DATA {  
      UInt   ulOffStart; 
      UInt   cbProcSize; 
      UInt   cdwLocals;  
      UShort cdwParams;  
      UChar  cbProlog;   
      UChar  cbRegs :3;  
      UChar  fHasSEH:1;  
      UChar  fUseBP :1;  
      UChar  reserved:1;
      UChar  cbFrame:2;  
   }
   FPO_DATA;

#define PDB_FRAME_FPO  0
#define PDB_FRAME_TRAP 1
#define PDB_FRAME_TSS  2


typedef
   struct {
      Addr    aMin;
      Addr    aMax;
      XArray*  vars;
   }
   DiAddrRange;

typedef
   struct {
      UChar* name;  
      UWord  typeR; 
      GExpr* gexpr; 
      GExpr* fbGX;  
      UChar* fileName; 
      Int    lineNo;   
   }
   DiVariable;

Word 
ML_(cmp_for_DiAddrRange_range) ( const void* keyV, const void* elemV );




struct _DebugInfoMapping
{
   Addr  avma; 
   SizeT size; 
   OffT  foff;
   Bool  rx, rw, ro;  
};

struct _DebugInfoFSM
{
   UChar*  filename;  
   XArray* maps;      
   Bool  have_rx_map; 
   Bool  have_rw_map; 
   Bool  have_ro_map; 
};


#define SEGINFO_STRCHUNKSIZE (64*1024)


#define N_EHFRAME_SECTS 2



struct _DebugInfo {

   

   struct _DebugInfo* next;   
   Bool               mark;   

   ULong handle;


   Bool trace_symtab; 
   Bool trace_cfi;    
   Bool ddump_syms;   
   Bool ddump_line;   
   Bool ddump_frames; 

   struct _DebugInfoFSM fsm;

   Bool  have_dinfo; 


   
   UChar* soname;

   
   Bool     text_present;
   Addr     text_avma;
   Addr     text_svma;
   SizeT    text_size;
   PtrdiffT text_bias;
   Addr     text_debug_svma;
   PtrdiffT text_debug_bias;
   
   Bool     data_present;
   Addr     data_svma;
   Addr     data_avma;
   SizeT    data_size;
   PtrdiffT data_bias;
   Addr     data_debug_svma;
   PtrdiffT data_debug_bias;
   
   Bool     sdata_present;
   Addr     sdata_svma;
   Addr     sdata_avma;
   SizeT    sdata_size;
   PtrdiffT sdata_bias;
   Addr     sdata_debug_svma;
   PtrdiffT sdata_debug_bias;
   
   Bool     rodata_present;
   Addr     rodata_svma;
   Addr     rodata_avma;
   SizeT    rodata_size;
   PtrdiffT rodata_bias;
   Addr     rodata_debug_svma;
   PtrdiffT rodata_debug_bias;
   
   Bool     bss_present;
   Addr     bss_svma;
   Addr     bss_avma;
   SizeT    bss_size;
   PtrdiffT bss_bias;
   Addr     bss_debug_svma;
   PtrdiffT bss_debug_bias;
   
   Bool     sbss_present;
   Addr     sbss_svma;
   Addr     sbss_avma;
   SizeT    sbss_size;
   PtrdiffT sbss_bias;
   Addr     sbss_debug_svma;
   PtrdiffT sbss_debug_bias;
   
   Bool   plt_present;
   Addr	  plt_avma;
   SizeT  plt_size;
   
   Bool   got_present;
   Addr   got_avma;
   SizeT  got_size;
   
   Bool   gotplt_present;
   Addr   gotplt_avma;
   SizeT  gotplt_size;
   
   Bool   opd_present;
   Addr   opd_avma;
   SizeT  opd_size;
   UInt   n_ehframe;  
   Addr   ehframe_avma[N_EHFRAME_SECTS];
   SizeT  ehframe_size[N_EHFRAME_SECTS];


   
   DiSym*  symtab;
   UWord   symtab_used;
   UWord   symtab_size;
   
   DiLoc*  loctab;
   UWord   loctab_used;
   UWord   loctab_size;
   DiCfSI* cfsi;
   UWord   cfsi_used;
   UWord   cfsi_size;
   Addr    cfsi_minavma;
   Addr    cfsi_maxavma;
   XArray* cfsi_exprs; 

   FPO_DATA* fpo;
   UWord     fpo_size;
   Addr      fpo_minavma;
   Addr      fpo_maxavma;
   Addr      fpo_base_avma;

   struct strchunk {
      UInt   strtab_used;
      struct strchunk* next;
      UChar  strtab[SEGINFO_STRCHUNKSIZE];
   } *strchunks;

   XArray* varinfo;


   XArray*  admin_tyents;

   
   XArray* admin_gexprs;

   struct _DebugInfoMapping* last_rx_map;
};



extern void ML_(addSym) ( struct _DebugInfo* di, DiSym* sym );

extern
void ML_(addLineInfo) ( struct _DebugInfo* di, 
                        UChar*   filename, 
                        UChar*   dirname,  
                        Addr this, Addr next, Int lineno, Int entry);

extern void ML_(addDiCfSI) ( struct _DebugInfo* di, DiCfSI* cfsi );

extern UChar* ML_(addStr) ( struct _DebugInfo* di, UChar* str, Int len );

extern void ML_(addVar)( struct _DebugInfo* di,
                         Int    level,
                         Addr   aMin,
                         Addr   aMax,
                         UChar* name,
                         UWord  typeR, 
                         GExpr* gexpr,
                         GExpr* fbGX, 
                         UChar* fileName, 
                         Int    lineNo, 
                         Bool   show );

extern void ML_(canonicaliseTables) ( struct _DebugInfo* di );

extern void ML_(canonicaliseCFI) ( struct _DebugInfo* di );


extern Word ML_(search_one_symtab) ( struct _DebugInfo* di, Addr ptr,
                                     Bool match_anywhere_in_sym,
                                     Bool findText );

extern Word ML_(search_one_loctab) ( struct _DebugInfo* di, Addr ptr );

extern Word ML_(search_one_cfitab) ( struct _DebugInfo* di, Addr ptr );

extern Word ML_(search_one_fpotab) ( struct _DebugInfo* di, Addr ptr );

extern struct _DebugInfoMapping* ML_(find_rx_mapping) ( struct _DebugInfo* di,
                                                        Addr lo, Addr hi );


extern 
void ML_(symerr) ( struct _DebugInfo* di, Bool serious, HChar* msg );

extern void ML_(ppSym) ( Int idx, DiSym* sym );

extern void ML_(ppDiCfSI) ( XArray*  exprs, DiCfSI* si );


#define TRACE_SYMTAB(format, args...) \
   if (di->trace_symtab) { VG_(printf)(format, ## args); }


#endif 

