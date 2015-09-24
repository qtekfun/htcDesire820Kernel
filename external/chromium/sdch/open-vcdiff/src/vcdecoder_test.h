// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VCDECODER_TEST_H_
#define OPEN_VCDIFF_VCDECODER_TEST_H_

#include "google/vcdecoder.h"
#include <string>
#include "checksum.h"
#include "testing.h"

namespace open_vcdiff {

// overwritten by each specific decoder test as needed.
class VCDiffDecoderTest : public testing::Test {
 protected:
  typedef std::string string;

  static const char kDictionary[];
  static const char kExpectedTarget[];

  VCDiffDecoderTest();

  virtual ~VCDiffDecoderTest() {}

  virtual void SetUp();

  
  
  void UseStandardFileHeader();
  void UseInterleavedFileHeader();

  
  
  
  
  
  virtual void InitializeDeltaFile();

  
  void AddChecksum(VCDChecksum checksum);

  
  
  void ComputeAndAddChecksum();

  
  
  void WriteMaxVarintAtOffset(int offset, int bytes_to_replace);

  
  
  void WriteNegativeVarintAtOffset(int offset, int bytes_to_replace);

  
  
  void WriteInvalidVarintAtOffset(int offset, int bytes_to_replace);

  
  
  
  
  
  bool FuzzOneByteInDeltaFile();

  
  
  
  static char GetByteFromStringLength(const char* s, int which_byte);

  
  
  static char StringLengthAsByte(const char* s) {
    return GetByteFromStringLength(s, 0);
  }

  
  
  static char FirstByteOfStringLength(const char* s) {
    return GetByteFromStringLength(s, 0);
  }

  
  
  static char SecondByteOfStringLength(const char* s) {
    return GetByteFromStringLength(s, 1);
  }

  VCDiffStreamingDecoder decoder_;

  
  
  string delta_file_;

  
  
  string output_;

  
  
  
  string dictionary_;
  string expected_target_;

  
  string delta_file_header_;
  string delta_window_header_;
  string delta_window_body_;

 private:
  
  
  static const char kStandardFileHeader[];
  static const char kInterleavedFileHeader[];

  
  
  size_t fuzzer_;
  size_t fuzzed_byte_position_;
};

class VCDiffStandardDecoderTest : public VCDiffDecoderTest {
 protected:
  VCDiffStandardDecoderTest();
  virtual ~VCDiffStandardDecoderTest() {}

 private:
  static const char kWindowHeader[];
  static const char kWindowBody[];
};

class VCDiffInterleavedDecoderTest : public VCDiffDecoderTest {
 protected:
  VCDiffInterleavedDecoderTest();
  virtual ~VCDiffInterleavedDecoderTest() {}

 private:
  static const char kWindowHeader[];
  static const char kWindowBody[];
};

}  

#endif  
