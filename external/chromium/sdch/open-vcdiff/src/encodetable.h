// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_ENCODETABLE_H_
#define OPEN_VCDIFF_ENCODETABLE_H_

#include <config.h>
#include <stddef.h>  
#include <stdint.h>  
#include <string>
#include <vector>
#include "addrcache.h"
#include "checksum.h"
#include "codetable.h"
#include "codetablewriter_interface.h"

namespace open_vcdiff {

class OutputStringInterface;
class VCDiffInstructionMap;

class VCDiffCodeTableWriter : public CodeTableWriterInterface {
 public:
  
  
  
  
  
  
  
  
  
  
  explicit VCDiffCodeTableWriter(bool interleaved);

  
  
  
  
  
  
  
  
  
  VCDiffCodeTableWriter(bool interleaved,
                        int near_cache_size,
                        int same_cache_size,
                        const VCDiffCodeTableData& code_table_data,
                        unsigned char max_mode);

  virtual ~VCDiffCodeTableWriter();

  
  
  
  
  
  
  
  bool Init(size_t dictionary_size);

  virtual size_t target_length() const { return target_length_; }

  
  virtual void Add(const char* data, size_t size);

  
  virtual void Copy(int32_t offset, size_t size);

  
  virtual void Run(size_t size, unsigned char byte);

  void AddChecksum(VCDChecksum checksum) {
    add_checksum_ = true;
    checksum_ = checksum;
  }

  
  
  
  virtual void Output(OutputStringInterface* out);

  const std::vector<int>& match_counts() const { return match_counts_; }

 private:
  typedef std::string string;

  
  
  
  
  
  static const size_t kMaxMatchSize = 2000;

  
  const unsigned char max_mode_;

  
  
  
  
  
  
  
  
  
  void InitSectionPointers(bool interleaved);

  
  
  
  
  void EncodeInstruction(VCDiffInstructionType inst,
                         size_t size,
                         unsigned char mode);

  void EncodeInstruction(VCDiffInstructionType inst, size_t size) {
    return EncodeInstruction(inst, size, 0);
  }

  
  
  static size_t CalculateLengthOfSizeAsVarint(size_t size);

  
  static void AppendSizeToString(size_t size, string* out);

  
  static void AppendSizeToOutputString(size_t size, OutputStringInterface* out);

  
  
  
  size_t CalculateLengthOfTheDeltaEncoding() const;

  

  
  
  
  string instructions_and_sizes_;

  
  
  
  
  string *data_for_add_and_run_;
  string separate_data_for_add_and_run_;

  
  
  
  
  
  string *addresses_for_copy_;
  string separate_addresses_for_copy_;

  VCDiffAddressCache address_cache_;

  size_t dictionary_size_;

  
  
  
  // for COPY instructions, and is also written into the header
  
  
  size_t target_length_;

  const VCDiffCodeTableData* code_table_data_;

  
  
  
  
  const VCDiffInstructionMap* instruction_map_;

  
  
  
  
  
  
  
  
  
  int last_opcode_index_;

  // If true, an Adler32 checksum of the target window data will be written as
  
  
  bool add_checksum_;

  // The checksum to be written to the current target window,
  
  
  
  
  
  
  
  
  VCDChecksum checksum_;

  
  
  std::vector<int> match_counts_;

  
  VCDiffCodeTableWriter(const VCDiffCodeTableWriter&);  
  void operator=(const VCDiffCodeTableWriter&);
};

};  

#endif  
