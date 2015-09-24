/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2000,2001,2002  Free Software Foundation, Inc.
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

#ifdef SUPPORT_SERIAL

#include <shared.h>
#include <serial.h>
#include <term.h>
#include <terminfo.h>

static char input_buf[8];
static int npending = 0;

static int serial_x;
static int serial_y;

static int keep_track = 1;



#ifndef GRUB_UTIL
struct divisor
{
  int speed;
  unsigned short div;
};

static unsigned short serial_hw_port = 0;

static struct divisor divisor_tab[] =
  {
    { 2400,   0x0030 },
    { 4800,   0x0018 },
    { 9600,   0x000C },
    { 19200,  0x0006 },
    { 38400,  0x0003 },
    { 57600,  0x0002 },
    { 115200, 0x0001 }
  };

static inline unsigned char
inb (unsigned short port)
{
  unsigned char value;

  asm volatile ("inb	%w1, %0" : "=a" (value) : "Nd" (port));
  asm volatile ("outb	%%al, $0x80" : : );
  
  return value;
}

static inline void
outb (unsigned short port, unsigned char value)
{
  asm volatile ("outb	%b0, %w1" : : "a" (value), "Nd" (port));
  asm volatile ("outb	%%al, $0x80" : : );
}

int
serial_hw_fetch (void)
{
  if (inb (serial_hw_port + UART_LSR) & UART_DATA_READY)
    return inb (serial_hw_port + UART_RX);

  return -1;
}

void
serial_hw_put (int c)
{
  int timeout = 100000;

  
  while ((inb (serial_hw_port + UART_LSR) & UART_EMPTY_TRANSMITTER) == 0)
    {
      if (--timeout == 0)
	
	return;
    }

  outb (serial_hw_port + UART_TX, c);
}

void
serial_hw_delay (void)
{
  outb (0x80, 0);
}

unsigned short
serial_hw_get_port (int unit)
{
  
  const unsigned short *addr = (const unsigned short *) 0x0400;
  
  return addr[unit];
}

int
serial_hw_init (unsigned short port, unsigned int speed,
		int word_len, int parity, int stop_bit_len)
{
  int i;
  unsigned short div = 0;
  unsigned char status = 0;
  
  
  outb (port + UART_IER, 0);

  
  outb (port + UART_LCR, UART_DLAB);
  
  
  for (i = 0; i < sizeof (divisor_tab) / sizeof (divisor_tab[0]); i++)
    if (divisor_tab[i].speed == speed)
      {
	div = divisor_tab[i].div;
	break;
      }
  
  if (div == 0)
    return 0;
  
  outb (port + UART_DLL, div & 0xFF);
  outb (port + UART_DLH, div >> 8);
  
  
  status |= parity | word_len | stop_bit_len;
  outb (port + UART_LCR, status);

  
  outb (port + UART_FCR, UART_ENABLE_FIFO);

  
  outb (port + UART_MCR, UART_ENABLE_MODEM);

  
  serial_hw_port = port;
  
  
  while (serial_checkkey () != -1)
    (void) serial_getkey ();

  
  for (i = 0; term_table[i].name; i++)
    if (grub_strcmp (term_table[i].name, "serial") == 0)
      {
	term_table[i].flags &= ~TERM_NEED_INIT;
	break;
      }

  
  
  return 1;
}
#endif 



