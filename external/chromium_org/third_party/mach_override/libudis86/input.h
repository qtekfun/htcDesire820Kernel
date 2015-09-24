/* udis86 - libudis86/input.h
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
#ifndef UD_INPUT_H
#define UD_INPUT_H

#include "types.h"
#include "udint.h"

uint8_t ud_inp_next(struct ud* u);

static inline void
inp_start(struct ud *u)
{
  u->inp_ctr = 0;
}

static inline void
inp_reset(struct ud *u)
{
  u->inp_curr -= u->inp_ctr;
  u->inp_ctr   = 0;
}

static inline uint8_t*
inp_sess(struct ud *u)
{
  return u->inp_sess;
}

static inline uint8_t
inp_curr(const struct ud *u)
{
  return u->inp_cache[u->inp_curr];
}

static inline void
inp_back(struct ud* u) 
{
  if (u->inp_ctr > 0) {
    --u->inp_curr;
    --u->inp_ctr;
  }
}

static inline uint8_t
inp_peek(struct ud* u) 
{
  uint8_t r = ud_inp_next(u);
  if (!u->error) {
      inp_back(u); 
  }
  return r;
}


static inline size_t
inp_len(const struct ud *u)
{
  return u->inp_ctr;
}

#endif 
