// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_CODETABLE_H_
#define OPEN_VCDIFF_CODETABLE_H_

#include <config.h>
#include <stdint.h>             

namespace open_vcdiff {

enum VCDiffInstructionType {
  VCD_NOOP = 0,
  VCD_ADD  = 1,
  VCD_RUN  = 2,
  VCD_COPY = 3,
  VCD_LAST_INSTRUCTION_TYPE = VCD_COPY,
  
  
  VCD_INSTRUCTION_ERROR = 4,
  VCD_INSTRUCTION_END_OF_DATA = 5
};

const char* VCDiffInstructionName(VCDiffInstructionType inst);

typedef uint16_t OpcodeOrNone;
const OpcodeOrNone kNoOpcode = 0x100;  

struct VCDiffCodeTableData {
  static const int kCodeTableSize = 256;

  static const VCDiffCodeTableData kDefaultCodeTableData;

  
  
  
  
  
  
  
  
  bool Validate(unsigned char max_mode) const;

  
  
  bool Validate() const;

  
  
  
  
  
  
  
  
  
  unsigned char inst1[kCodeTableSize];  
  unsigned char inst2[kCodeTableSize];  
  unsigned char size1[kCodeTableSize];
  unsigned char size2[kCodeTableSize];
  unsigned char mode1[kCodeTableSize];  
  unsigned char mode2[kCodeTableSize];  

 private:
  
  
  static const VCDiffInstructionType N = VCD_NOOP;
  static const VCDiffInstructionType A = VCD_ADD;
  static const VCDiffInstructionType R = VCD_RUN;
  static const VCDiffInstructionType C = VCD_COPY;

  static bool ValidateOpcode(int opcode,
                             unsigned char inst,
                             unsigned char size,
                             unsigned char mode,
                             unsigned char max_mode,
                             const char* first_or_second);
};

}  

#endif  
