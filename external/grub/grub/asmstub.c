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

#define _LARGEFILE_SOURCE	1
#define _FILE_OFFSET_BITS	64

int grub_stage2 (void);

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

#ifdef __linux__
# include <sys/ioctl.h>		
# if !defined(__GLIBC__) || \
	((__GLIBC__ < 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 1)))
#  include <linux/unistd.h>	
# endif 
# ifndef BLKFLSBUF
#  define BLKFLSBUF	_IO (0x12,97)	
# endif 
#endif 

#define WITHOUT_LIBC_STUBS 1
#include <shared.h>
#include <device.h>
#include <serial.h>
#include <term.h>

#define EXTENDED_MEMSIZE (3 * 1024 * 1024)	
#define CONVENTIONAL_MEMSIZE (640 * 1024)	

unsigned long install_partition = 0x20000;
unsigned long boot_drive = 0;
int saved_entryno = 0;
char version_string[] = VERSION;
char config_file[128] = "/boot/grub/menu.lst"; 
unsigned long linux_text_len = 0;
char *linux_data_tmp_addr = 0;
char *linux_data_real_addr = 0;
unsigned short io_map[IO_MAP_SIZE];
struct apm_info apm_bios_info;

char *grub_scratch_mem = 0;

struct geometry *disks = 0;

char **device_map = 0;

static jmp_buf env_for_exit;

int console_current_color = A_NORMAL;

static int serial_fd = -1;

static char *serial_device = 0;

#ifdef SIMULATE_SLOWNESS_OF_SERIAL
static unsigned int serial_speed;
#endif 

int
grub_stage2 (void)
{
  
  static int status = 0;
  static char *realstack;
  char *scratch, *simstack;
  int i;

  auto void doit (void);
  
  void doit (void)
    {
      
      asm volatile ("movl %%esp, %0\n\tmovl %1, %%esp\n"
		    : "=&r" (realstack) : "r" (simstack));
      
      
      if (! setjmp (env_for_exit))
	{
	  
	  status = 0;
	  init_bios_info ();
	}
      else
	{
	  
	  if (errnum)
	    status = 1;
	}
      
      
      asm volatile ("movl %0, %%esp\n" : : "r" (realstack));
    }

  assert (grub_scratch_mem == 0);
  scratch = malloc (0x100000 + EXTENDED_MEMSIZE + 15);
  assert (scratch);
  grub_scratch_mem = (char *) ((((int) scratch) >> 4) << 4);

  

  assert (disks == 0);
  disks = malloc (NUM_DISKS * sizeof (*disks));
  assert (disks);
  
  for (i = 0; i < NUM_DISKS; i++)
    disks[i].flags = -1;

  if (! init_device_map (&device_map, device_map_file, floppy_disks))
    return 1;
  
  
  assert ((SCRATCHSEG << 4) == SCRATCHADDR);
  assert ((BUFFERSEG << 4) == BUFFERADDR);
  assert (BUFFERADDR + BUFFERLEN == SCRATCHADDR);
  assert (FSYS_BUF % 16 == 0);
  assert (FSYS_BUF + FSYS_BUFLEN == BUFFERADDR);

#ifdef HAVE_LIBCURSES
  
  if (use_curses)
    {
      initscr ();
      cbreak ();
      noecho ();
      nonl ();
      scrollok (stdscr, TRUE);
      keypad (stdscr, TRUE);
      wtimeout (stdscr, 100);
      signal (SIGWINCH, SIG_IGN);
    }
#endif

  
  sync ();

  
  simstack = (char *) PROTSTACKINIT;
  doit ();

  
  sync ();

#ifdef HAVE_LIBCURSES
  if (use_curses)
    endwin ();
#endif

  
  for (i = 0; i < NUM_DISKS; i ++)
    if (disks[i].flags != -1)
      {
#ifdef __linux__
	ioctl (disks[i].flags, BLKFLSBUF, 0);
#else
# warning "In your operating system, the buffer cache will not be flushed."
#endif
	close (disks[i].flags);
      }

  if (serial_fd >= 0)
    close (serial_fd);
  
  
  restore_device_map (device_map);
  device_map = 0;
  free (disks);
  disks = 0;
  free (scratch);
  grub_scratch_mem = 0;

  if (serial_device)
    free (serial_device);
  serial_device = 0;
  
  
  return status;
}

