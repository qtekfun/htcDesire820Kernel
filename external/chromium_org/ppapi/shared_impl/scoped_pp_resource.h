// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_SCOPED_RESOURCE_H_
#define PPAPI_SHARED_IMPL_SCOPED_RESOURCE_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class Resource;

class PPAPI_SHARED_EXPORT ScopedPPResource {
 public:
  struct PassRef {};

  ScopedPPResource();

  
  explicit ScopedPPResource(PP_Resource resource);

  
  explicit ScopedPPResource(const PassRef&, PP_Resource resource);

  
  
  explicit ScopedPPResource(Resource* resource);

  
  ScopedPPResource(const ScopedPPResource& other);

  ~ScopedPPResource();

  ScopedPPResource& operator=(PP_Resource resource);
  ScopedPPResource& operator=(const ScopedPPResource& resource);

  
  PP_Resource get() const { return id_; }
  operator PP_Resource() const { return id_; }

  
  
  PP_Resource Release();

 private:
  
  
  void CallAddRef();
  void CallRelease();

  PP_Resource id_;
};

}  

#endif  
