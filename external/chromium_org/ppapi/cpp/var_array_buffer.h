// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_ARRAY_BUFFER_H_
#define PPAPI_CPP_VAR_ARRAY_BUFFER_H_

#include "ppapi/cpp/var.h"


namespace pp {

class VarArrayBuffer : public Var {
 public:
  
  
  VarArrayBuffer();

  
  
  
  
  
  
  explicit VarArrayBuffer(const Var& var);

  
  
  
  
  
  explicit VarArrayBuffer(uint32_t size_in_bytes);

  
  VarArrayBuffer(const VarArrayBuffer& buffer) : Var(buffer) {}

  virtual ~VarArrayBuffer() {}

  
  
  
  
  
  
  VarArrayBuffer& operator=(const VarArrayBuffer& other);

  
  
  
  
  
  
  
  
  virtual Var& operator=(const Var& other);

  
  
  
  
  uint32_t ByteLength() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void* Map();

  
  
  
  void Unmap();

 private:
  void ConstructWithSize(uint32_t size_in_bytes);
};

}  

#endif  