void
assign_device_name (int drive, const char *device)
{
  
  if (device_map[drive])
    free (device_map[drive]);

  
  if (disks[drive].flags != -1)
    {
      close (disks[drive].flags);
      disks[drive].flags = -1;
    }

  
  if (! device)
    device_map[drive] = 0;
  else
    device_map[drive] = strdup (device);
}

void
stop (void)
{
#ifdef HAVE_LIBCURSES
  if (use_curses)
    endwin ();
#endif

  
  longjmp (env_for_exit, 1);
}

void
grub_reboot (void)
{
  stop ();
}

void
grub_halt (int no_apm)
{
  stop ();
}

void
chain_stage1 (unsigned long segment, unsigned long offset,
	      unsigned long part_table_addr)
{
  stop ();
}


void
chain_stage2 (unsigned long segment, unsigned long offset, int second_sector)
{
  stop ();
}


void
linux_boot (void)
{
  stop ();
}


void
big_linux_boot (void)
{
  stop ();
}


void
multi_boot (int start, int mb_info)
{
  stop ();
}

void
gateA20 (int linear)
{
  
}

void
set_int15_handler (void)
{
  
}

void
unset_int15_handler (void)
{
  
}

unsigned short bios_key_map[KEY_MAP_SIZE + 1];
unsigned short ascii_key_map[KEY_MAP_SIZE + 1];

void
set_int13_handler (unsigned short *map)
{
  
}

int
get_code_end (void)
{
  
  return BOOTSEC_LOCATION + (60 * 1024);
}


int
get_memsize (int type)
{
  if (! type)
    return CONVENTIONAL_MEMSIZE >> 10;
  else
    return EXTENDED_MEMSIZE >> 10;
}


int
get_eisamemsize (void)
{
  return (EXTENDED_MEMSIZE >> 10);
}


#define MMAR_DESC_TYPE_AVAILABLE 1 
#define MMAR_DESC_TYPE_RESERVED 2 
#define MMAR_DESC_TYPE_ACPI_RECLAIM 3 
#define MMAR_DESC_TYPE_ACPI_NVS 4 

#define MMAR_DESC_LENGTH	20

int
get_mmap_entry (struct mmar_desc *desc, int cont)
{
  
  static struct mmar_desc desc_table[] =
  {
    
    {
      MMAR_DESC_LENGTH,
      0,
      CONVENTIONAL_MEMSIZE,
      MMAR_DESC_TYPE_AVAILABLE
    },
    
    {
      MMAR_DESC_LENGTH,
      CONVENTIONAL_MEMSIZE,
      0x100000 - CONVENTIONAL_MEMSIZE,
      MMAR_DESC_TYPE_RESERVED
    },
    
    {
      MMAR_DESC_LENGTH,
      0x100000,
      EXTENDED_MEMSIZE,
      MMAR_DESC_TYPE_AVAILABLE
    }
  };
  
  int num = sizeof (desc_table) / sizeof (*desc_table);

  if (cont < 0 || cont >= num)
    {
      
      desc->desc_len = 0;
    }
  else
    {
      
      *desc = desc_table[cont++];

      
      if (cont < num)
	return cont;
    }
  
  return 0;
}

void
track_int13 (int drive)
{
  
}

unsigned long
get_rom_config_table (void)
{
  return 0;
}

void
get_apm_info (void)
{
  
}

int
get_vbe_controller_info (struct vbe_controller *controller)
{
  
  return 0;
}

int
get_vbe_mode_info (int mode_number, struct vbe_mode *mode)
{
  
  return 0;
}

int
set_vbe_mode (int mode_number)
{
  
  return 0;
}

int
getrtsecs (void)
{
  
  return time (0);
}

int
currticks (void)
{
  struct timeval tv;
  long csecs;
  int ticks_per_csec, ticks_per_usec;

  

  
  gettimeofday (&tv, 0);

  
  csecs = tv.tv_sec / 10;

  
  ticks_per_csec = csecs * 182;

  
  ticks_per_usec = (((tv.tv_sec - csecs * 10) * 1000000 + tv.tv_usec)
		    * 182 / 10000000);

  
  return ticks_per_csec + ticks_per_usec;
}

