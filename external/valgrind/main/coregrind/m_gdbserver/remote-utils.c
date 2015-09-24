/* Remote utility routines for the remote server for GDB.
   Copyright (C) 1986, 1989, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2004, 2005, 2006, 2011
   Free Software Foundation, Inc.

   This file is part of GDB.
   It has been modified to integrate it in valgrind

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#include "pub_core_basics.h"
#include "pub_core_vki.h"
#include "pub_core_vkiscnums.h"
#include "pub_core_libcsignal.h"
#include "pub_core_options.h"

#include "server.h"

#  if defined(VGO_linux)
#include <sys/prctl.h>
#  endif

Bool noack_mode;

static int readchar (int single);

void remote_utils_output_status(void);

static int remote_desc;

static VgdbShared *shared;
static int  last_looked_cntr = -1;
static struct vki_pollfd remote_desc_pollfdread_activity;
#define INVALID_DESCRIPTOR -1

static int write_remote_desc = INVALID_DESCRIPTOR;
static int pid_from_to_creator;
static int mknod_done = 0;

static char *from_gdb = NULL;
static char *to_gdb = NULL;
static char *shared_mem = NULL;

static
int open_fifo (char *side, char *path, int flags)
{
  SysRes o;
  int fd;
  dlog(1, "Opening %s side %s\n", side, path);
  o = VG_(open) (path, flags, 0);
  if (sr_isError (o)) {
     sr_perror(o, "open fifo %s\n", path);
     fatal ("valgrind: fatal error: vgdb FIFO cannot be opened.\n");
  } else {
     fd = sr_Res(o);
     dlog(1, "result fd %d\n", fd);
  }
  fd = VG_(safe_fd)(fd);
  dlog(1, "result safe_fd %d\n", fd);
  if (fd == -1)
     fatal("safe_fd for vgdb FIFO failed\n");
  return fd;
}

void remote_utils_output_status(void)
{
   if (shared == NULL)
      VG_(umsg)("remote communication not initialized\n");
   else
      VG_(umsg)("shared->written_by_vgdb %d shared->seen_by_valgrind %d\n",
                shared->written_by_vgdb, shared->seen_by_valgrind);
}

static
int vgdb_state_looks_bad(char* where)
{
   if (VG_(kill)(shared->vgdb_pid, 0) != 0)
      return 1; 

   if (remote_desc_activity(where) == 2)
      return 2; 

   if (remote_desc == INVALID_DESCRIPTOR)
      return 3; 
       
   return 0; 
}

static
void set_ptracer(void)
{
#ifdef PR_SET_PTRACER
   SysRes o;
   char *ptrace_scope_setting_file = "/proc/sys/kernel/yama/ptrace_scope";
   int fd;
   char ptrace_scope;
   int ret;

   o = VG_(open) (ptrace_scope_setting_file, VKI_O_RDONLY, 0);
   if (sr_isError(o)) {
      if (VG_(debugLog_getLevel)() >= 1) {
         sr_perror(o, "error VG_(open) %s\n", ptrace_scope_setting_file);
      }
      
      return;
   }
   fd = sr_Res(o);
   if (VG_(read) (fd, &ptrace_scope, 1) == 1) {
      dlog(1, "ptrace_scope %c\n", ptrace_scope);
      if (ptrace_scope != '0') {
         ret = VG_(prctl) (PR_SET_PTRACER, shared->vgdb_pid, 0, 0, 0);
         dlog(1, "set_ptracer to vgdb_pid %d result %d\n",
              shared->vgdb_pid, ret);
      }
   } else {
      dlog(0, "Could not read the ptrace_scope setting from %s\n",
           ptrace_scope_setting_file);
   }

   VG_(close) (fd);
#endif
}

static
int poll_cond (short revents)
{
   return (revents & (VKI_POLLERR | VKI_POLLHUP | VKI_POLLNVAL));
}

static
int ensure_write_remote_desc(void)
{
   struct vki_pollfd write_remote_desc_ok;
   int ret;
   if (write_remote_desc != INVALID_DESCRIPTOR) {
      write_remote_desc_ok.fd = write_remote_desc;
      write_remote_desc_ok.events = VKI_POLLOUT;
      write_remote_desc_ok.revents = 0;
      ret = VG_(poll)(&write_remote_desc_ok, 1, 0);
      if (ret && poll_cond(write_remote_desc_ok.revents)) {
         dlog(1, "POLLcond %d closing write_remote_desc %d\n", 
              write_remote_desc_ok.revents, write_remote_desc);
         VG_(close) (write_remote_desc);
         write_remote_desc = INVALID_DESCRIPTOR;
      }
   }
   if (write_remote_desc == INVALID_DESCRIPTOR) {
      if (!vgdb_state_looks_bad("bad?@ensure_write_remote_desc")) {
         set_ptracer();
         write_remote_desc = open_fifo ("write", to_gdb, VKI_O_WRONLY);
      }
   }

   return (write_remote_desc != INVALID_DESCRIPTOR);
}

#if defined(VGO_darwin)
#define VKI_S_IFIFO 0010000
#endif
static
void safe_mknod (char *nod)
{
   SysRes m;
   m = VG_(mknod) (nod, VKI_S_IFIFO|0600, 0);
   if (sr_isError (m)) {
      if (sr_Err (m) == VKI_EEXIST) {
         if (VG_(clo_verbosity) > 1) {
            VG_(umsg)("%s already created\n", nod);
         }
      } else {
         sr_perror(m, "mknod %s\n", nod);
         VG_(umsg) ("valgrind: fatal error: vgdb FIFOs cannot be created.\n");
         VG_(exit)(1);
      }
   }
}


void remote_open (char *name)
{
   const HChar *user, *host;
   int save_fcntl_flags, len;
   VgdbShared vgdbinit = 
      {0, 0, (Addr) VG_(invoke_gdbserver),
       (Addr) VG_(threads), sizeof(ThreadState), 
       offsetof(ThreadState, status),
       offsetof(ThreadState, os_state) + offsetof(ThreadOSstate, lwpid),
       0};
   const int pid = VG_(getpid)();
   const int name_default = strcmp(name, VG_(vgdb_prefix_default)()) == 0;
   Addr addr_shared;
   SysRes o;
   int shared_mem_fd = INVALID_DESCRIPTOR;
   
   user = VG_(getenv)("LOGNAME");
   if (user == NULL) user = VG_(getenv)("USER");
   if (user == NULL) user = "???";

   host = VG_(getenv)("HOST");
   if (host == NULL) host = VG_(getenv)("HOSTNAME");
   if (host == NULL) host = "???";

   len = strlen(name) + strlen(user) + strlen(host) + 40;

   if (from_gdb != NULL) 
      free (from_gdb);
   from_gdb = malloc (len);
   if (to_gdb != NULL)
      free (to_gdb);
   to_gdb = malloc (len);
   if (shared_mem != NULL)
      free (shared_mem);
   shared_mem = malloc (len);
   
   VG_(sprintf) (from_gdb,   "%s-from-vgdb-to-%d-by-%s-on-%s",    name,
                 pid, user, host);
   VG_(sprintf) (to_gdb,     "%s-to-vgdb-from-%d-by-%s-on-%s",    name,
                 pid, user, host);
   VG_(sprintf) (shared_mem, "%s-shared-mem-vgdb-%d-by-%s-on-%s", name,
                 pid, user, host);
   if (VG_(clo_verbosity) > 1) {
      VG_(umsg)("embedded gdbserver: reading from %s\n", from_gdb);
      VG_(umsg)("embedded gdbserver: writing to   %s\n", to_gdb);
      VG_(umsg)("embedded gdbserver: shared mem   %s\n", shared_mem);
      VG_(umsg)("\n");
      VG_(umsg)("TO CONTROL THIS PROCESS USING vgdb (which you probably\n"
                "don't want to do, unless you know exactly what you're doing,\n"
                "or are doing some strange experiment):\n"
                "  %s/../../bin/vgdb --pid=%d%s%s ...command...\n",
                VG_(libdir),
                pid, (name_default ? "" : " --vgdb-prefix="),
                (name_default ? "" : name));
   }
   if (VG_(clo_verbosity) > 1 
       || VG_(clo_vgdb_error) < 999999999) {
      VG_(umsg)("\n");
      VG_(umsg)(
         "TO DEBUG THIS PROCESS USING GDB: start GDB like this\n"
         "  /path/to/gdb %s\n"
         "and then give GDB the following command\n"
         "  target remote | %s/../../bin/vgdb --pid=%d%s%s\n",
         VG_(args_the_exename),
         VG_(libdir),
         pid, (name_default ? "" : " --vgdb-prefix="), 
         (name_default ? "" : name)
      );
      VG_(umsg)("--pid is optional if only one valgrind process is running\n");
      VG_(umsg)("\n");
   }

   if (!mknod_done) {
      mknod_done++;

      VG_(unlink)(from_gdb);
      VG_(unlink)(to_gdb);
      VG_(unlink)(shared_mem);

      safe_mknod(from_gdb);
      safe_mknod(to_gdb);

      pid_from_to_creator = pid;
      
      o = VG_(open) (shared_mem, VKI_O_CREAT|VKI_O_RDWR, 0600);
      if (sr_isError (o)) {
         sr_perror(o, "cannot create shared_mem file %s\n", shared_mem);
         fatal("");
      } else {
         shared_mem_fd = sr_Res(o);
      }
      
      if (VG_(write)(shared_mem_fd, &vgdbinit, sizeof(VgdbShared)) 
          != sizeof(VgdbShared)) {
         fatal("error writing %d bytes to shared mem %s\n",
               (int) sizeof(VgdbShared), shared_mem);
      }
      {
         SysRes res = VG_(am_shared_mmap_file_float_valgrind)
            (sizeof(VgdbShared), VKI_PROT_READ|VKI_PROT_WRITE, 
             shared_mem_fd, (Off64T)0);
         if (sr_isError(res)) {
            sr_perror(res, "error VG_(am_shared_mmap_file_float_valgrind) %s\n",
                      shared_mem);
            fatal("");
         }  
         addr_shared = sr_Res (res);
      }
      shared = (VgdbShared*) addr_shared;
      VG_(close) (shared_mem_fd);
   }
   
   remote_desc = open_fifo ("read", from_gdb, VKI_O_RDONLY|VKI_O_NONBLOCK);
   save_fcntl_flags = VG_(fcntl) (remote_desc, VKI_F_GETFL, 0);
   VG_(fcntl) (remote_desc, VKI_F_SETFL, save_fcntl_flags & ~VKI_O_NONBLOCK);
   remote_desc_pollfdread_activity.fd = remote_desc;
   remote_desc_pollfdread_activity.events = VKI_POLLIN;
   remote_desc_pollfdread_activity.revents = 0;
}

void sync_gdb_connection(void)
{
   VG_(poll)(0, 0, 100);
}

static
char * ppFinishReason (FinishReason reason)
{
   switch (reason) {
   case orderly_finish:    return "orderly_finish";
   case reset_after_error: return "reset_after_error";
   case reset_after_fork:  return "reset_after_fork";
   default: vg_assert (0);
   }
}

void remote_finish (FinishReason reason)
{
   dlog(1, "remote_finish (reason %s) %d %d\n", 
        ppFinishReason(reason), remote_desc, write_remote_desc);
   reset_valgrind_sink(ppFinishReason(reason));
   if (write_remote_desc != INVALID_DESCRIPTOR)
      VG_(close) (write_remote_desc);
   write_remote_desc = INVALID_DESCRIPTOR;
   if (remote_desc != INVALID_DESCRIPTOR) {
      remote_desc_pollfdread_activity.fd = INVALID_DESCRIPTOR;
      remote_desc_pollfdread_activity.events = 0;
      remote_desc_pollfdread_activity.revents = 0;
      VG_(close) (remote_desc);
   }
   remote_desc = INVALID_DESCRIPTOR;
   noack_mode = False;
   
   
   if (reason == reset_after_fork)
      mknod_done = 0;
   
   if (reason == reset_after_error)
      sync_gdb_connection();
}

void remote_close (void)
{
   const int pid = VG_(getpid)();
   remote_finish(orderly_finish);
   if (pid == pid_from_to_creator) {
      dlog(1, "unlinking\n    %s\n    %s\n    %s\n", 
           from_gdb, to_gdb, shared_mem);
      if (VG_(unlink) (from_gdb) == -1)
         warning ("could not unlink %s\n", from_gdb);
      if (VG_(unlink) (to_gdb) == -1)
         warning ("could not unlink %s\n", to_gdb);
      if (VG_(unlink) (shared_mem) == -1)
         warning ("could not unlink %s\n", shared_mem);
   }
   else {
      dlog(1, "not creator => not unlinking %s and %s\n", from_gdb, to_gdb);
   }
   free (from_gdb);
   free (to_gdb);
}

Bool remote_connected(void)
{
   return write_remote_desc != INVALID_DESCRIPTOR;
}

static
void error_poll_cond(void)
{
   shared->seen_by_valgrind = shared->written_by_vgdb;
   remote_finish(reset_after_error);
}

int remote_desc_activity(char *msg)
{
   int ret;
   const int looking_at = shared->written_by_vgdb;
   if (shared->seen_by_valgrind == looking_at)
   
      return 0;
   if (remote_desc == INVALID_DESCRIPTOR)
      return 0;

   
   remote_desc_pollfdread_activity.revents = 0;
   ret = VG_(poll) (&remote_desc_pollfdread_activity, 1, 0);
   if (ret && poll_cond(remote_desc_pollfdread_activity.revents)) {
      dlog(1, "POLLcond %d remote_desc_pollfdread %d\n", 
           remote_desc_pollfdread_activity.revents, remote_desc);
      error_poll_cond();
      ret = 2;
   }
   dlog(1,
        "remote_desc_activity %s %d last_looked_cntr %d looking_at %d"
        " shared->written_by_vgdb %d shared->seen_by_valgrind %d"
        " ret %d\n", 
        msg, remote_desc, last_looked_cntr, looking_at, 
        shared->written_by_vgdb, shared->seen_by_valgrind,
        ret);
   
   if (ret != 2)
      last_looked_cntr = looking_at;
   return ret;
}


static
int fromhex (int a)
{
   if (a >= '0' && a <= '9')
      return a - '0';
   else if (a >= 'a' && a <= 'f')
      return a - 'a' + 10;
   else
      error ("Reply contains invalid hex digit 0x%x\n", a);
   return 0;
}

int unhexify (char *bin, const char *hex, int count)
{
   int i;
   
   for (i = 0; i < count; i++) {
      if (hex[0] == 0 || hex[1] == 0) {
         return i;
      }
      *bin++ = fromhex (hex[0]) * 16 + fromhex (hex[1]);
      hex += 2;
   }
   return i;
}

void decode_address (CORE_ADDR *addrp, const char *start, int len)
{
   CORE_ADDR addr;
   char ch;
   int i;
   
   addr = 0;
   for (i = 0; i < len; i++) {
      ch = start[i];
      addr = addr << 4;
      addr = addr | (fromhex (ch) & 0x0f);
   }
   *addrp = addr;
}


static
int tohex (int nib)
{
   if (nib < 10)
      return '0' + nib;
   else
      return 'a' + nib - 10;
}

int hexify (char *hex, const char *bin, int count)
{
   int i;

   
   if (count == 0)
      count = strlen (bin);

  for (i = 0; i < count; i++) {
     *hex++ = tohex ((*bin >> 4) & 0xf);
     *hex++ = tohex (*bin++ & 0xf);
  }
  *hex = 0;
  return i;
}

char* heximage (char *buf, char *bin, int count)
{
#if defined(VGA_x86) || defined(VGA_amd64)
   char rev[count]; 
   
  int i;
  for (i = 0; i < count; i++)
    rev[i] = bin[count - i - 1];
  hexify (buf, rev, count);
#else
  hexify (buf, bin, count);
#endif
  return buf;
}

void* C2v(CORE_ADDR addr)
{
   return (void*) addr;
}



int
remote_escape_output (const gdb_byte *buffer, int len,
		      gdb_byte *out_buf, int *out_len,
		      int out_maxlen)
{
   int input_index, output_index;
   
   output_index = 0;
   for (input_index = 0; input_index < len; input_index++) {
      gdb_byte b = buffer[input_index];

      if (b == '$' || b == '#' || b == '}' || b == '*') {
         
         if (output_index + 2 > out_maxlen)
	    break;
         out_buf[output_index++] = '}';
         out_buf[output_index++] = b ^ 0x20;
      } else {
         if (output_index + 1 > out_maxlen)
	    break;
         out_buf[output_index++] = b;
      }
   }

   *out_len = input_index;
   return output_index;
}

/* Convert BUFFER, escaped data LEN bytes long, into binary data
   in OUT_BUF.  Return the number of bytes written to OUT_BUF.
   Raise an error if the total number of bytes exceeds OUT_MAXLEN.

   This function reverses remote_escape_output.  It allows more
   escaped characters than that function does, in particular because
   '*' must be escaped to avoid the run-length encoding processing
   in reading packets.  */

