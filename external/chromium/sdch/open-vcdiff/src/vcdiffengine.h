// Copyright 2006 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VCDIFFENGINE_H_
#define OPEN_VCDIFF_VCDIFFENGINE_H_

#include <config.h>
#include <stddef.h>  
#include <stdint.h>  

namespace open_vcdiff {

class BlockHash;
class OutputStringInterface;
class CodeTableWriterInterface;

class VCDiffEngine {
 public:
  
  
  
  
  static const size_t kMinimumMatchSize = 32;

  VCDiffEngine(const char* dictionary, size_t dictionary_size);

  ~VCDiffEngine();

  
  
  
  
  
  
  
  
  
  bool Init();

  size_t dictionary_size() const { return dictionary_size_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  void Encode(const char* target_data,
              size_t target_size,
              bool look_for_target_matches,
              OutputStringInterface* diff,
              CodeTableWriterInterface* coder) const;

 private:
  static bool ShouldGenerateCopyInstructionForMatchOfSize(size_t size) {
    return size >= kMinimumMatchSize;
  }

  
  
  
  
  template<bool look_for_target_matches>
  void EncodeInternal(const char* target_data,
                      size_t target_size,
                      OutputStringInterface* diff,
                      CodeTableWriterInterface* coder) const;

  
  
  
  template<bool look_for_target_matches>
  size_t EncodeCopyForBestMatch(uint32_t hash_value,
                                const char* target_candidate_start,
                                const char* unencoded_target_start,
                                size_t unencoded_target_size,
                                const BlockHash* target_hash,
                                CodeTableWriterInterface* coder) const;

  void AddUnmatchedRemainder(const char* unencoded_target_start,
                             size_t unencoded_target_size,
                             CodeTableWriterInterface* coder) const;

  void FinishEncoding(size_t target_size,
                      OutputStringInterface* diff,
                      CodeTableWriterInterface* coder) const;

  const char* dictionary_;  

  const size_t dictionary_size_;

  
  
  
  const BlockHash* hashed_dictionary_;

  
  VCDiffEngine(const VCDiffEngine&);
  void operator=(const VCDiffEngine&);
};

}  

#endif  