void
console_putchar (int c)
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

#ifdef HAVE_LIBCURSES
  if (use_curses)
    {
      if (c == '\n')
	{
	  int x, y;

	  getyx (stdscr, y, x);
	  if (y + 1 == LINES)
	    scroll (stdscr);
	  else
	    move (y + 1, x);
	}
      else if (isprint (c))
	{
	  int x, y;

	  getyx (stdscr, y, x);
	  if (x + 1 == COLS)
	    {
	      console_putchar ('\r');
	      console_putchar ('\n');
	    }
	  addch (c | console_current_color);
	}
      else
	{
	  addch (c);
	}
      
#ifdef REFRESH_IMMEDIATELY
      refresh ();
#endif
    }
  else
#endif
    {
      
      if (c != '\r')
	putchar (c);
    }
}

#ifdef HAVE_LIBCURSES
static int save_char = ERR;
#endif

static int
console_translate_key (int c)
{
  switch (c)
    {
    case KEY_LEFT:
      return 2;
    case KEY_RIGHT:
      return 6;
    case KEY_UP:
      return 16;
    case KEY_DOWN:
      return 14;
    case KEY_DC:
      return 4;
    case KEY_BACKSPACE:
      return 8;
    case KEY_HOME:
      return 1;
    case KEY_END:
      return 5;
    case KEY_PPAGE:
      return 7;
    case KEY_NPAGE:
      return 3;
    default:
      break;
    }

  return c;
}

int
console_checkkey (void)
{
#ifdef HAVE_LIBCURSES
  if (use_curses)
    {
      int c;

      if (save_char != ERR)
	return save_char;

      c = getch ();
      
      if (c != ERR)
	save_char = c;
      return console_translate_key (c);
    }
#endif

  return ' ';
}

int
console_getkey (void)
{
  int c;

#ifdef HAVE_LIBCURSES
  if (use_curses)
    {
      
      if (save_char != ERR)
	{
	  c = save_char;
	  save_char = ERR;
	  return console_translate_key (c);
	}

      wtimeout (stdscr, -1);
      c = getch ();
      wtimeout (stdscr, 100);
    }
  else
#endif
    c = getchar ();

  
  if (c == -1)
    stop ();
  
  return console_translate_key (c);
}

int
console_getxy (void)
{
  int y, x;
#ifdef HAVE_LIBCURSES
  if (use_curses)
    getyx (stdscr, y, x);
  else
#endif
  y = x = 0;
  return (x << 8) | (y & 0xff);
}

void
console_gotoxy (int x, int y)
{
#ifdef HAVE_LIBCURSES
  if (use_curses)
    move (y, x);
#endif
}

void
console_cls (void)
{
#ifdef HAVE_LIBCURSES
  if (use_curses)
    clear ();
#endif
}

void
console_setcolorstate (color_state state)
{
  console_current_color = 
    (state == COLOR_STATE_HIGHLIGHT) ? A_REVERSE : A_NORMAL;
}

void
console_setcolor (int normal_color, int highlight_color)
{
  
}

int
console_setcursor (int on)
{
  return 1;
}

int
get_diskinfo (int drive, struct geometry *geometry)
{

  
  if (disks[drive].flags == -1)
    {
      
      char *devname = device_map[drive];
      char buf[512];

      if (! devname)
	return -1;

      if (verbose)
	grub_printf ("Attempt to open drive 0x%x (%s)\n",
		     drive, devname);

      
      if (! read_only)
	disks[drive].flags = open (devname, O_RDWR);

      if (disks[drive].flags == -1)
	{
	  if (read_only || errno == EACCES || errno == EROFS || errno == EPERM)
	    {
	      disks[drive].flags = open (devname, O_RDONLY);
	      if (disks[drive].flags == -1)
		{
		  assign_device_name (drive, 0);
		  return -1;
		}
	    }
	  else
	    {
	      assign_device_name (drive, 0);
	      return -1;
	    }
	}

      
      if (read (disks[drive].flags, buf, 512) != 512)
	{
	  close (disks[drive].flags);
	  disks[drive].flags = -1;
	  assign_device_name (drive, 0);
	  return -1;
	}

      if (disks[drive].flags != -1)
	get_drive_geometry (&disks[drive], device_map, drive);
    }

  if (disks[drive].flags == -1)
    return -1;

#ifdef __linux__
  ioctl (disks[drive].flags, BLKFLSBUF, 0);
#endif

  *geometry = disks[drive];
  return 0;
}

