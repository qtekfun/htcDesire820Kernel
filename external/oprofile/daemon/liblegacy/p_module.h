/* Definitions for the Linux module syscall interface.
   Copyright 1996, 1997 Linux International.

   Contributed by Richard Henderson <rth@tamu.edu>

   This file is part of the Linux modutils.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef MODUTILS_MODULE_H
#define MODUTILS_MODULE_H 1




struct old_kernel_sym
{
  unsigned long value;
  char name[60];
};

struct old_module_ref
{
  unsigned long module;		
  unsigned long next;
};

struct old_module_symbol
{
  unsigned long addr;
  unsigned long name;
};

struct old_symbol_table
{
  int size;			
  int n_symbols;
  int n_refs;
  struct old_module_symbol symbol[0]; 
  struct old_module_ref ref[0];	
};

struct old_mod_routines
{
  unsigned long init;
  unsigned long cleanup;
};

struct old_module
{
  unsigned long next;
  unsigned long ref;		
  unsigned long symtab;
  unsigned long name;
  int size;			
  unsigned long addr;		
  int state;
  unsigned long cleanup;	
};

#define OLD_MOD_AUTOCLEAN 0x40000000 

int get_kernel_syms(struct old_kernel_sym *);
int old_sys_init_module(char const * name, char *code, unsigned codesize,
			struct old_mod_routines *, struct old_symbol_table *);


#define tgt_sizeof_char		sizeof(char)
#define tgt_sizeof_short	sizeof(short)
#define tgt_sizeof_int		sizeof(int)
#define tgt_sizeof_long		sizeof(long)
#define tgt_sizeof_char_p	sizeof(char *)
#define tgt_sizeof_void_p	sizeof(void *)
#define tgt_long		long
#define tgt_long_fmt		"l"

#if defined(COMMON_3264) && defined(ONLY_64)
#undef tgt_long
#undef tgt_long_fmt
#undef tgt_sizeof_long
#undef tgt_sizeof_char_p
#undef tgt_sizeof_void_p
#define tgt_long                long long
#define tgt_long_fmt		"ll"
#define tgt_sizeof_long         8
#define tgt_sizeof_char_p       8
#define tgt_sizeof_void_p       8
#endif


struct module_symbol
{
  unsigned long value;
  unsigned long name;
};

struct module_ref
{
  unsigned tgt_long dep;		
  unsigned tgt_long ref;
  unsigned tgt_long next_ref;
};

struct module
{
  unsigned tgt_long size_of_struct;	
  unsigned tgt_long next;
  unsigned tgt_long name;
  unsigned tgt_long size;

  tgt_long usecount;
  unsigned tgt_long flags;		

  unsigned nsyms;
  unsigned ndeps;

  unsigned tgt_long syms;
  unsigned tgt_long deps;
  unsigned tgt_long refs;
  unsigned tgt_long init;
  unsigned tgt_long cleanup;
  unsigned tgt_long ex_table_start;
  unsigned tgt_long ex_table_end;
#ifdef __alpha__
  unsigned tgt_long gp;
#endif
  
  unsigned tgt_long read_start;		
  unsigned tgt_long read_end;
  unsigned tgt_long can_unload;
  unsigned tgt_long runsize;
  unsigned tgt_long kallsyms_start;
  unsigned tgt_long kallsyms_end;
  unsigned tgt_long archdata_start;
  unsigned tgt_long archdata_end;
  unsigned tgt_long kernel_data;
};

struct module_info
{
  unsigned long addr;
  unsigned long size;
  unsigned long flags;
	   long usecount;
};

#define NEW_MOD_RUNNING		1
#define NEW_MOD_DELETED		2
#define NEW_MOD_AUTOCLEAN	4
#define NEW_MOD_VISITED		8
#define NEW_MOD_USED_ONCE	16
#define NEW_MOD_INITIALIZING	64

int sys_init_module(char const * name, const struct module *);
int query_module(char const * name, int which, void *buf, size_t bufsize,
		 size_t *ret);


#define QM_MODULES	1
#define QM_DEPS		2
#define QM_REFS		3
#define QM_SYMBOLS	4
#define QM_INFO		5


unsigned long create_module(const char *, size_t);
int delete_module(const char *);

extern unsigned int safemode;

#endif 
