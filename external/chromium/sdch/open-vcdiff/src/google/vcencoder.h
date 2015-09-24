// Copyright 2007 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VCENCODER_H_
#define OPEN_VCDIFF_VCENCODER_H_

#include <stddef.h>  
#include <vector>
#include "google/output_string.h"

namespace open_vcdiff {

class VCDiffEngine;
class VCDiffStreamingEncoderImpl;

enum VCDiffFormatExtensionFlagValues {
  
  
  VCD_STANDARD_FORMAT = 0x00,
  
  
  
  
  
  
  VCD_FORMAT_INTERLEAVED = 0x01,
  
  
  VCD_FORMAT_CHECKSUM = 0x02
};

typedef int VCDiffFormatExtensionFlags;

class HashedDictionary {
 public:
  HashedDictionary(const char* dictionary_contents,
                   size_t dictionary_size);
  ~HashedDictionary();

  
  
  
  
  
  bool Init();

  const VCDiffEngine* engine() const { return engine_; }

 private:
  const VCDiffEngine* engine_;

  
  
  HashedDictionary(const HashedDictionary&);  
  void operator=(const HashedDictionary&);
};

class VCDiffStreamingEncoder {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VCDiffStreamingEncoder(const HashedDictionary* dictionary,
                         VCDiffFormatExtensionFlags format_extensions,
                         bool look_for_target_matches);
  ~VCDiffStreamingEncoder();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  template<class OutputType>
  bool StartEncoding(OutputType* output) {
    OutputString<OutputType> output_string(output);
    return StartEncodingToInterface(&output_string);
  }

  bool StartEncodingToInterface(OutputStringInterface* output_string);

  
  
  
  
  
  
  
  template<class OutputType>
  bool EncodeChunk(const char* data, size_t len, OutputType* output) {
    OutputString<OutputType> output_string(output);
    return EncodeChunkToInterface(data, len, &output_string);
  }

  bool EncodeChunkToInterface(const char* data, size_t len,
                              OutputStringInterface* output_string);

  
  
  
  
  
  
  template<class OutputType>
  bool FinishEncoding(OutputType* output) {
    OutputString<OutputType> output_string(output);
    return FinishEncodingToInterface(&output_string);
  }

  bool FinishEncodingToInterface(OutputStringInterface* output_string);

  
  
  
  
  void GetMatchCounts(std::vector<int>* match_counts) const;

 private:
  VCDiffStreamingEncoderImpl* const impl_;

  
  
  VCDiffStreamingEncoder(const VCDiffStreamingEncoder&);  
  void operator=(const VCDiffStreamingEncoder&);
};

class VCDiffEncoder {
 public:
  VCDiffEncoder(const char* dictionary_contents, size_t dictionary_size)
      : dictionary_(dictionary_contents, dictionary_size),
        encoder_(NULL),
        flags_(VCD_STANDARD_FORMAT),
        look_for_target_matches_(true) { }

  ~VCDiffEncoder() {
    delete encoder_;
  }

  
  
  
  void SetFormatFlags(VCDiffFormatExtensionFlags flags) { flags_ = flags; }

  
  
  
  
  void SetTargetMatching(bool look_for_target_matches) {
    look_for_target_matches_ = look_for_target_matches;
  }

  
  
  template<class OutputType>
  bool Encode(const char* target_data,
              size_t target_len,
              OutputType* output) {
    OutputString<OutputType> output_string(output);
    return EncodeToInterface(target_data, target_len, &output_string);
  }

 private:
  bool EncodeToInterface(const char* target_data,
                         size_t target_len,
                         OutputStringInterface* output_string);

  HashedDictionary dictionary_;
  VCDiffStreamingEncoder* encoder_;
  VCDiffFormatExtensionFlags flags_;
  bool look_for_target_matches_;

  
  
  VCDiffEncoder(const VCDiffEncoder&);  
  void operator=(const VCDiffEncoder&);
};

}  

#endif  
