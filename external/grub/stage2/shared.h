/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2003,2004  Free Software Foundation, Inc.
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


#ifndef GRUB_SHARED_HEADER
#define GRUB_SHARED_HEADER	1

#include <config.h>

#ifdef HAVE_ASM_USCORE
# define EXT_C(sym) _ ## sym
#else
# define EXT_C(sym) sym
#endif

#ifdef GRUB_UTIL
extern char *grub_scratch_mem;
# define RAW_ADDR(x) ((x) + (int) grub_scratch_mem)
# define RAW_SEG(x) (RAW_ADDR ((x) << 4) >> 4)
#else
# define RAW_ADDR(x) (x)
# define RAW_SEG(x) (x)
#endif


#define MAXINT     0x7FFFFFFF

#define MAX_CMDLINE 1600
#define NEW_HEAPSIZE 1500

#define SCRATCHADDR  RAW_ADDR (0x77e00)
#define SCRATCHSEG   RAW_SEG (0x77e0)


#define BUFFERLEN   0x7e00
#define BUFFERADDR  RAW_ADDR (0x70000)
#define BUFFERSEG   RAW_SEG (0x7000)

#define BOOT_PART_TABLE	RAW_ADDR (0x07be)

#define BIOSDISK_READ			0x0
#define BIOSDISK_WRITE			0x1
#define BIOSDISK_ERROR_GEOMETRY		0x100
#define BIOSDISK_FLAG_LBA_EXTENSION	0x1
#define BIOSDISK_FLAG_CDROM		0x2


#define FSYS_BUFLEN  0x8000
#define FSYS_BUF RAW_ADDR (0x68000)

#define MB_CMDLINE_BUF		RAW_ADDR (0x2000)
#define MB_CMDLINE_BUFLEN	0x6000

#define PASSWORD_BUF		RAW_ADDR (0x78000)
#define PASSWORD_BUFLEN		0x200

#define DEFAULT_FILE_BUF	(PASSWORD_BUF + PASSWORD_BUFLEN)
#define DEFAULT_FILE_BUFLEN	0x60

#define CMDLINE_BUF		(DEFAULT_FILE_BUF + DEFAULT_FILE_BUFLEN)
#define CMDLINE_BUFLEN		MAX_CMDLINE

#define KILL_BUF		(CMDLINE_BUF + CMDLINE_BUFLEN)
#define KILL_BUFLEN		MAX_CMDLINE

#define HISTORY_BUF		(KILL_BUF + KILL_BUFLEN)
#define HISTORY_SIZE		5
#define HISTORY_BUFLEN		(MAX_CMDLINE * HISTORY_SIZE)

#define COMPLETION_BUF		(HISTORY_BUF + HISTORY_BUFLEN)
#define COMPLETION_BUFLEN	MAX_CMDLINE

#define UNIQUE_BUF		(COMPLETION_BUF + COMPLETION_BUFLEN)
#define UNIQUE_BUFLEN		MAX_CMDLINE

#define MENU_BUF		(UNIQUE_BUF + UNIQUE_BUFLEN)
#define MENU_BUFLEN		(0x8000 + PASSWORD_BUF - MENU_BUF)

#define DRIVE_MAP_SIZE		8

#define KEY_MAP_SIZE		128

#define IO_MAP_SIZE		128


#define LINUX_MAGIC_SIGNATURE		0x53726448	
#define LINUX_DEFAULT_SETUP_SECTS	4
#define LINUX_FLAG_CAN_USE_HEAP		0x80
#define LINUX_INITRD_MAX_ADDRESS	0x38000000
#define LINUX_MAX_SETUP_SECTS		64
#define LINUX_BOOT_LOADER_TYPE		0x71
#define LINUX_HEAP_END_OFFSET		(0x9000 - 0x200)

#define LINUX_BZIMAGE_ADDR		RAW_ADDR (0x100000)
#define LINUX_ZIMAGE_ADDR		RAW_ADDR (0x10000)
#define LINUX_OLD_REAL_MODE_ADDR	RAW_ADDR (0x90000)
#define LINUX_SETUP_STACK		0x9000

#define LINUX_FLAG_BIG_KERNEL		0x1

#define LINUX_VID_MODE_NORMAL		0xFFFF
#define LINUX_VID_MODE_EXTENDED		0xFFFE
#define LINUX_VID_MODE_ASK		0xFFFD

