/* Copyright (c) 2007, Google Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Joi Sigurdsson
 *
 * Definition of MiniDisassembler.
 */

#ifndef GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_H_
#define GOOGLE_PERFTOOLS_MINI_DISASSEMBLER_H_

#include "config.h"
#include <windows.h>
#include "mini_disassembler_types.h"

#include "base/logging.h"
#define SIDESTEP_ASSERT(cond)  RAW_DCHECK(cond, #cond)
#define SIDESTEP_LOG(msg)      RAW_VLOG(1, msg)

namespace sidestep {

class PERFTOOLS_DLL_DECL MiniDisassembler {
 public:

  
  
  
  
  
  
  MiniDisassembler(bool operand_default_32_bits,
                   bool address_default_32_bits);

  
  MiniDisassembler();

  
  
  
  
  
  
  
  
  
  
  
  
  
  InstructionType Disassemble(unsigned char* start, unsigned int& instruction_bytes);

 private:

  
  void Initialize();

  
  
  InstructionType ProcessPrefixes(unsigned char* start, unsigned int& size);

  
  
  
  InstructionType ProcessOpcode(unsigned char* start,
                                unsigned int table,
                                unsigned int& size);

  
  
  
  
  bool ProcessOperand(int flag_operand);

  
  
  
  
  bool ProcessModrm(unsigned char* start, unsigned int& size);

  
  
  
  
  bool ProcessSib(unsigned char* start, unsigned char mod, unsigned int& size);

  
  InstructionType instruction_type_;

  
  
  
  unsigned int operand_bytes_;

  
  bool have_modrm_;

  
  
  bool should_decode_modrm_;

  
  bool operand_is_32_bits_;

  
  bool operand_default_is_32_bits_;

  
  bool address_is_32_bits_;

  
  bool address_default_is_32_bits_;

  
  bool operand_default_support_64_bits_;

  
  bool operand_is_64_bits_;

  
  
  static const OpcodeTable s_ia32_opcode_map_[];

  
  
  
  static const ModrmEntry s_ia16_modrm_map_[];

  
  
  
  static const ModrmEntry s_ia32_modrm_map_[];

  
  
  
  bool got_f2_prefix_, got_f3_prefix_, got_66_prefix_;
};

};  

#endif  
