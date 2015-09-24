
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

#ifndef __PUB_TOOL_EXECONTEXT_H
#define __PUB_TOOL_EXECONTEXT_H

typedef
   struct _ExeContext
   ExeContext;

typedef
   enum { Vg_LowRes, Vg_MedRes, Vg_HighRes }
   VgRes;

extern 
ExeContext* VG_(record_ExeContext) ( ThreadId tid, Word first_ip_delta );

extern
ExeContext* VG_(record_depth_1_ExeContext)(ThreadId tid, Word first_ip_delta);

extern void VG_(apply_ExeContext)( void(*action)(UInt n, Addr ip),
                                   ExeContext* ec, UInt n_ips );

extern Bool VG_(eq_ExeContext) ( VgRes res, ExeContext* e1, ExeContext* e2 );

extern void VG_(pp_ExeContext) ( ExeContext* ec );

extern UInt VG_(get_ECU_from_ExeContext)( ExeContext* e );

extern Int VG_(get_ExeContext_n_ips)( ExeContext* e );

extern ExeContext* VG_(get_ExeContext_from_ECU)( UInt uniq );

ExeContext* VG_(make_depth_1_ExeContext_from_Addr)( Addr a );

static inline Bool VG_(is_plausible_ECU)( UInt ecu ) {
   return (ecu > 0) && ((ecu & 3) == 0);
}

ExeContext* VG_(make_ExeContext_from_StackTrace)( Addr* ips, UInt n_ips );

#endif   