#define LINUX_CL_OFFSET			0x9000
#define LINUX_CL_END_OFFSET		0x90FF
#define LINUX_SETUP_MOVE_SIZE		0x9100
#define LINUX_CL_MAGIC			0xA33F


#define SECTOR_SIZE		0x200
#define SECTOR_BITS		9
#define BIOS_FLAG_FIXED_DISK	0x80

#define BOOTSEC_LOCATION		RAW_ADDR (0x7C00)
#define BOOTSEC_SIGNATURE		0xAA55
#define BOOTSEC_BPB_OFFSET		0x3
#define BOOTSEC_BPB_LENGTH		0x3B
#define BOOTSEC_BPB_SYSTEM_ID		0x3
#define BOOTSEC_BPB_HIDDEN_SECTORS	0x1C
#define BOOTSEC_PART_OFFSET		0x1BE
#define BOOTSEC_PART_LENGTH		0x40
#define BOOTSEC_SIG_OFFSET		0x1FE
#define BOOTSEC_LISTSIZE		8

#define NETWORK_DRIVE	0x20


#include <stage1.h>

#define STAGE2_VER_MAJ_OFFS	0x6
#define STAGE2_INSTALLPART	0x8
#define STAGE2_SAVED_ENTRYNO	0xc
#define STAGE2_STAGE2_ID	0x10
#define STAGE2_FORCE_LBA	0x11
#define STAGE2_VER_STR_OFFS	0x12

#define STAGE2_ID_STAGE2		0
#define STAGE2_ID_FFS_STAGE1_5		1
#define STAGE2_ID_E2FS_STAGE1_5		2
#define STAGE2_ID_FAT_STAGE1_5		3
#define STAGE2_ID_MINIX_STAGE1_5	4
#define STAGE2_ID_REISERFS_STAGE1_5	5
#define STAGE2_ID_VSTAFS_STAGE1_5	6
#define STAGE2_ID_JFS_STAGE1_5		7
#define STAGE2_ID_XFS_STAGE1_5		8
#define STAGE2_ID_ISO9660_STAGE1_5	9
#define STAGE2_ID_UFS2_STAGE1_5		10

#ifndef STAGE1_5
# define STAGE2_ID	STAGE2_ID_STAGE2
#else
# if defined(FSYS_FFS)
#  define STAGE2_ID	STAGE2_ID_FFS_STAGE1_5
# elif defined(FSYS_EXT2FS)
#  define STAGE2_ID	STAGE2_ID_E2FS_STAGE1_5
# elif defined(FSYS_FAT)
#  define STAGE2_ID	STAGE2_ID_FAT_STAGE1_5
# elif defined(FSYS_MINIX)
#  define STAGE2_ID	STAGE2_ID_MINIX_STAGE1_5
# elif defined(FSYS_REISERFS)
#  define STAGE2_ID	STAGE2_ID_REISERFS_STAGE1_5
# elif defined(FSYS_VSTAFS)
#  define STAGE2_ID	STAGE2_ID_VSTAFS_STAGE1_5
# elif defined(FSYS_JFS)
#  define STAGE2_ID	STAGE2_ID_JFS_STAGE1_5
# elif defined(FSYS_XFS)
#  define STAGE2_ID	STAGE2_ID_XFS_STAGE1_5
# elif defined(FSYS_ISO9660)
#  define STAGE2_ID	STAGE2_ID_ISO9660_STAGE1_5
# elif defined(FSYS_UFS2)
#  define STAGE2_ID	STAGE2_ID_UFS2_STAGE1_5
# else
#  error "unknown Stage 2"
# endif
#endif


#define CR0_PE_ON	0x1
#define CR0_PE_OFF	0xfffffffe
#define PROT_MODE_CSEG	0x8
#define PROT_MODE_DSEG  0x10
#define PSEUDO_RM_CSEG	0x18
#define PSEUDO_RM_DSEG	0x20
#define STACKOFF	(0x2000 - 0x10)
#define PROTSTACKINIT   (FSYS_BUF - 0x10)



#define ENTRY(x) .globl EXT_C(x) ; EXT_C(x):
#define VARIABLE(x) ENTRY(x)


#define K_RDWR  	0x60	
#define K_STATUS	0x64	
#define K_CMD		0x64	