static int
nread (int fd, char *buf, size_t len)
{
  int size = len;

  while (len)
    {
      int ret = read (fd, buf, len);

      if (ret <= 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    return ret;
	}

      len -= ret;
      buf += ret;
    }

  return size;
}

static int
nwrite (int fd, char *buf, size_t len)
{
  int size = len;

  while (len)
    {
      int ret = write (fd, buf, len);

      if (ret <= 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    return ret;
	}

      len -= ret;
      buf += ret;
    }

  return size;
}

static void
hex_dump (void *buf, size_t size)
{
  
#define MAX_COLUMN	70

  
  unsigned char *ptr = buf;
  
  int column = 0;
  /* how many bytes written */
  int count = 0;

  while (size > 0)
    {
      
      int hi = *ptr >> 4;
      
      int low = *ptr & 0xf;

      grub_printf ("%x%x", hi, low);
      column += 2;
      count++;
      ptr++;
      size--;

      
      if (size != 0 && (count % 4) == 0)
	{
	  if (column < MAX_COLUMN)
	    {
	      grub_printf (" ");
	      column++;
	    }
	  else
	    {
	      grub_printf ("\n");
	      column = 0;
	    }
	}
    }

  
  grub_printf ("\n");
}

int
biosdisk (int subfunc, int drive, struct geometry *geometry,
	  int sector, int nsec, int segment)
{
  char *buf;
  int fd = geometry->flags;

  
  if (fd == -1 || fd != disks[drive].flags)
    return BIOSDISK_ERROR_GEOMETRY;

  
#if defined(__linux__) && (!defined(__GLIBC__) || \
	((__GLIBC__ < 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ < 1))))
  
  {
    loff_t offset, result;
    static int _llseek (uint filedes, ulong hi, ulong lo,
			loff_t *res, uint wh);
    _syscall5 (int, _llseek, uint, filedes, ulong, hi, ulong, lo,
	       loff_t *, res, uint, wh);

    offset = (loff_t) sector * (loff_t) SECTOR_SIZE;
    if (_llseek (fd, offset >> 32, offset & 0xffffffff, &result, SEEK_SET))
      return -1;
  }
#else
  {
    off_t offset = (off_t) sector * (off_t) SECTOR_SIZE;

    if (lseek (fd, offset, SEEK_SET) != offset)
      return -1;
  }
#endif

  buf = (char *) (segment << 4);

  switch (subfunc)
    {
    case BIOSDISK_READ:
#ifdef __linux__
      if (sector == 0 && nsec > 1)
	{
	  if (nread (fd, buf, SECTOR_SIZE) != SECTOR_SIZE)
	    return -1;
	  buf += SECTOR_SIZE;
	  nsec--;
	}
#endif
      if (nread (fd, buf, nsec * SECTOR_SIZE) != nsec * SECTOR_SIZE)
	return -1;
      break;

    case BIOSDISK_WRITE:
      if (verbose)
	{
	  grub_printf ("Write %d sectors starting from %d sector"
		       " to drive 0x%x (%s)\n",
		       nsec, sector, drive, device_map[drive]);
	  hex_dump (buf, nsec * SECTOR_SIZE);
	}
      if (! read_only)
	if (nwrite (fd, buf, nsec * SECTOR_SIZE) != nsec * SECTOR_SIZE)
	  return -1;
      break;

    default:
      grub_printf ("unknown subfunc %d\n", subfunc);
      break;
    }

  return 0;
}


void
stop_floppy (void)
{
  
}

int
serial_hw_fetch (void)
{
  fd_set fds;
  struct timeval to;
  char c;

  
  FD_ZERO (&fds);
  FD_SET (serial_fd, &fds);

  to.tv_sec = 0;
  to.tv_usec = 0;
  
  if (select (serial_fd + 1, &fds, 0, 0, &to) > 0)
    {
      if (nread (serial_fd, &c, 1) != 1)
	stop ();

      return c;
    }
  
  return -1;
}

void
serial_hw_put (int c)
{
  char ch = (char) c;
  
  if (nwrite (serial_fd, &ch, 1) != 1)
    stop ();
}

