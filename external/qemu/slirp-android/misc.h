/*
 * Copyright (c) 1995 Danny Gasparovski.
 *
 * Please read the file COPYRIGHT for the
 * terms and conditions of the copyright.
 */

#ifndef _MISC_H_
#define _MISC_H_

struct ex_list {
	int ex_pty;			
	int ex_addr;			
	int ex_fport;                   
	const char *ex_exec;            
	struct ex_list *ex_next;
};

extern struct ex_list *exec_list;
extern u_int time_fasttimo, last_slowtimo;

extern int (*lprint_print) _P((void *, const char *, va_list));
extern char *lprint_ptr, *lprint_ptr2, **lprint_arg;
extern struct sbuf *lprint_sb;

#ifndef HAVE_STRDUP
char *strdup _P((const char *));
#endif

void do_wait _P((int));

#define EMU_NONE 0x0

#define EMU_CTL 0x1
#define EMU_FTP 0x2
#define EMU_KSH 0x3
#define EMU_IRC 0x4
#define EMU_REALAUDIO 0x5
#define EMU_RLOGIN 0x6
#define EMU_IDENT 0x7
#define EMU_RSH 0x8

#define EMU_NOCONNECT 0x10	

#define EMU_TALK	0x1
#define EMU_NTALK	0x2
#define EMU_CUSEEME	0x3

struct tos_t {
	u_int16_t lport;
	u_int16_t fport;
	u_int8_t tos;
	u_int8_t emu;
};

struct emu_t {
	u_int16_t lport;
	u_int16_t fport;
	u_int8_t tos;
	u_int8_t emu;
	struct emu_t *next;
};

#ifndef CONFIG_QEMU
extern struct emu_t *tcpemu;
#endif

void getouraddr _P((void));
void slirp_insque _P((void *, void *));
void slirp_remque _P((void *));
int add_exec _P((struct ex_list **, int, char *, int, int));
int slirp_openpty _P((int *, int *));
int fork_exec(struct socket *so, const char *ex, int do_pty);
void snooze_hup _P((int));
void snooze _P((void));
void relay _P((int));
void add_emu _P((char *));

#endif