#define K_OBUF_FUL 	0x01	
#define K_IBUF_FUL 	0x02	

#define KC_CMD_WIN	0xd0	
#define KC_CMD_WOUT	0xd1	
#define KB_OUTPUT_MASK  0xdd	
#define KB_A20_ENABLE   0x02

#define ASCII_CHAR(x)   ((x) & 0xFF)
#if !defined(GRUB_UTIL) || !defined(HAVE_LIBCURSES)
# define KEY_LEFT        0x4B00
# define KEY_RIGHT       0x4D00
# define KEY_UP          0x4800
# define KEY_DOWN        0x5000
# define KEY_IC          0x5200	
# define KEY_DC          0x5300	
# define KEY_BACKSPACE   0x0008
# define KEY_HOME        0x4700
# define KEY_END         0x4F00
# define KEY_NPAGE       0x5100
# define KEY_PPAGE       0x4900
# define A_NORMAL        0x7
# define A_REVERSE       0x70
#elif defined(HAVE_NCURSES_CURSES_H)
# include <ncurses/curses.h>
#elif defined(HAVE_NCURSES_H)
# include <ncurses.h>
#elif defined(HAVE_CURSES_H)
# include <curses.h>
#endif

#ifndef A_NORMAL
# define A_NORMAL	0
#endif 
#ifndef A_REVERSE
# ifdef A_STANDOUT
#  define A_REVERSE	A_STANDOUT
# else 
#  define A_REVERSE	0
# endif 
#endif 

#undef ACS_ULCORNER
#undef ACS_URCORNER
#undef ACS_LLCORNER
#undef ACS_LRCORNER
#undef ACS_HLINE
#undef ACS_VLINE
#undef ACS_LARROW
#undef ACS_RARROW
#undef ACS_UARROW
#undef ACS_DARROW

#define ACS_ULCORNER	'+'
#define ACS_URCORNER	'+'
#define ACS_LLCORNER	'+'
#define ACS_LRCORNER	'+'
#define ACS_HLINE	'-'
#define ACS_VLINE	'|'
#define ACS_LARROW	'<'
#define ACS_RARROW	'>'
#define ACS_UARROW	'^'
#define ACS_DARROW	'v'

#define DISP_UL		218
#define DISP_UR		191
#define DISP_LL		192
#define DISP_LR		217
#define DISP_HORIZ	196
#define DISP_VERT	179
#define DISP_LEFT	0x1b
#define DISP_RIGHT	0x1a
#define DISP_UP		0x18
#define DISP_DOWN	0x19

#ifndef WITHOUT_LIBC_STUBS
#define memmove grub_memmove
#define memcpy grub_memmove	
#define memset grub_memset
#define isspace grub_isspace
#define printf grub_printf
#define sprintf grub_sprintf
#undef putchar
#define putchar grub_putchar
#define strncat grub_strncat
#define strstr grub_strstr
#define memcmp grub_memcmp
#define strcmp grub_strcmp
#define tolower grub_tolower
#define strlen grub_strlen
#define strcpy grub_strcpy
#endif 


#ifndef ASM_FILE


#include "mb_header.h"
#include "mb_info.h"

struct linux_kernel_header
{
  char code1[0x0020];
  unsigned short cl_magic;		
  unsigned short cl_offset;		
  char code2[0x01F1 - 0x0020 - 2 - 2];
  unsigned char setup_sects;		
  unsigned short root_flags;		
  unsigned short syssize;		
  unsigned short swap_dev;		
  unsigned short ram_size;		
  unsigned short vid_mode;		
  unsigned short root_dev;		
  unsigned short boot_flag;		
  unsigned short jump;			
  unsigned long header;			
  unsigned short version;		
  unsigned long realmode_swtch;		
  unsigned long start_sys;		
  unsigned char type_of_loader;		
  unsigned char loadflags;		
  unsigned short setup_move_size;	
  unsigned long code32_start;		
  unsigned long ramdisk_image;		
  unsigned long ramdisk_size;		
  unsigned long bootsect_kludge;	
  unsigned short heap_end_ptr;		
  unsigned short pad1;			
  char *cmd_line_ptr;			
  unsigned long initrd_addr_max;	
} __attribute__ ((packed));

