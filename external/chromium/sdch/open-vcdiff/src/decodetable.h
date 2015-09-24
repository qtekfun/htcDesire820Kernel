// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_DECODETABLE_H_
#define OPEN_VCDIFF_DECODETABLE_H_

#include <config.h>
#include <stddef.h>     
#include <stdint.h>     
#include <memory>       
#include "codetable.h"  
#include "logging.h"

namespace open_vcdiff {

class VCDiffCodeTableReader {
 public:
  
  
  
  
  
  
  VCDiffCodeTableReader();

  
  
  
  
  
  
  
  
  bool UseCodeTable(const VCDiffCodeTableData& code_table_data,
                    unsigned char max_mode);

  
  
  
  
  
  void Init(const char** instructions_and_sizes,
            const char* instructions_and_sizes_end) {
    instructions_and_sizes_ = instructions_and_sizes;
    instructions_and_sizes_end_ = instructions_and_sizes_end;
    last_instruction_start_ = NULL;
    pending_second_instruction_ = kNoOpcode;
    last_pending_second_instruction_ = kNoOpcode;
  }

  
  
  
  
  
  
  void UpdatePointers(const char** instructions_and_sizes,
                      const char* instructions_and_sizes_end) {
    instructions_and_sizes_ = instructions_and_sizes;
    instructions_and_sizes_end_ = instructions_and_sizes_end;
    last_instruction_start_ = *instructions_and_sizes;
    
    last_pending_second_instruction_ = pending_second_instruction_;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VCDiffInstructionType GetNextInstruction(int32_t* size, unsigned char* mode);

  
  
  
  
  
  
  
  void UnGetInstruction() {
    if (last_instruction_start_) {
      if (last_instruction_start_ > *instructions_and_sizes_) {
        LOG(DFATAL) << "Internal error: last_instruction_start past end of "
                       "instructions_and_sizes in UnGetInstruction" << LOG_ENDL;
      }
      *instructions_and_sizes_ = last_instruction_start_;
      if ((pending_second_instruction_ != kNoOpcode) &&
          (last_pending_second_instruction_ != kNoOpcode)) {
        LOG(DFATAL) << "Internal error: two pending instructions in a row "
                       "in UnGetInstruction" << LOG_ENDL;
      }
      pending_second_instruction_ = last_pending_second_instruction_;
    }
  }

 private:
  
  
  const VCDiffCodeTableData* code_table_data_;

  
  
  
  
  
  
  
  std::auto_ptr<VCDiffCodeTableData> non_default_code_table_data_;

  const char** instructions_and_sizes_;
  const char* instructions_and_sizes_end_;
  const char* last_instruction_start_;
  OpcodeOrNone pending_second_instruction_;
  OpcodeOrNone last_pending_second_instruction_;

  
  VCDiffCodeTableReader(const VCDiffCodeTableReader&);
  void operator=(const VCDiffCodeTableReader&);
};

};  

#endif  
