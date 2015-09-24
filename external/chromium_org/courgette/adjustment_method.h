// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_ADJUSTMENT_METHOD_H_
#define COURGETTE_ADJUSTMENT_METHOD_H_

#include "base/basictypes.h"

namespace courgette {

class AssemblyProgram;

class AdjustmentMethod {
 public:
  

  
  static AdjustmentMethod* MakeProductionAdjustmentMethod() {
    return MakeShingleAdjustmentMethod();
  }

  
  static AdjustmentMethod* MakeNullAdjustmentMethod();

  
  static AdjustmentMethod* MakeTrieAdjustmentMethod();

  
  static AdjustmentMethod* MakeShingleAdjustmentMethod();

  

  
  
  
  virtual bool Adjust(const AssemblyProgram& model,
                      AssemblyProgram* program) = 0;

  
  virtual void Destroy();

 protected:
  AdjustmentMethod() {}
  virtual ~AdjustmentMethod() {}

  DISALLOW_COPY_AND_ASSIGN(AdjustmentMethod);
};

}  
#endif  
