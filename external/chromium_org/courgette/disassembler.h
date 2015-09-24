// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_DISASSEMBLER_H_
#define COURGETTE_DISASSEMBLER_H_

#include "base/basictypes.h"

#include "courgette/courgette.h"

#define COURGETTE_HISTOGRAM_TARGETS 0

namespace courgette {

class AssemblyProgram;

typedef uint32 RVA;

class Disassembler {
 public:
  virtual ~Disassembler();

  virtual ExecutableType kind() { return EXE_UNKNOWN; }

  
  
  bool ok() const { return failure_reason_ == NULL; }

  
  
  virtual bool ParseHeader() = 0;

  
  
  virtual bool Disassemble(AssemblyProgram* program) = 0;

  
  size_t length() const { return length_; }
  const uint8* start() const { return start_; }
  const uint8* end() const { return end_; }

  
  
  const uint8* OffsetToPointer(size_t offset) const;

 protected:
  Disassembler(const void* start, size_t length);

  bool Good();
  bool Bad(const char *reason);

  
  uint16 ReadU16(const uint8* address, size_t offset) {
    return *reinterpret_cast<const uint16*>(address + offset);
  }

  uint32 ReadU32(const uint8* address, size_t offset) {
    return *reinterpret_cast<const uint32*>(address + offset);
  }

  uint64 ReadU64(const uint8* address, size_t offset) {
    return *reinterpret_cast<const uint64*>(address + offset);
  }

  static uint32 Read32LittleEndian(const void* address) {
    return *reinterpret_cast<const uint32*>(address);
  }

  static uint16 Read16LittleEndian(const void* address) {
    return *reinterpret_cast<const uint16*>(address);
  }

  
  
  void ReduceLength(size_t reduced_length);

 private:
  const char* failure_reason_;

  
  
  
  
  
  size_t length_;         
  const uint8* start_;    
  const uint8* end_;      

  DISALLOW_COPY_AND_ASSIGN(Disassembler);
};

}  
#endif  
