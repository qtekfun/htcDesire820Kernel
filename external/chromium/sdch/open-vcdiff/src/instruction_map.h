// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_INSTRUCTION_MAP_H_
#define OPEN_VCDIFF_INSTRUCTION_MAP_H_

#include <config.h>
#include "codetable.h"
#include "vcdiff_defs.h"

namespace open_vcdiff {

class VCDiffInstructionMap {
 public:
  
  
  
  
  
  
  
  VCDiffInstructionMap(const VCDiffCodeTableData& code_table_data,
                       unsigned char max_mode);

  static VCDiffInstructionMap* GetDefaultInstructionMap();

  
  
  
  
  
  
  
  
  
  
  
  
  
  OpcodeOrNone LookupFirstOpcode(unsigned char inst,
                                 unsigned char size,
                                 unsigned char mode) const {
    return first_instruction_map_.Lookup(inst, size, mode);
  }

  
  
  
  
  
  
  
  
  
  OpcodeOrNone LookupSecondOpcode(unsigned char first_opcode,
                                  unsigned char inst,
                                  unsigned char size,
                                  unsigned char mode) const {
    return second_instruction_map_.Lookup(first_opcode, inst, size, mode);
  }

 private:
  
  
  class FirstInstructionMap {
   public:
    FirstInstructionMap(int num_insts_and_modes, int max_size_1);
    ~FirstInstructionMap();

    void Add(unsigned char inst,
             unsigned char size,
             unsigned char mode,
             unsigned char opcode) {
      OpcodeOrNone* opcode_slot = &first_opcodes_[inst + mode][size];
      if (*opcode_slot == kNoOpcode) {
        *opcode_slot = opcode;
      }
    }

    
    
    OpcodeOrNone Lookup(unsigned char inst,
                        unsigned char size,
                        unsigned char mode) const {
      int inst_mode = (inst == VCD_COPY) ? (inst + mode) : inst;
      if (size > max_size_1_) {
        return kNoOpcode;
      }
      
      return first_opcodes_[inst_mode][size];
    }

   private:
    
    
    
    
    
    
    
    
    
    
    const int num_instruction_type_modes_;

    
    
    const int max_size_1_;

    
    
    
    
    
    
    
    
    
    
    
    OpcodeOrNone** first_opcodes_;

    
    
    FirstInstructionMap(const FirstInstructionMap&);  
    void operator=(const FirstInstructionMap&);
  } first_instruction_map_;

  
  
  class SecondInstructionMap {
   public:
    SecondInstructionMap(int num_insts_and_modes, int max_size_2);
    ~SecondInstructionMap();
    void Add(unsigned char first_opcode,
             unsigned char inst,
             unsigned char size,
             unsigned char mode,
             unsigned char second_opcode);

    
    OpcodeOrNone Lookup(unsigned char first_opcode,
                        unsigned char inst,
                        unsigned char size,
                        unsigned char mode) const;
   private:
    
    const int num_instruction_type_modes_;

    
    const int max_size_2_;

    
    
    
    
    
    
    
    
    
    
    
    OpcodeOrNone** second_opcodes_[VCDiffCodeTableData::kCodeTableSize];

    
    
    SecondInstructionMap(const SecondInstructionMap&);  
    void operator=(const SecondInstructionMap&);
  } second_instruction_map_;

  static VCDiffInstructionMap* default_instruction_map;

  
  VCDiffInstructionMap(const VCDiffInstructionMap&);  
  void operator=(const VCDiffInstructionMap&);
};

};  

#endif  