void
serial_hw_delay (void)
{
#ifdef SIMULATE_SLOWNESS_OF_SERIAL
  struct timeval otv, tv;

  gettimeofday (&otv, 0);

  while (1)
    {
      long delta;
      
      gettimeofday (&tv, 0);
      delta = tv.tv_usec - otv.tv_usec;
      if (delta < 0)
	delta += 1000000;
      
      if (delta >= 1000000 / (serial_speed >> 3))
	break;
    }
#endif 
}

static speed_t
get_termios_speed (int speed)
{
  switch (speed)
    {
    case 2400: return B2400;
    case 4800: return B4800;
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
#ifdef B57600
    case 57600: return B57600;
#endif
#ifdef B115200      
    case 115200: return B115200;
#endif
    }

  return B0;
}

unsigned short
serial_hw_get_port (int unit)
{
  return 0;
}

int
serial_hw_init (unsigned short port, unsigned int speed,
		int word_len, int parity, int stop_bit_len)
{
  struct termios termios;
  speed_t termios_speed;
  int i;
  
  
  if (! serial_device)
    return 0;

  
  if (serial_fd >= 0)
    close (serial_fd);
  
  
  serial_fd = open (serial_device,
		    O_RDWR | O_NOCTTY
#if defined(O_SYNC)
		    
		    | O_SYNC
#elif defined(O_FSYNC)
		    
		    | O_FSYNC
#endif
		    );
  if (serial_fd < 0)
    return 0;

  
  if (tcgetattr (serial_fd, &termios))
    goto fail;

  
  cfmakeraw (&termios);

  
  termios_speed = get_termios_speed (speed);
  if (termios_speed == B0)
    goto fail;
  
  cfsetispeed (&termios, termios_speed);
  cfsetospeed (&termios, termios_speed);

  
  termios.c_cflag &= ~CSIZE;
  switch (word_len)
    {
    case UART_5BITS_WORD:
      termios.c_cflag |= CS5;
      break;
    case UART_6BITS_WORD:
      termios.c_cflag |= CS6;
      break;
    case UART_7BITS_WORD:
      termios.c_cflag |= CS7;
      break;
    case UART_8BITS_WORD:
      termios.c_cflag |= CS8;
      break;
    default:
      goto fail;
    }

  
  switch (parity)
    {
    case UART_NO_PARITY:
      termios.c_cflag &= ~PARENB;
      break;
    case UART_ODD_PARITY:
      termios.c_cflag |= PARENB;
      termios.c_cflag |= PARODD;
      break;
    case UART_EVEN_PARITY:
      termios.c_cflag |= PARENB;
      termios.c_cflag &= ~PARODD;
      break;
    default:
      goto fail;
    }

  
  switch (stop_bit_len)
    {
    case UART_1_STOP_BIT:
      termios.c_cflag &= ~CSTOPB;
      break;
    case UART_2_STOP_BITS:
      termios.c_cflag |= CSTOPB;
      break;
    default:
      goto fail;
    }

  
  if (tcsetattr (serial_fd, TCSANOW, &termios))
    goto fail;

#ifdef SIMULATE_SLOWNESS_OF_SERIAL
  serial_speed = speed;
#endif 

  
  for (i = 0; term_table[i].name; i++)
    {
      if (strcmp (term_table[i].name, "serial") == 0)
	{
	  term_table[i].flags &= ~(TERM_NEED_INIT);
	  break;
	}
    }
  
  return 1;

 fail:
  close (serial_fd);
  serial_fd = -1;
  return 0;
}

void
serial_set_device (const char *device)
{
  if (serial_device)
    free (serial_device);
  
  serial_device = strdup (device);
}

void
hercules_putchar (int c)
{
  console_putchar (c);
}

int
hercules_getxy (void)
{
  return console_getxy ();
}

void
hercules_gotoxy (int x, int y)
{
  console_gotoxy (x, y);
}

void
hercules_cls (void)
{
  console_cls ();
}

void
hercules_setcolorstate (color_state state)
{
  console_setcolorstate (state);
}

void
hercules_setcolor (int normal_color, int highlight_color)
{
  console_setcolor (normal_color, highlight_color);
}

int
hercules_setcursor (int on)
{
  return 1;
}
