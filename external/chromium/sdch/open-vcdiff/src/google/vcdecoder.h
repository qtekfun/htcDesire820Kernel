// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VCDECODER_H_
#define OPEN_VCDIFF_VCDECODER_H_

#include <stddef.h>  
#include <string>
#include "google/output_string.h"

namespace open_vcdiff {

class VCDiffStreamingDecoderImpl;

class VCDiffStreamingDecoder {
 public:
  VCDiffStreamingDecoder();
  ~VCDiffStreamingDecoder();

  
  
  
  
  
  void StartDecoding(const char* dictionary_ptr, size_t dictionary_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template<class OutputType>
  bool DecodeChunk(const char* data, size_t len, OutputType* output) {
    OutputString<OutputType> output_string(output);
    return DecodeChunkToInterface(data, len, &output_string);
  }

  bool DecodeChunkToInterface(const char* data, size_t len,
                              OutputStringInterface* output_string);

  
  
  
  
  
  bool FinishDecoding();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SetMaximumTargetFileSize(size_t new_maximum_target_file_size);

  
  
  
  
  bool SetMaximumTargetWindowSize(size_t new_maximum_target_window_size);

  
  
  
  
  
  
  void SetAllowVcdTarget(bool allow_vcd_target);

 private:
  VCDiffStreamingDecoderImpl* const impl_;

  
  
  explicit VCDiffStreamingDecoder(const VCDiffStreamingDecoder&);
  void operator=(const VCDiffStreamingDecoder&);
};

class VCDiffDecoder {
 public:
  typedef std::string string;

  VCDiffDecoder() { }
  ~VCDiffDecoder() { }

  

  
  
  
  
  
  
  template<class OutputType>
  bool Decode(const char* dictionary_ptr,
              size_t dictionary_size,
              const string& encoding,
              OutputType* target) {
    OutputString<OutputType> output_string(target);
    return DecodeToInterface(dictionary_ptr,
                             dictionary_size,
                             encoding,
                             &output_string);
  }

 private:
  bool DecodeToInterface(const char* dictionary_ptr,
                         size_t dictionary_size,
                         const string& encoding,
                         OutputStringInterface* target);

  VCDiffStreamingDecoder decoder_;

  
  
  explicit VCDiffDecoder(const VCDiffDecoder&);
  void operator=(const VCDiffDecoder&);
};

};  

#endif  
