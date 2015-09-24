

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2005-2012 Nicholas Nethercote
      njn@valgrind.org
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

#ifndef __PUB_TOOL_VKISCNUMS_H
#define __PUB_TOOL_VKISCNUMS_H

#include "pub_tool_vkiscnums_asm.h"


extern Char* VG_(sysnum_string)      (Word sysnum, SizeT n_buf, Char* buf);

extern Char* VG_(sysnum_string_extra)(Word sysnum, SizeT n_buf, Char* buf);

#define VG_SYSNUM_STRING(sysnum) \
   ({ Char qq_zz_buf[32]; VG_(sysnum_string)(sysnum, 32, qq_zz_buf); })
#define VG_SYSNUM_STRING_EXTRA(sysnum) \
   ({ Char qq_zz_buf[64]; VG_(sysnum_string_extra)(sysnum, 64, qq_zz_buf); })


#endif   