static
int remote_unescape_input (const gdb_byte *buffer, int len,
		       gdb_byte *out_buf, int out_maxlen)
{
   int input_index, output_index;
   int escaped;
   
   output_index = 0;
   escaped = 0;
   for (input_index = 0; input_index < len; input_index++) {
      gdb_byte b = buffer[input_index];

      if (output_index + 1 > out_maxlen)
         error ("Received too much data (len %d) from the target.\n", len);

      if (escaped) {
         out_buf[output_index++] = b ^ 0x20;
         escaped = 0;
      } else if (b == '}') {
         escaped = 1;
      } else {
         out_buf[output_index++] = b;
      }
   }

   if (escaped)
      error ("Unmatched escape character in target response.\n");

   return output_index;
}


static
int try_rle (char *buf, int remaining, unsigned char *csum, char **p)
{
   int n;

   
   *csum += buf[0];
   *(*p)++ = buf[0];

   
   if (remaining > 97)
      remaining = 97;
   
   for (n = 1; n < remaining; n++)
      if (buf[n] != buf[0])
         break;
   
   n--;
   
   if (n < 3)
      return 1;
   
   while (n + 29 == '$' || n + 29 == '#')
      n--;
   
   *csum += '*';
   *(*p)++ = '*';
   *csum += n + 29;
   *(*p)++ = n + 29;
  
   return n + 1;
}


