/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002  Free Software Foundation, Inc.
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

#ifndef GRUB_TERM_HEADER
#define GRUB_TERM_HEADER	1

typedef enum
{
  COLOR_STATE_STANDARD,
  
  COLOR_STATE_NORMAL,
  
  COLOR_STATE_HIGHLIGHT
} color_state;

#ifndef STAGE1_5


#define TERM_NO_ECHO		(1 << 0)
#define TERM_NO_EDIT		(1 << 1)
#define TERM_DUMB		(1 << 2)
#define TERM_NEED_INIT		(1 << 16)

struct term_entry
{
  
  const char *name;
  
  unsigned long flags;
  
  void (*putchar) (int c);
  
  int (*checkkey) (void);
  
  int (*getkey) (void);
  
  int (*getxy) (void);
  
  void (*gotoxy) (int x, int y);
  
  void (*cls) (void);
  
  void (*setcolorstate) (color_state state);
  void (*setcolor) (int normal_color, int highlight_color);
  
  int (*setcursor) (int on);
};

extern struct term_entry term_table[];
extern struct term_entry *current_term;

#endif 

extern int console_current_color;
void console_putchar (int c);

#ifndef STAGE1_5
int console_checkkey (void);
int console_getkey (void);
int console_getxy (void);
void console_gotoxy (int x, int y);
void console_cls (void);
void console_setcolorstate (color_state state);
void console_setcolor (int normal_color, int highlight_color);
int console_setcursor (int on);
#endif

#ifdef SUPPORT_SERIAL
void serial_putchar (int c);
int serial_checkkey (void);
int serial_getkey (void);
int serial_getxy (void);
void serial_gotoxy (int x, int y);
void serial_cls (void);
void serial_setcolorstate (color_state state);
#endif

#ifdef SUPPORT_HERCULES
void hercules_putchar (int c);
int hercules_getxy (void);
void hercules_gotoxy (int x, int y);
void hercules_cls (void);
void hercules_setcolorstate (color_state state);
void hercules_setcolor (int normal_color, int highlight_color);
int hercules_setcursor (int on);
#endif

#endif 
