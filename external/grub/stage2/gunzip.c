/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999  Free Software Foundation, Inc.
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

/*
 * Most of this file was originally the source file "inflate.c", written
 * by Mark Adler.  It has been very heavily modified.  In particular, the
 * original would run through the whole file at once, and this version can
 * be stopped and restarted on any boundary during the decompression process.
 *
 * The license and header comments that file are included here.
 */

/* inflate.c -- Not copyrighted 1992 by Mark Adler
   version c10p1, 10 January 1993 */





#ifndef NO_DECOMPRESSION

#include "shared.h"

#include "filesys.h"

int no_decompression = 0;

int compressed_file;

static int gzip_data_offset;
static int gzip_filepos;
static int gzip_filemax;
static int gzip_fsmax;
static int saved_filepos;
static unsigned long gzip_crc;

static int block_type;
static int block_len;
static int last_block;
static int code_state;


static void initialize_tables (void);


static unsigned long linalloc_topaddr;

static void *
linalloc (int size)
{
  linalloc_topaddr = (linalloc_topaddr - size) & ~3;
  return (void *) linalloc_topaddr;
}

static void
reset_linalloc (void)
{
  linalloc_topaddr = RAW_ADDR ((mbi.mem_upper << 10) + 0x100000);
}


static void
gunzip_swap_values (void)
{
  register int itmp;

  
  itmp = filepos;
  filepos = gzip_filepos;
  gzip_filepos = itmp;

  
  itmp = filemax;
  filemax = gzip_filemax;
  gzip_filemax = itmp;

  
  itmp = fsmax;
  fsmax = gzip_fsmax;
  gzip_fsmax = itmp;
}


static int
bad_field (int len)
{
  char ch = 1;
  int not_retval = 1;

  do
    {
      if (len >= 0)
	{
	  if (!(len--))
	    break;
	}
      else
	{
	  if (!ch)
	    break;
	}
    }
  while ((not_retval = grub_read (&ch, 1)) == 1);

  return (!not_retval);
}


#define GZIP_HDR_LE      0x8B1F
#define OLD_GZIP_HDR_LE  0x9E1F

#define STORED      0
#define COMPRESSED  1
#define PACKED      2
#define LZHED       3
#define DEFLATED    8
#define MAX_METHODS 9

#define ASCII_FLAG   0x01	
#define CONTINUATION 0x02	
#define EXTRA_FIELD  0x04	
#define ORIG_NAME    0x08	
#define COMMENT      0x10	
#define ENCRYPTED    0x20	
#define RESERVED     0xC0	

#define UNSUPP_FLAGS (CONTINUATION|ENCRYPTED|RESERVED)

#define INFLATE_STORED    0
#define INFLATE_FIXED     1
#define INFLATE_DYNAMIC   2

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;


#define WSIZE 0x8000


int
gunzip_test_header (void)
{
  unsigned char buf[10];
  
  

  if (no_decompression
      || grub_read (buf, 10) != 10
      || ((*((unsigned short *) buf) != GZIP_HDR_LE)
	  && (*((unsigned short *) buf) != OLD_GZIP_HDR_LE)))
    {
      filepos = 0;
      return ! errnum;
    }

  if (buf[2] != DEFLATED
      || (buf[3] & UNSUPP_FLAGS)
      || ((buf[3] & EXTRA_FIELD)
	  && (grub_read (buf, 2) != 2
	      || bad_field (*((unsigned short *) buf))))
      || ((buf[3] & ORIG_NAME) && bad_field (-1))
      || ((buf[3] & COMMENT) && bad_field (-1)))
    {
      if (! errnum)
	errnum = ERR_BAD_GZIP_HEADER;
      
      return 0;
    }

  gzip_data_offset = filepos;
  
  filepos = filemax - 8;
  
  if (grub_read (buf, 8) != 8)
    {
      if (! errnum)
	errnum = ERR_BAD_GZIP_HEADER;
      
      return 0;
    }

  gzip_crc = *((unsigned long *) buf);
  gzip_fsmax = gzip_filemax = *((unsigned long *) (buf + 4));

  initialize_tables ();

  compressed_file = 1;
  gunzip_swap_values ();

  filepos = 0;

  return 1;
}


