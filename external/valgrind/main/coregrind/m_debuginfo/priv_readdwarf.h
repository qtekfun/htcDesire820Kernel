

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

#ifndef __PRIV_READDWARF_H
#define __PRIV_READDWARF_H



extern
void ML_(read_debuginfo_dwarf3)
        ( struct _DebugInfo* di,
          UChar* debug_info_img, Word debug_info_sz,  
          UChar* debug_types_img, Word debug_types_sz,  
          UChar* debug_abbv_img, Word debug_abbv_sz,  
          UChar* debug_line_img, Word debug_line_sz,  
          UChar* debug_str_img,  Word debug_str_sz,   
          UChar* debug_str_alt_img, Word debug_str_alt_sz ); 

extern
void ML_(read_debuginfo_dwarf1) ( struct _DebugInfo* di,
                                  UChar* dwarf1d, Int dwarf1d_sz,
                                  UChar* dwarf1l, Int dwarf1l_sz );

extern
void ML_(read_callframe_info_dwarf3)
        ( struct _DebugInfo* di,
          UChar* frame_image, SizeT frame_size, Addr frame_avma,
          Bool is_ehframe );


#endif 

