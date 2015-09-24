// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_ARRAY_H_
#define PPAPI_CPP_VAR_ARRAY_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/var.h"


namespace pp {

class VarArray : public Var {
 public:
  
  VarArray();

  
  
  
  
  
  explicit VarArray(const Var& var);

  
  
  
  
  explicit VarArray(const PP_Var& var);

  
  VarArray(const VarArray& other);

  virtual ~VarArray();

  
  VarArray& operator=(const VarArray& other);

  
  
  
  
  
  
  virtual Var& operator=(const Var& other);

  
  
  
  
  
  
  Var Get(uint32_t index) const;

  
  
  
  
  
  
  
  
  
  
  bool Set(uint32_t index, const Var& value);

  
  
  
  uint32_t GetLength() const;

  
  
  
  
  
  
  
  
  
  bool SetLength(uint32_t length);
};

}  

#endif  