struct huft
{
  uch e;			
  uch b;			
  union
    {
      ush n;			
      struct huft *t;		
    }
  v;
};




static uch slide[WSIZE];

static unsigned wp;


static unsigned bitorder[] =
{				
  16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static ush cplens[] =
{				
  3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
  35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
	
static ush cplext[] =
{				
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
  3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99};	
static ush cpdist[] =
{				
  1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
  257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
  8193, 12289, 16385, 24577};
static ush cpdext[] =
{				
  0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
  7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
  12, 12, 13, 13};




static int lbits = 9;		
static int dbits = 6;		


#define BMAX 16			
#define N_MAX 288		


static unsigned hufts;		



static ulg bb;			
static unsigned bk;		

static ush mask_bits[] =
{
  0x0000,
  0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
  0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};

#define NEEDBITS(n) do {while(k<(n)){b|=((ulg)get_byte())<<k;k+=8;}} while (0)
#define DUMPBITS(n) do {b>>=(n);k-=(n);} while (0)

#define INBUFSIZ  0x2000

static uch inbuf[INBUFSIZ];
static int bufloc;

static int
get_byte (void)
{
  if (filepos == gzip_data_offset || bufloc == INBUFSIZ)
    {
      bufloc = 0;
      grub_read (inbuf, INBUFSIZ);
    }

  return inbuf[bufloc++];
}

static struct huft *tl;		
static struct huft *td;		
static int bl;			
static int bd;			


static int huft_build (unsigned *, unsigned, unsigned, ush *, ush *,
		       struct huft **, int *);
static int inflate_codes_in_window (void);



static int
huft_build (unsigned *b,	
	    unsigned n,		
	    unsigned s,		
	    ush * d,		
	    ush * e,		
	    struct huft **t,	
	    int *m)		
{
  unsigned a;			
  unsigned c[BMAX + 1];		
  unsigned f;			
  int g;			
  int h;			
  register unsigned i;		
  register unsigned j;		
  register int k;		
  int l;			
  register unsigned *p;		
  register struct huft *q;	
  struct huft r;		
  struct huft *u[BMAX];		
  unsigned v[N_MAX];		
  register int w;		
  unsigned x[BMAX + 1];		
  unsigned *xp;			
  int y;			
  unsigned z;			

  
  memset ((char *) c, 0, sizeof (c));
  p = b;
  i = n;
  do
    {
      c[*p]++;			
      p++;			
    }
  while (--i);
  if (c[0] == n)		
    {
      *t = (struct huft *) NULL;
      *m = 0;
      return 0;
    }

  
  l = *m;
  for (j = 1; j <= BMAX; j++)
    if (c[j])
      break;
  k = j;			
  if ((unsigned) l < j)
    l = j;
  for (i = BMAX; i; i--)
    if (c[i])
      break;
  g = i;			
  if ((unsigned) l > i)
    l = i;
  *m = l;

  
  for (y = 1 << j; j < i; j++, y <<= 1)
    if ((y -= c[j]) < 0)
      return 2;			
  if ((y -= c[i]) < 0)
    return 2;
  c[i] += y;

  
  x[1] = j = 0;
  p = c + 1;
  xp = x + 2;
  while (--i)
    {				
      *xp++ = (j += *p++);
    }

  
  p = b;
  i = 0;
  do
    {
      if ((j = *p++) != 0)
	v[x[j]++] = i;
    }
  while (++i < n);

  
  x[0] = i = 0;			
  p = v;			
  h = -1;			
  w = -l;			
  u[0] = (struct huft *) NULL;	
  q = (struct huft *) NULL;	
  z = 0;			

  
  for (; k <= g; k++)
    {
      a = c[k];
      while (a--)
	{
	  
	  
	  while (k > w + l)
	    {
	      h++;
	      w += l;		

	      
	      z = (z = g - w) > (unsigned) l ? l : z;	
	      if ((f = 1 << (j = k - w)) > a + 1)	
		{		
		  f -= a + 1;	
		  xp = c + k;
		  while (++j < z)	
		    {
		      if ((f <<= 1) <= *++xp)
			break;	
		      f -= *xp;	
		    }
		}
	      z = 1 << j;	

	      
	      q = (struct huft *) linalloc ((z + 1) * sizeof (struct huft));

	      hufts += z + 1;	
	      *t = q + 1;	
	      *(t = &(q->v.t)) = (struct huft *) NULL;
	      u[h] = ++q;	

	      
	      if (h)
		{
		  x[h] = i;	
		  r.b = (uch) l;	
		  r.e = (uch) (16 + j);		
		  r.v.t = q;	
		  j = i >> (w - l);	
		  u[h - 1][j] = r;	
		}
	    }

	  
	  r.b = (uch) (k - w);
	  if (p >= v + n)
	    r.e = 99;		
	  else if (*p < s)
	    {
	      r.e = (uch) (*p < 256 ? 16 : 15);		
	      r.v.n = (ush) (*p);	
	      p++;		
	    }
	  else
	    {
	      r.e = (uch) e[*p - s];	
	      r.v.n = d[*p++ - s];
	    }

	  
	  f = 1 << (k - w);
	  for (j = i >> w; j < z; j += f)
	    q[j] = r;

	  
	  for (j = 1 << (k - 1); i & j; j >>= 1)
	    i ^= j;
	  i ^= j;

	  
	  while ((i & ((1 << w) - 1)) != x[h])
	    {
	      h--;		
	      w -= l;
	    }
	}
    }

  
  return y != 0 && g != 1;
}



static unsigned inflate_n, inflate_d;

static int
inflate_codes_in_window (void)
{
  register unsigned e;		
  unsigned n, d;		
  unsigned w;			
  struct huft *t;		
  unsigned ml, md;		
  register ulg b;		
  register unsigned k;		

  
  d = inflate_d;
  n = inflate_n;
  b = bb;			
  k = bk;
  w = wp;			

  
  ml = mask_bits[bl];		
  md = mask_bits[bd];
  for (;;)			
    {
      if (!code_state)
	{
	  NEEDBITS ((unsigned) bl);
	  if ((e = (t = tl + ((unsigned) b & ml))->e) > 16)
	    do
	      {
		if (e == 99)
		  {
		    errnum = ERR_BAD_GZIP_DATA;
		    return 0;
		  }
		DUMPBITS (t->b);
		e -= 16;
		NEEDBITS (e);
	      }
	    while ((e = (t = t->v.t + ((unsigned) b & mask_bits[e]))->e) > 16);
	  DUMPBITS (t->b);

	  if (e == 16)		
	    {
	      slide[w++] = (uch) t->v.n;
	      if (w == WSIZE)
		break;
	    }
	  else
	    
	    {
	      
	      if (e == 15)
		{
		  block_len = 0;
		  break;
		}

	      
	      NEEDBITS (e);
	      n = t->v.n + ((unsigned) b & mask_bits[e]);
	      DUMPBITS (e);

	      
	      NEEDBITS ((unsigned) bd);
	      if ((e = (t = td + ((unsigned) b & md))->e) > 16)
		do
		  {
		    if (e == 99)
		      {
			errnum = ERR_BAD_GZIP_DATA;
			return 0;
		      }
		    DUMPBITS (t->b);
		    e -= 16;
		    NEEDBITS (e);
		  }
		while ((e = (t = t->v.t + ((unsigned) b & mask_bits[e]))->e)
		       > 16);
	      DUMPBITS (t->b);
	      NEEDBITS (e);
	      d = w - t->v.n - ((unsigned) b & mask_bits[e]);
	      DUMPBITS (e);
	      code_state++;
	    }
	}

      if (code_state)
	{
	  
	  do
	    {
	      n -= (e = (e = WSIZE - ((d &= WSIZE - 1) > w ? d : w)) > n ? n
		    : e);
	      if (w - d >= e)
		{
		  memmove (slide + w, slide + d, e);
		  w += e;
		  d += e;
		}
	      else
		
		{
		  while (e--)
		    slide[w++] = slide[d++];
		}
	      if (w == WSIZE)
		break;
	    }
	  while (n);

	  if (!n)
	    code_state--;

	  
	  if (w == WSIZE)
	    break;
	}
    }

  
  inflate_d = d;
  inflate_n = n;
  wp = w;			
  bb = b;			
  bk = k;

  return !block_len;
}



static void
init_stored_block (void)
{
  register ulg b;		
  register unsigned k;		

  
  b = bb;			
  k = bk;

  
  DUMPBITS (k & 7);

  
  NEEDBITS (16);
  block_len = ((unsigned) b & 0xffff);
  DUMPBITS (16);
  NEEDBITS (16);
  if (block_len != (unsigned) ((~b) & 0xffff))
    errnum = ERR_BAD_GZIP_DATA;
  DUMPBITS (16);

  
  bb = b;
  bk = k;
}



static void
init_fixed_block ()
{
  int i;			
  unsigned l[288];		

  
  for (i = 0; i < 144; i++)
    l[i] = 8;
  for (; i < 256; i++)
    l[i] = 9;
  for (; i < 280; i++)
    l[i] = 7;
  for (; i < 288; i++)		
    l[i] = 8;
  bl = 7;
  if ((i = huft_build (l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
    {
      errnum = ERR_BAD_GZIP_DATA;
      return;
    }

  
  for (i = 0; i < 30; i++)	
    l[i] = 5;
  bd = 5;
  if ((i = huft_build (l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
    {
      errnum = ERR_BAD_GZIP_DATA;
      return;
    }

  
  code_state = 0;
  block_len++;
}



static void
init_dynamic_block (void)
{
  int i;			
  unsigned j;
  unsigned l;			
  unsigned m;			
  unsigned n;			
  unsigned nb;			
  unsigned nl;			
  unsigned nd;			
  unsigned ll[286 + 30];	
  register ulg b;		
  register unsigned k;		

  
  b = bb;
  k = bk;

  
  NEEDBITS (5);
  nl = 257 + ((unsigned) b & 0x1f);	
  DUMPBITS (5);
  NEEDBITS (5);
  nd = 1 + ((unsigned) b & 0x1f);	
  DUMPBITS (5);
  NEEDBITS (4);
  nb = 4 + ((unsigned) b & 0xf);	
  DUMPBITS (4);
  if (nl > 286 || nd > 30)
    {
      errnum = ERR_BAD_GZIP_DATA;
      return;
    }

  
  for (j = 0; j < nb; j++)
    {
      NEEDBITS (3);
      ll[bitorder[j]] = (unsigned) b & 7;
      DUMPBITS (3);
    }
  for (; j < 19; j++)
    ll[bitorder[j]] = 0;

  
  bl = 7;
  if ((i = huft_build (ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
    {
      errnum = ERR_BAD_GZIP_DATA;
      return;
    }

  
  n = nl + nd;
  m = mask_bits[bl];
  i = l = 0;
  while ((unsigned) i < n)
    {
      NEEDBITS ((unsigned) bl);
      j = (td = tl + ((unsigned) b & m))->b;
      DUMPBITS (j);
      j = td->v.n;
      if (j < 16)		
	ll[i++] = l = j;	
      else if (j == 16)		
	{
	  NEEDBITS (2);
	  j = 3 + ((unsigned) b & 3);
	  DUMPBITS (2);
	  if ((unsigned) i + j > n)
	    {
	      errnum = ERR_BAD_GZIP_DATA;
	      return;
	    }
	  while (j--)
	    ll[i++] = l;
	}
      else if (j == 17)		
	{
	  NEEDBITS (3);
	  j = 3 + ((unsigned) b & 7);
	  DUMPBITS (3);
	  if ((unsigned) i + j > n)
	    {
	      errnum = ERR_BAD_GZIP_DATA;
	      return;
	    }
	  while (j--)
	    ll[i++] = 0;
	  l = 0;
	}
      else
	
	{
	  NEEDBITS (7);
	  j = 11 + ((unsigned) b & 0x7f);
	  DUMPBITS (7);
	  if ((unsigned) i + j > n)
	    {
	      errnum = ERR_BAD_GZIP_DATA;
	      return;
	    }
	  while (j--)
	    ll[i++] = 0;
	  l = 0;
	}
    }

  
  reset_linalloc ();

  
  bb = b;
  bk = k;

  
  bl = lbits;
  if ((i = huft_build (ll, nl, 257, cplens, cplext, &tl, &bl)) != 0)
    {
#if 0
      if (i == 1)
	printf ("gunzip: incomplete literal tree\n");
#endif

      errnum = ERR_BAD_GZIP_DATA;
      return;
    }
  bd = dbits;
  if ((i = huft_build (ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
    {
#if 0
      if (i == 1)
	printf ("gunzip: incomplete distance tree\n");
#endif

      errnum = ERR_BAD_GZIP_DATA;
      return;
    }

  
  code_state = 0;
  block_len++;
}


static void
get_new_block (void)
{
  register ulg b;		
  register unsigned k;		

  hufts = 0;

  
  b = bb;
  k = bk;

  
  NEEDBITS (1);
  last_block = (int) b & 1;
  DUMPBITS (1);

  
  NEEDBITS (2);
  block_type = (unsigned) b & 3;
  DUMPBITS (2);

  
  bb = b;
  bk = k;

  if (block_type == INFLATE_STORED)
    init_stored_block ();
  if (block_type == INFLATE_FIXED)
    init_fixed_block ();
  if (block_type == INFLATE_DYNAMIC)
    init_dynamic_block ();
}


static void
inflate_window (void)
{
  
  wp = 0;


  while (wp < WSIZE && !errnum)
    {
      if (!block_len)
	{
	  if (last_block)
	    break;

	  get_new_block ();
	}

      if (block_type > INFLATE_DYNAMIC)
	errnum = ERR_BAD_GZIP_DATA;

      if (errnum)
	return;

      if (block_type == INFLATE_STORED)
	{
	  int w = wp;


	  while (block_len && w < WSIZE && !errnum)
	    {
	      slide[w++] = get_byte ();
	      block_len--;
	    }

	  wp = w;

	  continue;
	}


      if (inflate_codes_in_window ())
	reset_linalloc ();
    }

  saved_filepos += WSIZE;

  
}


static void
initialize_tables (void)
{
  saved_filepos = 0;
  filepos = gzip_data_offset;

  
  bk = 0;
  bb = 0;

  
  last_block = 0;
  block_len = 0;

  
  reset_linalloc ();
}


int
gunzip_read (char *buf, int len)
{
  int ret = 0;

  compressed_file = 0;
  gunzip_swap_values ();

  
  if (saved_filepos > gzip_filepos + WSIZE)
    initialize_tables ();


  while (len > 0 && !errnum)
    {
      register int size;
      register char *srcaddr;

      while (gzip_filepos >= saved_filepos)
	inflate_window ();

      srcaddr = (char *) ((gzip_filepos & (WSIZE - 1)) + slide);
      size = saved_filepos - gzip_filepos;
      if (size > len)
	size = len;

      memmove (buf, srcaddr, size);

      buf += size;
      len -= size;
      gzip_filepos += size;
      ret += size;
    }

  compressed_file = 1;
  gunzip_swap_values ();

  if (errnum)
    ret = 0;

  return ret;
}

#endif 
