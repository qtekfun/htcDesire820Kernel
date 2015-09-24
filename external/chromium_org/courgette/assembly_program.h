// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_ASSEMBLY_PROGRAM_H_
#define COURGETTE_ASSEMBLY_PROGRAM_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

#include "courgette/disassembler.h"
#include "courgette/memory_allocator.h"

namespace courgette {

class EncodedProgram;
class Instruction;

typedef NoThrowBuffer<Instruction*> InstructionVector;

class Label {
 public:
  static const int kNoIndex = -1;
  Label() : rva_(0), index_(kNoIndex), count_(0) {}
  explicit Label(RVA rva) : rva_(rva), index_(kNoIndex), count_(0) {}

  RVA rva_;    
  int index_;  
  int count_;
};

typedef std::map<RVA, Label*> RVAToLabel;

class AssemblyProgram {
 public:
  explicit AssemblyProgram(ExecutableType kind);
  ~AssemblyProgram();

  ExecutableType kind() const { return kind_; }

  void set_image_base(uint64 image_base) { image_base_ = image_base; }

  

  
  CheckBool EmitPeRelocsInstruction() WARN_UNUSED_RESULT;

  
  CheckBool EmitElfRelocationInstruction() WARN_UNUSED_RESULT;

  
  CheckBool EmitElfARMRelocationInstruction() WARN_UNUSED_RESULT;

  
  CheckBool EmitOriginInstruction(RVA rva) WARN_UNUSED_RESULT;

  
  CheckBool EmitByteInstruction(uint8 byte) WARN_UNUSED_RESULT;

  
  CheckBool EmitBytesInstruction(const uint8* value, uint32 len)
      WARN_UNUSED_RESULT;

  
  CheckBool EmitRel32(Label* label) WARN_UNUSED_RESULT;

  
  
  CheckBool EmitRel32ARM(uint16 op, Label* label, const uint8* arm_op,
                         uint16 op_size) WARN_UNUSED_RESULT;

  
  CheckBool EmitAbs32(Label* label) WARN_UNUSED_RESULT;

  
  Label* FindOrMakeAbs32Label(RVA rva);

  
  Label* FindOrMakeRel32Label(RVA rva);

  void DefaultAssignIndexes();
  void UnassignIndexes();
  void AssignRemainingIndexes();

  EncodedProgram* Encode() const;

  
  const InstructionVector& instructions() const {
    return instructions_;
  }

  
  
  Label* InstructionAbs32Label(const Instruction* instruction) const;

  
  
  Label* InstructionRel32Label(const Instruction* instruction) const;

  
  CheckBool TrimLabels();

  void PrintLabelCounts(RVAToLabel* labels);
  void CountRel32ARM();

 private:
  ExecutableType kind_;

  CheckBool Emit(Instruction* instruction) WARN_UNUSED_RESULT;

  static const int kLabelLowerLimit;

  
  Label* FindLabel(RVA rva, RVAToLabel* labels);

  
  static void UnassignIndexes(RVAToLabel* labels);
  static void DefaultAssignIndexes(RVAToLabel* labels);
  static void AssignRemainingIndexes(RVAToLabel* labels);

  
  Instruction* GetByteInstruction(uint8 byte);
  scoped_ptr<Instruction*[]> byte_instruction_cache_;

  uint64 image_base_;  

  InstructionVector instructions_;  

  
  
  
  RVAToLabel rel32_labels_;
  RVAToLabel abs32_labels_;

  DISALLOW_COPY_AND_ASSIGN(AssemblyProgram);
};

}  
#endif  
