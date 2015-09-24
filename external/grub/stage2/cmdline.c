/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2004  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <shared.h>

#ifdef SUPPORT_DISKLESS
# define GRUB	1
# include <etherboot.h>
#endif

grub_jmp_buf restart_cmdline_env;

char *
skip_to (int after_equal, char *cmdline)
{
  
  while (*cmdline && *cmdline != ' ' && *cmdline != '\t' &&
	 ! (after_equal && *cmdline == '='))
    cmdline ++;

  
  while (*cmdline == ' ' || *cmdline == '\t' ||
	 (after_equal && *cmdline == '='))
    cmdline ++;

  return cmdline;
}

void
print_cmdline_message (int forever)
{
  printf (" [ Minimal BASH-like line editing is supported.  For the first word, TAB\n"
	  "   lists possible command completions.  Anywhere else TAB lists the possible\n"
	  "   completions of a device/filename.%s ]\n",
	  (forever ? "" : "  ESC at any time exits."));
}

struct builtin *
find_command (char *command)
{
  char *ptr;
  char c;
  struct builtin **builtin;

  
  ptr = command;
  while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '=')
    ptr ++;

  c = *ptr;
  *ptr = 0;

  
  for (builtin = builtin_table; *builtin != 0; builtin++)
    {
      int ret = grub_strcmp (command, (*builtin)->name);

      if (ret == 0)
	{
	  
	  *ptr = c;
	  return *builtin;
	}
      else if (ret < 0)
	break;
    }

  
  errnum = ERR_UNRECOGNIZED;
  *ptr = c;
  return 0;
}

static void
init_cmdline (void)
{
  
  saved_drive = boot_drive;
  saved_partition = install_partition;
  current_drive = GRUB_INVALID_DRIVE;
  errnum = 0;
  count_lines = -1;
  
  
  mbi.mem_upper = saved_mem_upper;
  if (mbi.mmap_length)
    mbi.flags |= MB_INFO_MEM_MAP;

  
  init_builtins ();
}

void
enter_cmdline (char *heap, int forever)
{
  
  init_cmdline ();
  grub_setjmp (restart_cmdline_env);
  init_page ();
#ifdef SUPPORT_DISKLESS
  print_network_configuration ();
  grub_putchar ('\n');
#endif
  print_cmdline_message (forever);
  
  while (1)
    {
      struct builtin *builtin;
      char *arg;

      *heap = 0;
      print_error ();
      errnum = ERR_NONE;

      
      if (get_cmdline (PACKAGE "> ", heap, 2048, 0, 1))
	return;

      
      if (! heap[0])
	continue;

      
      builtin = find_command (heap);
      if (! builtin)
	continue;

      
      if (! (builtin->flags & BUILTIN_CMDLINE))
	{
	  errnum = ERR_UNRECOGNIZED;
	  continue;
	}

      buf_drive = -1;

      
      if (use_pager)
	count_lines = 0;
      
      
      arg = skip_to (1, heap);
      (builtin->func) (arg, BUILTIN_CMDLINE);

      
      count_lines = -1;
    }
}

int
run_script (char *script, char *heap)
{
  char *old_entry;
  char *cur_entry = script;

  
  init_cmdline ();

  while (1)
    {
      struct builtin *builtin;
      char *arg;

      print_error ();

      if (errnum)
	{
	  errnum = ERR_NONE;

	  if (fallback_entryno < 0)
	    {
	      grub_printf ("\nPress any key to continue...");
	      (void) getkey ();
	    }
	  
	  return 1;
	}

      
      old_entry = cur_entry;
      while (*cur_entry++)
	;

      grub_memmove (heap, old_entry, (int) cur_entry - (int) old_entry);
      if (! *heap)
	{
	  

	  
	  if (kernel_type == KERNEL_TYPE_NONE)
	    return 0;

	  
	  grub_memmove (heap, "boot", 5);
	}

      
      builtin = find_command (heap);
      if (! builtin)
	{
	  grub_printf ("%s\n", old_entry);
	  continue;
	}

      if (! (builtin->flags & BUILTIN_NO_ECHO))
	grub_printf ("%s\n", old_entry);

      
      if (! (builtin->flags & BUILTIN_CMDLINE))
	{
	  errnum = ERR_UNRECOGNIZED;
	  continue;
	}

      buf_drive = -1;

      
      arg = skip_to (1, heap);
      (builtin->func) (arg, BUILTIN_SCRIPT);
    }
}