struct mmar_desc
{
  unsigned long desc_len;	
  unsigned long long addr;	
  unsigned long long length;	
  unsigned long type;		
} __attribute__ ((packed));

struct vbe_controller
{
  unsigned char signature[4];
  unsigned short version;
  unsigned long oem_string;
  unsigned long capabilities;
  unsigned long video_mode;
  unsigned short total_memory;
  unsigned short oem_software_rev;
  unsigned long oem_vendor_name;
  unsigned long oem_product_name;
  unsigned long oem_product_rev;
  unsigned char reserved[222];
  unsigned char oem_data[256];
} __attribute__ ((packed));

struct vbe_mode
{
  unsigned short mode_attributes;
  unsigned char win_a_attributes;
  unsigned char win_b_attributes;
  unsigned short win_granularity;
  unsigned short win_size;
  unsigned short win_a_segment;
  unsigned short win_b_segment;
  unsigned long win_func;
  unsigned short bytes_per_scanline;

  
  unsigned short x_resolution;
  unsigned short y_resolution;
  unsigned char x_char_size;
  unsigned char y_char_size;
  unsigned char number_of_planes;
  unsigned char bits_per_pixel;
  unsigned char number_of_banks;
  unsigned char memory_model;
  unsigned char bank_size;
  unsigned char number_of_image_pages;
  unsigned char reserved0;

  
  unsigned char red_mask_size;
  unsigned char red_field_position;
  unsigned char green_mask_size;
  unsigned char green_field_position;
  unsigned char blue_mask_size;
  unsigned char blue_field_position;
  unsigned char reserved_mask_size;
  unsigned char reserved_field_position;
  unsigned char direct_color_mode_info;

  
  unsigned long phys_base;
  unsigned long reserved1;
  unsigned short reversed2;

  
  unsigned short linear_bytes_per_scanline;
  unsigned char banked_number_of_image_pages;
  unsigned char linear_number_of_image_pages;
  unsigned char linear_red_mask_size;
  unsigned char linear_red_field_position;
  unsigned char linear_green_mask_size;
  unsigned char linear_green_field_position;
  unsigned char linear_blue_mask_size;
  unsigned char linear_blue_field_position;
  unsigned char linear_reserved_mask_size;
  unsigned char linear_reserved_field_position;
  unsigned long max_pixel_clock;

  unsigned char reserved3[189];
} __attribute__ ((packed));


#undef NULL
#define NULL         ((void *) 0)

typedef enum
{
  ERR_NONE = 0,
  ERR_BAD_FILENAME,
  ERR_BAD_FILETYPE,
  ERR_BAD_GZIP_DATA,
  ERR_BAD_GZIP_HEADER,
  ERR_BAD_PART_TABLE,
  ERR_BAD_VERSION,
  ERR_BELOW_1MB,
  ERR_BOOT_COMMAND,
  ERR_BOOT_FAILURE,
  ERR_BOOT_FEATURES,
  ERR_DEV_FORMAT,
  ERR_DEV_VALUES,
  ERR_EXEC_FORMAT,
  ERR_FILELENGTH,
  ERR_FILE_NOT_FOUND,
  ERR_FSYS_CORRUPT,
  ERR_FSYS_MOUNT,
  ERR_GEOM,
  ERR_NEED_LX_KERNEL,
  ERR_NEED_MB_KERNEL,
  ERR_NO_DISK,
  ERR_NO_PART,
  ERR_NUMBER_PARSING,
  ERR_OUTSIDE_PART,
  ERR_READ,
  ERR_SYMLINK_LOOP,
  ERR_UNRECOGNIZED,
  ERR_WONT_FIT,
  ERR_WRITE,
  ERR_BAD_ARGUMENT,
  ERR_UNALIGNED,
  ERR_PRIVILEGED,
  ERR_DEV_NEED_INIT,
  ERR_NO_DISK_SPACE,
  ERR_NUMBER_OVERFLOW,

  MAX_ERR_NUM
} grub_error_t;

extern unsigned long install_partition;
extern unsigned long boot_drive;
extern unsigned long install_second_sector;
extern struct apm_info apm_bios_info;
extern unsigned long boot_part_addr;
extern int saved_entryno;
extern unsigned char force_lba;
extern char version_string[];
extern char config_file[];
extern unsigned long linux_text_len;
extern char *linux_data_tmp_addr;
extern char *linux_data_real_addr;

