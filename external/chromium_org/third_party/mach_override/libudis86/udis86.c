/* udis86 - libudis86/udis86.c
 *
 * Copyright (c) 2002-2013 Vivek Thampi
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

#include "input.h"
#include "extern.h"
#include "decode.h"

#if !defined(__UD_STANDALONE__)
# if HAVE_STRING_H
#  include <string.h>
# endif
#endif 

extern void 
ud_init(struct ud* u)
{
  memset((void*)u, 0, sizeof(struct ud));
  ud_set_mode(u, 16);
  u->mnemonic = UD_Iinvalid;
  ud_set_pc(u, 0);
#ifndef __UD_STANDALONE__
  ud_set_input_file(u, stdin);
#endif 

  ud_set_asm_buffer(u, u->asm_buf_int, sizeof(u->asm_buf_int));
}

extern unsigned int
ud_disassemble(struct ud* u)
{
  if (ud_input_end(u))
  return 0;

  u->asm_buf[0] = 0;
 
  if (ud_decode(u) == 0)
  return 0;
  if (u->translator)
  u->translator(u);
  return ud_insn_len(u);
}

extern void 
ud_set_mode(struct ud* u, uint8_t m)
{
  switch(m) {
  case 16:
  case 32:
  case 64: u->dis_mode = m ; return;
  default: u->dis_mode = 16; return;
  }
}

extern void 
ud_set_vendor(struct ud* u, unsigned v)
{
  switch(v) {
  case UD_VENDOR_INTEL:
    u->vendor = v;
    break;
  case UD_VENDOR_ANY:
    u->vendor = v;
    break;
  default:
    u->vendor = UD_VENDOR_AMD;
  }
}

extern void 
ud_set_pc(struct ud* u, uint64_t o)
{
  u->pc = o;
}

extern void 
ud_set_syntax(struct ud* u, void (*t)(struct ud*))
{
  u->translator = t;
}

const char* 
ud_insn_asm(const struct ud* u) 
{
  return u->asm_buf;
}

uint64_t
ud_insn_off(const struct ud* u) 
{
  return u->insn_offset;
}


const char* 
ud_insn_hex(struct ud* u) 
{
  u->insn_hexcode[0] = 0;
  if (!u->error) {
    unsigned int i;
    unsigned char *src_ptr = inp_sess(u);
    char* src_hex;
    src_hex = (char*) u->insn_hexcode;
    
    for (i = 0; i < u->inp_ctr && i < sizeof(u->insn_hexcode) / 2;
         ++i, ++src_ptr) {
      sprintf(src_hex, "%02x", *src_ptr & 0xFF);
      src_hex += 2;
    }
  }
  return u->insn_hexcode;
}


extern const uint8_t* 
ud_insn_ptr(const struct ud* u) 
{
  return u->inp_sess;
}

extern unsigned int 
ud_insn_len(const struct ud* u) 
{
  return u->inp_ctr;
}


const struct ud_operand*
ud_insn_opr(const struct ud *u, unsigned int n)
{
  if (n > 2 || u->operand[n].type == UD_NONE) {
    return NULL; 
  } else {
    return &u->operand[n];
  }
}


int
ud_opr_is_sreg(const struct ud_operand *opr)
{
  return opr->type == UD_OP_REG && 
         opr->base >= UD_R_ES   &&
         opr->base <= UD_R_GS;
}


int
ud_opr_is_gpr(const struct ud_operand *opr)
{
  return opr->type == UD_OP_REG && 
         opr->base >= UD_R_AL   &&
         opr->base <= UD_R_R15;
}


void
ud_set_user_opaque_data(struct ud * u, void* opaque)
{
  u->user_opaque_data = opaque;
}

void*
ud_get_user_opaque_data(const struct ud *u)
{
  return u->user_opaque_data;
}


void
ud_set_asm_buffer(struct ud *u, char *buf, size_t size)
{
  if (buf == NULL) {
    ud_set_asm_buffer(u, u->asm_buf_int, sizeof(u->asm_buf_int));
  } else {
    u->asm_buf = buf;
    u->asm_buf_size = size;
  }
}


void
ud_set_sym_resolver(struct ud *u, const char* (*resolver)(struct ud*, 
                                                          uint64_t addr,
                                                          int64_t *offset))
{
  u->sym_resolver = resolver;
}

