// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_RESOURCE_H_
#define PPAPI_CPP_RESOURCE_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/pass_ref.h"

namespace pp {

class VarResource_Dev;

class Resource {
 public:
  
  Resource();

  
  
  
  Resource(const Resource& other);

  
  virtual ~Resource();

  
  
  
  
  
  
  Resource& operator=(const Resource& other);

  
  
  
  
  bool is_null() const { return !pp_resource_; }

  PP_Resource pp_resource() const { return pp_resource_; }

  
  
  
  
  
  
  
  PP_Resource detach();

 protected:
  
  
  
  
  
  explicit Resource(PP_Resource resource);

  
  
  Resource(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PassRefFromConstructor(PP_Resource resource);

  
  void Clear();

 private:
  friend class VarResource_Dev;

  PP_Resource pp_resource_;
};

}  

inline bool operator==(const pp::Resource& lhs, const pp::Resource& rhs) {
  return lhs.pp_resource() == rhs.pp_resource();
}

inline bool operator!=(const pp::Resource& lhs, const pp::Resource& rhs) {
  return !(lhs == rhs);
}

#endif 
