/* Copyright (C) 2007-2010 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/


#ifndef ELFF_DWARF_UTILS_
#define ELFF_DWARF_UTILS_

#include "dwarf_defs.h"

const char* dwarf_at_name(Dwarf_At at);

const char* dwarf_form_name(Dwarf_Form form);

const char* dwarf_tag_name(Dwarf_Tag tag);

void dump_attrib(Dwarf_At at, Dwarf_Form form, const Dwarf_Value* val);

void dump_value(const Dwarf_Value* val);

#endif  
