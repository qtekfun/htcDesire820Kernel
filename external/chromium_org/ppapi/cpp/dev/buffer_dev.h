// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_BUFFER_DEV_H_
#define PPAPI_CPP_DEV_BUFFER_DEV_H_

#include "ppapi/cpp/resource.h"

namespace pp {

class InstanceHandle;

class Buffer_Dev : public Resource {
 public:
  
  Buffer_Dev();
  Buffer_Dev(const Buffer_Dev& other);
  explicit Buffer_Dev(PP_Resource resource);

  
  
  Buffer_Dev(const InstanceHandle& instance, uint32_t size);

  
  
  Buffer_Dev(PassRef, PP_Resource resource);

  
  virtual ~Buffer_Dev();

  Buffer_Dev& operator=(const Buffer_Dev& rhs);

  uint32_t size() const { return size_; }
  void* data() const { return data_; }

 private:
  void Init();

  void* data_;
  uint32_t size_;
};

}  

#endif  
