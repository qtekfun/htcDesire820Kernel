

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2004-2012 OpenWorks LLP
      info@open-works.net

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

   The GNU General Public License is contained in the file COPYING.

   Neither the names of the U.S. Department of Energy nor the
   University of California nor the names of its contributors may be
   used to endorse or promote products derived from this software
   without prior written permission.
*/

#ifndef __VEX_HOST_GENERIC_REGS_H
#define __VEX_HOST_GENERIC_REGS_H

#include "libvex_basictypes.h"




typedef UInt HReg;

typedef
   enum { 
      HRcINVALID=1,   
      HRcInt32=3,     
      HRcInt64=4,     
      HRcFlt32=5,     
      HRcFlt64=6,     
      HRcVec64=7,     
      HRcVec128=8     
   }
   HRegClass;

extern void ppHRegClass ( HRegClass );


extern void ppHReg ( HReg );

static inline HReg mkHReg ( UInt regno, HRegClass rc, Bool virtual ) {
   UInt r24 = regno & 0x00FFFFFF;
   if (r24 != regno)
      vpanic("mkHReg: regno exceeds 2^24");
   return regno | (((UInt)rc) << 28) | (virtual ? (1<<24) : 0);
}

static inline HRegClass hregClass ( HReg r ) {
   UInt rc = r;
   rc = (rc >> 28) & 0x0F;
   vassert(rc >= HRcInt32 && rc <= HRcVec128);
   return (HRegClass)rc;
}

static inline UInt hregNumber ( HReg r ) {
   return ((UInt)r) & 0x00FFFFFF;
}

static inline Bool hregIsVirtual ( HReg r ) {
   return toBool(((UInt)r) & (1<<24));
}




#define INVALID_HREG ((HReg)0xFFFFFFFF)



typedef
   enum { HRmRead, HRmWrite, HRmModify }
   HRegMode;


#define N_HREG_USAGE 25

typedef
   struct {
      HReg     hreg[N_HREG_USAGE];
      HRegMode mode[N_HREG_USAGE];
      Int      n_used;
   }
   HRegUsage;

extern void ppHRegUsage ( HRegUsage* );

static inline void initHRegUsage ( HRegUsage* tab ) {
   tab->n_used = 0;
}

extern void addHRegUse ( HRegUsage*, HRegMode, HReg );





#define N_HREG_REMAP 6

typedef
   struct {
      HReg orig       [N_HREG_REMAP];
      HReg replacement[N_HREG_REMAP];
      Int  n_used;
   }
   HRegRemap;

extern void ppHRegRemap     ( HRegRemap* );
extern void initHRegRemap   ( HRegRemap* );
extern void addToHRegRemap  ( HRegRemap*, HReg, HReg );
extern HReg lookupHRegRemap ( HRegRemap*, HReg );




typedef  void  HInstr;


typedef
   struct {
      HInstr** arr;
      Int      arr_size;
      Int      arr_used;
      Int      n_vregs;
   }
   HInstrArray;

extern HInstrArray* newHInstrArray ( void );
extern void         addHInstr ( HInstrArray*, HInstr* );



extern
HInstrArray* doRegisterAllocation (

    
   HInstrArray* instrs_in,

   HReg* available_real_regs,
   Int   n_available_real_regs,

   Bool (*isMove) (HInstr*, HReg*, HReg*),

   
   void (*getRegUsage) (HRegUsage*, HInstr*, Bool),

   
   void (*mapRegs) (HRegRemap*, HInstr*, Bool),

   void    (*genSpill) (  HInstr**, HInstr**, HReg, Int, Bool ),
   void    (*genReload) ( HInstr**, HInstr**, HReg, Int, Bool ),
   HInstr* (*directReload) ( HInstr*, HReg, Short ),
   Int     guest_sizeB,

   
   void (*ppInstr) ( HInstr*, Bool ),
   void (*ppReg) ( HReg ),

   
   Bool mode64
);


#endif 