#ifdef GRUB_UTIL
extern int use_config_file;
extern int use_preset_menu;
extern int use_curses;
extern int verbose;
extern int read_only;
extern int floppy_disks;
extern char **device_map;
extern char *device_map_file;
extern struct geometry *disks;
extern void assign_device_name (int drive, const char *device);
#endif

#ifndef STAGE1_5
# define MAX_FALLBACK_ENTRIES	8
extern int fallback_entries[MAX_FALLBACK_ENTRIES];
extern int fallback_entryno;
extern int default_entry;
extern int current_entryno;

typedef enum
{
  PASSWORD_PLAIN,
  PASSWORD_MD5,
  PASSWORD_UNSUPPORTED
}
password_t;

extern char *password;
extern password_t password_type;
extern int auth;
extern char commands[];

extern int max_lines;
extern int count_lines;
extern int use_pager;
#endif

#ifndef NO_DECOMPRESSION
extern int no_decompression;
extern int compressed_file;
#endif

extern void (*disk_read_hook) (int, int, int);
extern void (*disk_read_func) (int, int, int);

#ifndef STAGE1_5
extern int debug;
#endif 

extern unsigned long current_drive;
extern unsigned long current_partition;

extern int fsys_type;

struct geometry
{
  
  unsigned long cylinders;
  
  unsigned long heads;
  
  unsigned long sectors;
  
  unsigned long total_sectors;
  
  unsigned long sector_size;
  
  unsigned long flags;
};

extern unsigned long part_start;
extern unsigned long part_length;

extern int current_slice;

extern int buf_drive;
extern int buf_track;
extern struct geometry buf_geom;

extern int filepos;
extern int filemax;


extern struct multiboot_info mbi;
extern unsigned long saved_drive;
extern unsigned long saved_partition;
extern unsigned long cdrom_drive;
#ifndef STAGE1_5
extern unsigned long saved_mem_upper;
extern unsigned long extended_memory;
#endif


extern grub_error_t errnum;
extern char *err_list[];

typedef void (*entry_func) (int, int, int, int, int, int)
     __attribute__ ((noreturn));

extern entry_func entry_addr;

void cmain (void);

void stop (void) __attribute__ ((noreturn));

void grub_reboot (void) __attribute__ ((noreturn));

void grub_halt (int no_apm) __attribute__ ((noreturn));

void set_int13_handler (unsigned short *map);

void set_int15_handler (void);

void unset_int15_handler (void);

void track_int13 (int drive);

extern unsigned short bios_key_map[];
extern unsigned short ascii_key_map[];
extern unsigned short io_map[];

void chain_stage1 (unsigned long segment, unsigned long offset,
		   unsigned long part_table_addr)
     __attribute__ ((noreturn));
void chain_stage2 (unsigned long segment, unsigned long offset,
		   int second_sector)
     __attribute__ ((noreturn));

void linux_boot (void) __attribute__ ((noreturn));

void big_linux_boot (void) __attribute__ ((noreturn));

void multi_boot (int start, int mb_info) __attribute__ ((noreturn));

void gateA20 (int linear);

int get_memsize (int type);
int get_eisamemsize (void);

int get_mmap_entry (struct mmar_desc *desc, int cont);

unsigned long get_rom_config_table (void);

void get_apm_info (void);

int get_vbe_controller_info (struct vbe_controller *controller);

int get_vbe_mode_info (int mode_number, struct vbe_mode *mode);

int set_vbe_mode (int mode_number);

int get_code_end (void);

int getrtsecs (void);
int currticks (void);

void cls (void);

int setcursor (int on);

int getxy (void);

void gotoxy (int x, int y);

void grub_putchar (int c);

int getkey (void);

int checkkey (void);

int get_diskinfo (int drive, struct geometry *geometry);
int biosdisk (int subfunc, int drive, struct geometry *geometry,
	      int sector, int nsec, int segment);
void stop_floppy (void);

#ifndef STAGE1_5

#define BUILTIN_CMDLINE		0x1	
#define BUILTIN_MENU		0x2	
#define BUILTIN_TITLE		0x4	
#define BUILTIN_SCRIPT		0x8	
#define BUILTIN_NO_ECHO		0x10	
#define BUILTIN_HELP_LIST	0x20	

