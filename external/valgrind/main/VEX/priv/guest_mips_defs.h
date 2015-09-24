

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2010-2012 RT-RK
      mips-valgrind@rt-rk.com

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


#ifndef __VEX_GUEST_MIPS_DEFS_H
#define __VEX_GUEST_MIPS_DEFS_H


extern DisResult disInstr_MIPS ( IRSB*        irbb,
                                 Bool         (*resteerOkFn) (void *, Addr64),
                                 Bool         resteerCisOk,
                                 void*        callback_opaque,
                                 UChar*       guest_code,
                                 Long         delta,
                                 Addr64       guest_IP,
                                 VexArch      guest_arch,
                                 VexArchInfo* archinfo,
                                 VexAbiInfo*  abiinfo,
                                 Bool         host_bigendian );

extern IRExpr *guest_mips32_spechelper(HChar * function_name, IRExpr ** args,
                                       IRStmt ** precedingStmts,
                                       Int n_precedingStmts);

extern Bool guest_mips32_state_requires_precise_mem_exns(Int, Int);

extern VexGuestLayout mips32Guest_layout;


extern UInt mips32_dirtyhelper_mfc0(UInt rd, UInt sel);

extern void mips32_dirtyhelper_sync(UInt sync);



typedef enum {
   MIPSCondEQ = 0,      
   MIPSCondNE = 1,      

   MIPSCondHS = 2,      
   MIPSCondLO = 3,      

   MIPSCondMI = 4,      
   MIPSCondPL = 5,      

   MIPSCondVS = 6,      
   MIPSCondVC = 7,      

   MIPSCondHI = 8,      
   MIPSCondLS = 9,      

   MIPSCondGE = 10,  
   MIPSCondLT = 11,  

   MIPSCondGT = 12,  
   MIPSCondLE = 13,  

   MIPSCondAL = 14,  
   MIPSCondNV = 15      
} MIPSCondcode;

#endif            