static void
serial_translate_key_sequence (void)
{
  const struct
  {
    char key;
    char ascii;
  }
  three_code_table[] =
    {
      {'A', 16},
      {'B', 14},
      {'C', 6},
      {'D', 2},
      {'F', 5},
      {'H', 1},
      {'4', 4}
    };

  const struct
  {
    short key;
    char ascii;
  }
  four_code_table[] =
    {
      {('1' | ('~' << 8)), 1},
      {('3' | ('~' << 8)), 4},
      {('5' | ('~' << 8)), 7},
      {('6' | ('~' << 8)), 3},
    };
  
  
  if (*((unsigned short *) input_buf) != ('\e' | ('[' << 8)))
    return;
  
  if (npending >= 3)
    {
      int i;

      for (i = 0;
	   i < sizeof (three_code_table) / sizeof (three_code_table[0]);
	   i++)
	if (three_code_table[i].key == input_buf[2])
	  {
	    input_buf[0] = three_code_table[i].ascii;
	    npending -= 2;
	    grub_memmove (input_buf + 1, input_buf + 3, npending - 1);
	    return;
	  }
    }

  if (npending >= 4)
    {
      int i;
      short key = *((short *) (input_buf + 2));

      for (i = 0;
	   i < sizeof (four_code_table) / sizeof (four_code_table[0]);
	   i++)
	if (four_code_table[i].key == key)
	  {
	    input_buf[0] = four_code_table[i].ascii;
	    npending -= 3;
	    grub_memmove (input_buf + 1, input_buf + 4, npending - 1);
	    return;
	  }
    }
}
    
static
int fill_input_buf (int nowait)
{
  int i;

  for (i = 0; i < 10000 && npending < sizeof (input_buf); i++)
    {
      int c;

      c = serial_hw_fetch ();
      if (c >= 0)
	{
	  input_buf[npending++] = c;

	  
	  i = 0;
	}
      
      if (nowait)
	break;
    }

  
  serial_translate_key_sequence ();
	  
  return npending;
}

int
serial_getkey (void)
{
  int c;
  
  while (! fill_input_buf (0))
    ;

  c = input_buf[0];
  npending--;
  grub_memmove (input_buf, input_buf + 1, npending);
  
  return c;
}

int
serial_checkkey (void)
{
  if (fill_input_buf (1))
    return input_buf[0];

  return -1;
}

void
serial_putchar (int c)
{
  
  if (keep_track)
    {
      
      switch (c)
	{
	case DISP_UL:
	  c = ACS_ULCORNER;
	  break;
	case DISP_UR:
	  c = ACS_URCORNER;
	  break;
	case DISP_LL:
	  c = ACS_LLCORNER;
	  break;
	case DISP_LR:
	  c = ACS_LRCORNER;
	  break;
	case DISP_HORIZ:
	  c = ACS_HLINE;
	  break;
	case DISP_VERT:
	  c = ACS_VLINE;
	  break;
	case DISP_LEFT:
	  c = ACS_LARROW;
	  break;
	case DISP_RIGHT:
	  c = ACS_RARROW;
	  break;
	case DISP_UP:
	  c = ACS_UARROW;
	  break;
	case DISP_DOWN:
	  c = ACS_DARROW;
	  break;
	default:
	  break;
	}
      
      switch (c)
	{
	case '\r':
	  serial_x = 0;
	  break;
	  
	case '\n':
	  serial_y++;
	  break;
	  
	case '\b':
	case 127:
	  if (serial_x > 0)
	    serial_x--;
	  break;
	  
	case '\a':
	  break;
	  
	default:
	  if (serial_x >= 79)
	    {
	      serial_putchar ('\r');
	      serial_putchar ('\n');
	    }
	  serial_x++;
	  break;
	}
    }
  
  serial_hw_put (c);
}

int
serial_getxy (void)
{
  return (serial_x << 8) | serial_y;
}

void
serial_gotoxy (int x, int y)
{
  keep_track = 0;
  ti_cursor_address (x, y);
  keep_track = 1;
  
  serial_x = x;
  serial_y = y;
}

void
serial_cls (void)
{
  keep_track = 0;
  ti_clear_screen ();
  keep_track = 1;
  
  serial_x = serial_y = 0;
}

void
serial_setcolorstate (color_state state)
{
  keep_track = 0;
  if (state == COLOR_STATE_HIGHLIGHT)
    ti_enter_standout_mode ();
  else
    ti_exit_standout_mode ();
  keep_track = 1;
}

#endif 
