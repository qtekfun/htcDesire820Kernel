// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_HEADERPARSER_H_
#define OPEN_VCDIFF_HEADERPARSER_H_

#include <config.h>
#include <stddef.h>  
#include <stdint.h>  
#include "checksum.h"  
#include "vcdiff_defs.h"  

namespace open_vcdiff {

class ParseableChunk {
 public:
  ParseableChunk(const char* data_start, size_t data_size) {
    SetDataBuffer(data_start, data_size);
  }

  const char* End() const { return end_; }

  
  
  
  size_t UnparsedSize() const {
    return end_ - position_;
  }

  
  size_t ParsedSize() const {
    return position_ - start_;
  }

  bool Empty() const { return 0 == UnparsedSize(); }

  
  const char* UnparsedData() const { return position_; }

  
  const char** UnparsedDataAddr() { return &position_; }

  
  void Advance(size_t number_of_bytes);

  
  void SetPosition(const char* position);

  
  void Finish() {
    position_ = end_;
  }

  
  
  
  void FinishExcept(size_t number_of_bytes);

  void SetDataBuffer(const char* data_start, size_t data_size) {
    start_ = data_start;
    end_ = data_start + data_size;
    position_ = start_;
  }

 private:
  const char* start_;
  const char* end_;

  
  
  const char* position_;

  
  ParseableChunk(const ParseableChunk&);
  void operator=(const ParseableChunk&);
};

class DeltaWindowSection {
 public:
  DeltaWindowSection() : parseable_chunk_(NULL), owned_(true) { }

  ~DeltaWindowSection() {
    FreeChunk();
  }

  void Init(const char* data_start, size_t data_size) {
    if (owned_ && parseable_chunk_) {
      
      parseable_chunk_->SetDataBuffer(data_start, data_size);
    } else {
      parseable_chunk_ = new ParseableChunk(data_start, data_size);
      owned_ = true;
    }
  }

  void Init(DeltaWindowSection* original) {
    FreeChunk();
    parseable_chunk_ = original->parseable_chunk_;
    owned_ = false;
  }

  void Invalidate() { FreeChunk(); }

  bool IsOwned() const { return owned_; }

  
  

  const char* End() const {
    return parseable_chunk_->End();
  }

  size_t UnparsedSize() const {
    return parseable_chunk_->UnparsedSize();
  }

  size_t ParsedSize() const {
    return parseable_chunk_->ParsedSize();
  }

  bool Empty() const {
    return parseable_chunk_->Empty();
  }

  const char* UnparsedData() const {
    return parseable_chunk_->UnparsedData();
  }

  const char** UnparsedDataAddr() {
    return parseable_chunk_->UnparsedDataAddr();
  }

  void Advance(size_t number_of_bytes) {
    return parseable_chunk_->Advance(number_of_bytes);
  }
 private:
  void FreeChunk() {
    if (owned_) {
      delete parseable_chunk_;
    }
    parseable_chunk_ = NULL;
  }

  
  
  
  
  
  
  
  ParseableChunk* parseable_chunk_;
  bool owned_;

  
  DeltaWindowSection(const DeltaWindowSection&);
  void operator=(const DeltaWindowSection&);
};

class VCDiffHeaderParser {
 public:
  
  
  
  VCDiffHeaderParser(const char* header_start, const char* data_end);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ParseByte(unsigned char* value);
  bool ParseInt32(const char* variable_description, int32_t* value);
  bool ParseUInt32(const char* variable_description, uint32_t* value);
  bool ParseChecksum(const char* variable_description, VCDChecksum* value);
  bool ParseSize(const char* variable_description, size_t* value);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ParseWinIndicatorAndSourceSegment(size_t dictionary_size,
                                         size_t decoded_target_size,
                                         bool allow_vcd_target,
                                         unsigned char* win_indicator,
                                         size_t* source_segment_length,
                                         size_t* source_segment_position);

  
  
  
  
  
  
  
  
  bool ParseWindowLengths(size_t* target_window_length);

  
  
  
  
  const char* EndOfDeltaWindow() const;

  
  
  
  
  
  
  
  
  
  bool ParseDeltaIndicator();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ParseSectionLengths(bool has_checksum,
                           size_t* add_and_run_data_length,
                           size_t* instructions_and_sizes_length,
                           size_t* addresses_length,
                           VCDChecksum* checksum);

  
  
  
  
  VCDiffResult GetResult() const {
    return return_code_;
  }

  
  

  const char* End() const {
    return parseable_chunk_.End();
  }

  size_t UnparsedSize() const {
    return parseable_chunk_.UnparsedSize();
  }

  size_t ParsedSize() const {
    return parseable_chunk_.ParsedSize();
  }

  const char* UnparsedData() const {
    return parseable_chunk_.UnparsedData();
  }

 private:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ParseSourceSegmentLengthAndPosition(size_t from_size,
                                           const char* from_boundary_name,
                                           const char* from_name,
                                           size_t* source_segment_length,
                                           size_t* source_segment_position);

  ParseableChunk parseable_chunk_;

  
  
  
  
  VCDiffResult return_code_;

  
  
  
  size_t delta_encoding_length_;

  
  
  
  
  
  const char* delta_encoding_start_;

  
  VCDiffHeaderParser(const VCDiffHeaderParser&);
  void operator=(const VCDiffHeaderParser&);
};

}  

#endif  
