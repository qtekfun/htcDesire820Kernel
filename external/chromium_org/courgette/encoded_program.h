// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_ENCODED_PROGRAM_H_
#define COURGETTE_ENCODED_PROGRAM_H_

#include <vector>

#include "base/basictypes.h"
#include "courgette/disassembler.h"
#include "courgette/memory_allocator.h"
#include "courgette/types_elf.h"

namespace courgette {

class SinkStream;
class SinkStreamSet;
class SourceStreamSet;

class EncodedProgram {
 public:
  EncodedProgram();
  ~EncodedProgram();

  
  
  
  void set_image_base(uint64 base) { image_base_ = base; }

  
  CheckBool DefineRel32Label(int index, RVA address) WARN_UNUSED_RESULT;
  CheckBool DefineAbs32Label(int index, RVA address) WARN_UNUSED_RESULT;
  void EndLabels();

  
  
  
  CheckBool AddOrigin(RVA rva) WARN_UNUSED_RESULT;
  CheckBool AddCopy(uint32 count, const void* bytes) WARN_UNUSED_RESULT;
  CheckBool AddRel32(int label_index) WARN_UNUSED_RESULT;
  CheckBool AddRel32ARM(uint16 op, int label_index) WARN_UNUSED_RESULT;
  CheckBool AddAbs32(int label_index) WARN_UNUSED_RESULT;
  CheckBool AddPeMakeRelocs(ExecutableType kind) WARN_UNUSED_RESULT;
  CheckBool AddElfMakeRelocs() WARN_UNUSED_RESULT;
  CheckBool AddElfARMMakeRelocs() WARN_UNUSED_RESULT;

  
  CheckBool WriteTo(SinkStreamSet* streams) WARN_UNUSED_RESULT;

  
  
  
  bool ReadFrom(SourceStreamSet* streams);

  
  CheckBool AssembleTo(SinkStream* buffer) WARN_UNUSED_RESULT;

 private:
  
  
  
  enum OP {
    ORIGIN = 0,    
    COPY = 1,      
    COPY1 = 2,     
    REL32 = 3,     
                   
    ABS32 = 4,     
                   
    MAKE_PE_RELOCATION_TABLE = 5,  
    MAKE_ELF_RELOCATION_TABLE = 6, 
    MAKE_ELF_ARM_RELOCATION_TABLE = 7, 
    MAKE_PE64_RELOCATION_TABLE = 8, 
    
    
    REL32ARM8   = 0x1000,
    REL32ARM11  = 0x2000,
    REL32ARM24  = 0x3000,
    REL32ARM25  = 0x4000,
    REL32ARM21  = 0x5000,
    LAST_ARM    = 0x5FFF,
  };

  typedef NoThrowBuffer<RVA> RvaVector;
  typedef NoThrowBuffer<uint32> UInt32Vector;
  typedef NoThrowBuffer<uint8> UInt8Vector;
  typedef NoThrowBuffer<OP> OPVector;

  void DebuggingSummary();
  CheckBool GeneratePeRelocations(SinkStream *buffer,
                                  uint8 type) WARN_UNUSED_RESULT;
  CheckBool GenerateElfRelocations(Elf32_Word pending_elf_relocation_table,
                                   SinkStream *buffer) WARN_UNUSED_RESULT;
  CheckBool DefineLabelCommon(RvaVector*, int, RVA) WARN_UNUSED_RESULT;
  void FinishLabelsCommon(RvaVector* addresses);

  
  CheckBool EvaluateRel32ARM(OP op, size_t& ix_rel32_ix, RVA& current_rva,
                             SinkStream* output);

  
  uint64 image_base_;
  RvaVector rel32_rva_;
  RvaVector abs32_rva_;
  OPVector ops_;
  RvaVector origins_;
  UInt32Vector copy_counts_;
  UInt8Vector copy_bytes_;
  UInt32Vector rel32_ix_;
  UInt32Vector abs32_ix_;

  
  
  UInt32Vector abs32_relocs_;

  DISALLOW_COPY_AND_ASSIGN(EncodedProgram);
};

}  
#endif  