int putpkt_binary (char *buf, int cnt)
{
   int i;
   unsigned char csum = 0;
   char *buf2;
   char *p;
   int cc;

   buf2 = malloc (PBUFSIZ);


   p = buf2;
   *p++ = '$';

   for (i = 0; i < cnt;)
      i += try_rle (buf + i, cnt - i, &csum, &p);

   *p++ = '#';
   *p++ = tohex ((csum >> 4) & 0xf);
   *p++ = tohex (csum & 0xf);

   *p = '\0';

   
   if (!ensure_write_remote_desc()) {
      warning ("putpkt(write) error: no write_remote_desc\n");
      return -1;
   }


   do {
      if (VG_(write) (write_remote_desc, buf2, p - buf2) != p - buf2) {
         warning ("putpkt(write) error\n");
         return -1;
      }

      if (noack_mode)
         dlog(1, "putpkt (\"%s\"); [no ack]\n", buf2);
      else
         dlog(1,"putpkt (\"%s\"); [looking for ack]\n", buf2);

      if (noack_mode)
         break;

      cc = readchar (1);
      if (cc > 0)
         dlog(1, "[received '%c' (0x%x)]\n", cc, cc);

      if (cc <= 0) {
         if (cc == 0)
            dlog(1, "putpkt(read): Got EOF\n");
         else
	    warning ("putpkt(read) error\n");

         free (buf2);
         return -1;
      }

      
      if (cc == '\003')
         dlog(1, "Received 0x03 character (SIGINT)\n");
   }
   while (cc != '+');

   free (buf2);
   return 1;			
}


