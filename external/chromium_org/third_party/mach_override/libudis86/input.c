/* udis86 - libudis86/input.c
 *
 * Copyright (c) 2002-2009 Vivek Thampi
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, 
 *       this list of conditions and the following disclaimer in the documentation 
 *       and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "extern.h"
#include "types.h"
#include "input.h"
#include "udint.h"

static void
inp_init(struct ud *u)
{
  u->inp_curr = 0;
  u->inp_fill = 0;
  u->inp_ctr  = 0;
  u->inp_end  = 0;
}


static int 
inp_buff_hook(struct ud* u)
{
  if (u->inp_buff < u->inp_buff_end)
  return *u->inp_buff++;
  else  return -1;
}

#ifndef __UD_STANDALONE__
static int 
inp_file_hook(struct ud* u)
{
  return fgetc(u->inp_file);
}
#endif 

void 
ud_set_input_hook(register struct ud* u, int (*hook)(struct ud*))
{
  u->inp_hook = hook;
  inp_init(u);
}

void 
ud_set_input_buffer(register struct ud* u, const uint8_t* buf, size_t len)
{
  u->inp_hook = inp_buff_hook;
  u->inp_buff = buf;
  u->inp_buff_end = buf + len;
  inp_init(u);
}

#ifndef __UD_STANDALONE__
void 
ud_set_input_file(register struct ud* u, FILE* f)
{
  u->inp_hook = inp_file_hook;
  u->inp_file = f;
  inp_init(u);
}
#endif 

void 
ud_input_skip(struct ud* u, size_t n)
{
  while (n--) {
    u->inp_hook(u);
  }
}

int 
ud_input_end(const struct ud* u)
{
  return (u->inp_curr == u->inp_fill) && u->inp_end;
}


/* 
 * ud_inp_next
 *    Loads and returns the next byte from input.
 *
 *    inp_curr and inp_fill are pointers to the cache. The program is
 *    written based on the property that they are 8-bits in size, and
 *    will eventually wrap around forming a circular buffer. So, the 
 *    size of the cache is 256 in size, kind of unnecessary yet 
 *    optimal.
 *
 *    A buffer inp_sess stores the bytes disassembled for a single
 *    session.
 */
uint8_t
ud_inp_next(struct ud* u) 
{
  int c = -1;
  if (u->inp_curr != u->inp_fill) {
    c = u->inp_cache[ ++u->inp_curr ];
  
  } else if (u->inp_end || (c = u->inp_hook(u)) == -1) {
    UDERR(u, "byte expected, eoi received");
    
    u->inp_end = 1;
    return 0;
  } else {
    
    u->inp_curr = ++u->inp_fill;
    
    u->inp_cache[u->inp_fill] = c;
  }
  
  u->inp_sess[u->inp_ctr++] = c;
  
  return  (uint8_t) c;
}

