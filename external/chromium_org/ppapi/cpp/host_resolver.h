// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_HOST_RESOLVER_H_
#define PPAPI_CPP_HOST_RESOLVER_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_host_resolver.h"
#include "ppapi/cpp/net_address.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace pp {

class CompletionCallback;
class InstanceHandle;

class HostResolver : public Resource {
 public:
  
  
  HostResolver();

  
  
  
  
  explicit HostResolver(const InstanceHandle& instance);

  
  
  
  
  HostResolver(PassRef, PP_Resource resource);

  
  
  
  HostResolver(const HostResolver& other);

  
  virtual ~HostResolver();

  
  
  
  
  
  HostResolver& operator=(const HostResolver& other);

  
  
  
  
  static bool IsAvailable();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Resolve(const char* host,
                  uint16_t port,
                  const PP_HostResolver_Hint& hint,
                  const CompletionCallback& callback);

  
  
  
  
  
  
  
  Var GetCanonicalName() const;

  
  
  
  
  
  uint32_t GetNetAddressCount() const;

  
  
  
  
  
  
  
  
  NetAddress GetNetAddress(uint32_t index) const;
};

}  

#endif  