int putpkt (char *buf)
{
   return putpkt_binary (buf, strlen (buf));
}

void monitor_output (char *s)
{
   const int len = strlen(s);
   char *buf = malloc(1 + 2*len + 1);

   buf[0] = 'O';
   hexify(buf+1, s, len);
   if (putpkt (buf) < 0) {
      
      reset_valgrind_sink("Error writing monitor output");
      
      VG_(printf) ("%s", s);
   }
   
   free (buf);
}

static
int readchar (int single)
{
   static unsigned char buf[PBUFSIZ];
   static int bufcnt = 0;
   static unsigned char *bufp;
   int ret;
  
   if (bufcnt-- > 0)
      return *bufp++;

   if (remote_desc == INVALID_DESCRIPTOR)
      return -1;

   remote_desc_pollfdread_activity.revents = 0;
   ret = VG_(poll)(&remote_desc_pollfdread_activity, 1, -1);
   if (ret != 1) {
      dlog(0, "readchar: poll got %d\n", ret);
      return -1;
   }
   if (single)
      bufcnt = VG_(read) (remote_desc, buf, 1);
   else
      bufcnt = VG_(read) (remote_desc, buf, sizeof (buf));

   if (bufcnt <= 0) {
      if (bufcnt == 0)
         dlog (1, "readchar: Got EOF\n");
      else
         warning ("readchar read error\n");
      
      return -1;
   }

   shared->seen_by_valgrind += bufcnt;
   
   if (write_remote_desc == INVALID_DESCRIPTOR 
       && !ensure_write_remote_desc()) {
      dlog(1, "reachar: write_remote_desc could not be created");
   }

   bufp = buf;
   bufcnt--;

   if (poll_cond(remote_desc_pollfdread_activity.revents)) {
      dlog(1, "readchar: POLLcond got %d\n",
           remote_desc_pollfdread_activity.revents);
      error_poll_cond();
   }

   return *bufp++;
}



