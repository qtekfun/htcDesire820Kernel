

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

#ifndef __LIBVEX_PUB_GUEST_MIPS32_H
#define __LIBVEX_PUB_GUEST_MIPS32_H

#include "libvex_basictypes.h"
#include "libvex_emwarn.h"



typedef
   struct {
      
       UInt guest_r0; 
       UInt guest_r1;   
       UInt guest_r2;   
       UInt guest_r3;   
       UInt guest_r4;   
       UInt guest_r5;
       UInt guest_r6;
       UInt guest_r7;
       UInt guest_r8;   
       UInt guest_r9;
       UInt guest_r10;
       UInt guest_r11;
       UInt guest_r12;
       UInt guest_r13;
       UInt guest_r14;
       UInt guest_r15;
       UInt guest_r16;   
       UInt guest_r17;
       UInt guest_r18;
       UInt guest_r19;
       UInt guest_r20;
       UInt guest_r21;
       UInt guest_r22;
       UInt guest_r23;
       UInt guest_r24;   
       UInt guest_r25;
       UInt guest_r26;   
       UInt guest_r27;
       UInt guest_r28;   
       UInt guest_r29;   
       UInt guest_r30;   
       UInt guest_r31;   
       UInt guest_PC;   
       UInt guest_HI;
       UInt guest_LO;

      
       UInt guest_f0; 
       UInt guest_f1;
       UInt guest_f2;
       UInt guest_f3;
       UInt guest_f4;
       UInt guest_f5;
       UInt guest_f6;
       UInt guest_f7;
       UInt guest_f8;
       UInt guest_f9;
       UInt guest_f10;
       UInt guest_f11;
       UInt guest_f12;
       UInt guest_f13;
       UInt guest_f14;
       UInt guest_f15;
       UInt guest_f16;
       UInt guest_f17;
       UInt guest_f18;
       UInt guest_f19;
       UInt guest_f20;
       UInt guest_f21;
       UInt guest_f22;
       UInt guest_f23;
       UInt guest_f24;
       UInt guest_f25;
       UInt guest_f26;
       UInt guest_f27;
       UInt guest_f28;
       UInt guest_f29;
       UInt guest_f30;
       UInt guest_f31;
  
       UInt guest_FIR;
       UInt guest_FCCR;
       UInt guest_FEXR;
       UInt guest_FENR;
       UInt guest_FCSR;

       UInt guest_ULR;

      
          UInt   guest_EMWARN;  

      
        UInt guest_TISTART;     
        UInt guest_TILEN;        
        UInt guest_NRADDR;      

        UInt host_EvC_FAILADDR; 
        UInt host_EvC_COUNTER;  
        UInt guest_COND;        
} VexGuestMIPS32State;



extern
void LibVEX_GuestMIPS32_initialise ( VexGuestMIPS32State* vex_state );


#endif 


