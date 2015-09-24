// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_VCDIFF_DEFS_H_
#define OPEN_VCDIFF_VCDIFF_DEFS_H_

#include <config.h>
#include <limits.h>             
#include <stdint.h>             

namespace open_vcdiff {

enum VCDiffResult {
  RESULT_SUCCESS = 0,
  
  
  
  
  RESULT_ERROR = -1,
  
  
  RESULT_END_OF_DATA = -2
};

typedef struct DeltaFileHeader {
  unsigned char header1;  
  unsigned char header2;  
  unsigned char header3;  
  unsigned char header4;  
  unsigned char hdr_indicator;
} DeltaFileHeader;

const unsigned char VCD_DECOMPRESS = 0x01;
const unsigned char VCD_CODETABLE = 0x02;

const unsigned char VCD_SOURCE = 0x01;
const unsigned char VCD_TARGET = 0x02;
const unsigned char VCD_CHECKSUM = 0x04;

const unsigned char VCD_DATACOMP = 0x01;
const unsigned char VCD_INSTCOMP = 0x02;
const unsigned char VCD_ADDRCOMP = 0x04;

typedef int32_t VCDAddress;

enum VCDiffModes {
  VCD_SELF_MODE = 0,
  VCD_HERE_MODE = 1,
  VCD_FIRST_NEAR_MODE = 2,
  VCD_MAX_MODES = UCHAR_MAX + 1  
};

}  

#endif  