int getpkt (char *buf)
{
   char *bp;
   unsigned char csum, c1, c2;
   int c;
  
   while (1) {
      csum = 0;
      
      while (1) {
         c = readchar (0);
         if (c == '$')
	    break;
         dlog(1, "[getpkt: discarding char '%c']\n", c);
         if (c < 0)
	    return -1;
      }

      bp = buf;
      while (1) {
         c = readchar (0);
         if (c < 0)
	    return -1;
         if (c == '#')
	    break;
         *bp++ = c;
         csum += c;
      }
      *bp = 0;

      c1 = fromhex (readchar (0));
      c2 = fromhex (readchar (0));

      if (csum == (c1 << 4) + c2)
         break;

      dlog (0, "Bad checksum, sentsum=0x%x, csum=0x%x, buf=%s\n",
            (c1 << 4) + c2, csum, buf);
      if (!ensure_write_remote_desc()) {
         dlog(1, "getpkt(write nack) no write_remote_desc");
      }
      VG_(write) (write_remote_desc, "-", 1);
   }

   if (noack_mode)
      dlog(1, "getpkt (\"%s\");  [no ack] \n", buf);
   else
      dlog(1, "getpkt (\"%s\");  [sending ack] \n", buf);

   if (!noack_mode) {
      if (!ensure_write_remote_desc()) {
         dlog(1, "getpkt(write ack) no write_remote_desc");
      }
      VG_(write) (write_remote_desc, "+", 1);
      dlog(1, "[sent ack]\n");
   }

   return bp - buf;
}