struct builtin
{
  
  char *name;
  
  int (*func) (char *, int);
  
  int flags;
  
  char *short_doc;
  
  char *long_doc;
};

extern struct builtin *builtin_table[];

typedef enum
{
  KERNEL_TYPE_NONE,		
  KERNEL_TYPE_MULTIBOOT,	
  KERNEL_TYPE_LINUX,		
  KERNEL_TYPE_BIG_LINUX,	
  KERNEL_TYPE_FREEBSD,		
  KERNEL_TYPE_NETBSD,		
  KERNEL_TYPE_CHAINLOADER	
}
kernel_t;

extern kernel_t kernel_type;
extern int show_menu;
extern int grub_timeout;

void init_builtins (void);
void init_config (void);
char *skip_to (int after_equal, char *cmdline);
struct builtin *find_command (char *command);
void print_cmdline_message (int forever);
void enter_cmdline (char *heap, int forever);
int run_script (char *script, char *heap);
#endif

void grub_printf (const char *format,...);
int grub_sprintf (char *buffer, const char *format, ...);
int grub_tolower (int c);
int grub_isspace (int c);
int grub_strncat (char *s1, const char *s2, int n);
void *grub_memmove (void *to, const void *from, int len);
void *grub_memset (void *start, int c, int len);
int grub_strncat (char *s1, const char *s2, int n);
char *grub_strstr (const char *s1, const char *s2);
int grub_memcmp (const char *s1, const char *s2, int n);
int grub_strcmp (const char *s1, const char *s2);
int grub_strlen (const char *str);
char *grub_strcpy (char *dest, const char *src);

#ifndef GRUB_UTIL
typedef unsigned long grub_jmp_buf[6];
#else
# include <setjmp.h>
# define grub_jmp_buf jmp_buf
#endif

#ifdef GRUB_UTIL
# define grub_setjmp	setjmp
# define grub_longjmp	longjmp
#else 
int grub_setjmp (grub_jmp_buf env);
void grub_longjmp (grub_jmp_buf env, int val);
#endif 

extern grub_jmp_buf restart_env;
extern grub_jmp_buf restart_cmdline_env;

void init_page (void);
void print_error (void);
char *convert_to_ascii (char *buf, int c, ...);
int get_cmdline (char *prompt, char *cmdline, int maxlen,
		 int echo_char, int history);
int substring (const char *s1, const char *s2);
int nul_terminate (char *str);
int get_based_digit (int c, int base);
int safe_parse_maxint (char **str_ptr, int *myint_ptr);
int memcheck (int start, int len);
void grub_putstr (const char *str);

#ifndef NO_DECOMPRESSION
int gunzip_test_header (void);
int gunzip_read (char *buf, int len);
#endif 

int rawread (int drive, int sector, int byte_offset, int byte_len, char *buf);
int devread (int sector, int byte_offset, int byte_len, char *buf);
int rawwrite (int drive, int sector, char *buf);
int devwrite (int sector, int sector_len, char *buf);

char *set_device (char *device);
int open_device (void);
int real_open_partition (int flags);
int open_partition (void);
int next_partition (unsigned long drive, unsigned long dest,
		    unsigned long *partition, int *type,
		    unsigned long *start, unsigned long *len,
		    unsigned long *offset, int *entry,
		    unsigned long *ext_offset, char *buf);

int make_saved_active (void);

int set_partition_hidden_flag (int hidden);

int grub_open (char *filename);

int grub_read (char *buf, int len);

int grub_seek (int offset);

void grub_close (void);

int dir (char *dirname);

int set_bootdev (int hdbias);

void print_fsys_type (void);

void print_a_completion (char *filename);
int print_completions (int is_filename, int is_completion);

void copy_current_part_entry (char *buf);

#ifndef STAGE1_5
void bsd_boot (kernel_t type, int bootdev, char *arg)
     __attribute__ ((noreturn));

#define KERNEL_LOAD_NO_MEM_OPTION	(1 << 0)

kernel_t load_image (char *kernel, char *arg, kernel_t suggested_type,
		     unsigned long load_flags);

int load_module (char *module, char *arg);
int load_initrd (char *initrd);

int check_password(char *entered, char* expected, password_t type);
#endif

void init_bios_info (void);

#endif 

#endif 