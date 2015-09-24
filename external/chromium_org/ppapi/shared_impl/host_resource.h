// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_HOST_RESOURCE_H_
#define PPAPI_SHARED_IMPL_HOST_RESOURCE_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

class PPAPI_SHARED_EXPORT HostResource {
 public:
  HostResource();

  bool is_null() const {
    return !host_resource_;
  }

  
  
  
  
  static HostResource MakeInstanceOnly(PP_Instance instance);

  
  
  
  
  
  void SetHostResource(PP_Instance instance, PP_Resource resource);
  PP_Resource host_resource() const {
    return host_resource_;
  }

  PP_Instance instance() const { return instance_; }

  
  bool operator<(const HostResource& other) const {
    if (instance_ != other.instance_)
      return instance_ < other.instance_;
    return host_resource_ < other.host_resource_;
  }

 private:
  PP_Instance instance_;
  PP_Resource host_resource_;
};

}  

#endif  