void write_ok (char *buf)
{
   buf[0] = 'O';
   buf[1] = 'K';
   buf[2] = '\0';
}

void write_enn (char *buf)
{
   
   buf[0] = 'E';
   buf[1] = '0';
   buf[2] = '1';
   buf[3] = '\0';
}

void convert_int_to_ascii (unsigned char *from, char *to, int n)
{
   int nib;
   int ch;
   while (n--) {
      ch = *from++;
      nib = ((ch & 0xf0) >> 4) & 0x0f;
      *to++ = tohex (nib);
      nib = ch & 0x0f;
      *to++ = tohex (nib);
   }
   *to++ = 0;
}


void convert_ascii_to_int (char *from, unsigned char *to, int n)
{
   int nib1, nib2;
   while (n--) {
      nib1 = fromhex (*from++);
      nib2 = fromhex (*from++);
      *to++ = (((nib1 & 0x0f) << 4) & 0xf0) | (nib2 & 0x0f);
   }
}

static
char * outreg (int regno, char *buf)
{
   if ((regno >> 12) != 0)
      *buf++ = tohex ((regno >> 12) & 0xf);
   if ((regno >> 8) != 0)
      *buf++ = tohex ((regno >> 8) & 0xf);
   *buf++ = tohex ((regno >> 4) & 0xf);
   *buf++ = tohex (regno & 0xf);
   *buf++ = ':';
   collect_register_as_string (regno, buf);
   buf += 2 * register_size (regno);
   *buf++ = ';';

   return buf;
}

void prepare_resume_reply (char *buf, char status, unsigned char sig)
{
   int nib;
   
   *buf++ = status;

   nib = ((sig & 0xf0) >> 4);
   *buf++ = tohex (nib);
   nib = sig & 0x0f;
   *buf++ = tohex (nib);

   if (status == 'T') {
      const char **regp = gdbserver_expedite_regs;
      
      if (valgrind_stopped_by_watchpoint()) {
         CORE_ADDR addr;
         int i;

         strncpy (buf, "watch:", 6);
         buf += 6;

         addr = valgrind_stopped_data_address ();

         for (i = sizeof (void *) * 2; i > 0; i--) {
            *buf++ = tohex ((addr >> (i - 1) * 4) & 0xf);
         }
         *buf++ = ';';
      }

      while (*regp) {
         buf = outreg (find_regno (*regp), buf);
         regp ++;
      }

      {
         unsigned int gdb_id_from_wait;
         
         
         thread_from_wait = 
            ((struct inferior_list_entry *)current_inferior)->id;
         gdb_id_from_wait = thread_to_gdb_id (current_inferior);
         
         dlog(1, "Writing resume reply for %ld\n", thread_from_wait);
         if (1 || old_thread_from_wait != thread_from_wait) {
            general_thread = thread_from_wait;
            VG_(sprintf) (buf, "thread:%x;", gdb_id_from_wait);
            buf += strlen (buf);
            old_thread_from_wait = thread_from_wait;
         }
      }
   }
   
   *buf++ = 0;
}

void decode_m_packet (char *from, CORE_ADDR *mem_addr_ptr, unsigned int *len_ptr)
{
   int i = 0, j = 0;
   char ch;
   *mem_addr_ptr = *len_ptr = 0;

   while ((ch = from[i++]) != ',') {
      *mem_addr_ptr = *mem_addr_ptr << 4;
      *mem_addr_ptr |= fromhex (ch) & 0x0f;
   }

   for (j = 0; j < 4; j++) {
      if ((ch = from[i++]) == 0)
         break;
      *len_ptr = *len_ptr << 4;
      *len_ptr |= fromhex (ch) & 0x0f;
   }
}

void decode_M_packet (char *from, CORE_ADDR *mem_addr_ptr, unsigned int *len_ptr,
		 unsigned char *to)
{
   int i = 0;
   char ch;
   *mem_addr_ptr = *len_ptr = 0;

   while ((ch = from[i++]) != ',') {
      *mem_addr_ptr = *mem_addr_ptr << 4;
      *mem_addr_ptr |= fromhex (ch) & 0x0f;
   }

   while ((ch = from[i++]) != ':') {
      *len_ptr = *len_ptr << 4;
      *len_ptr |= fromhex (ch) & 0x0f;
   }

   convert_ascii_to_int (&from[i++], to, *len_ptr);
}

int decode_X_packet (char *from, int packet_len, CORE_ADDR *mem_addr_ptr,
		 unsigned int *len_ptr, unsigned char *to)
{
   int i = 0;
   char ch;
   *mem_addr_ptr = *len_ptr = 0;
   
   while ((ch = from[i++]) != ',') {
      *mem_addr_ptr = *mem_addr_ptr << 4;
      *mem_addr_ptr |= fromhex (ch) & 0x0f;
   }

   while ((ch = from[i++]) != ':') {
      *len_ptr = *len_ptr << 4;
      *len_ptr |= fromhex (ch) & 0x0f;
   }

   if (remote_unescape_input ((const gdb_byte *) &from[i], packet_len - i,
                              to, *len_ptr) != *len_ptr)
      return -1;

   return 0;
}


HChar *
VG_(vgdb_prefix_default)(void)
{
   static HChar *prefix;
   
   if (prefix == NULL) {
     const HChar *tmpdir = VG_(tmpdir)();
     prefix = malloc(strlen(tmpdir) + strlen("/vgdb-pipe") + 1);
     strcpy(prefix, tmpdir);
     strcat(prefix, "/vgdb-pipe");
   }
   return prefix;
}
