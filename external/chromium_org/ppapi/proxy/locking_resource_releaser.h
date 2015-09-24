// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_LOCKING_RESOURCE_RELEASER_H_
#define PPAPI_PROXY_LOCKING_RESOURCE_RELEASER_H_

#include "ppapi/shared_impl/ppapi_globals.h"
#include "ppapi/shared_impl/proxy_lock.h"
#include "ppapi/shared_impl/resource_tracker.h"

namespace ppapi {
namespace proxy {

class LockingResourceReleaser {
 public:
  explicit LockingResourceReleaser(PP_Resource resource)
      : resource_(resource) {
  }
  ~LockingResourceReleaser() {
    ProxyAutoLock lock;
    PpapiGlobals::Get()->GetResourceTracker()->ReleaseResource(resource_);
  }

  PP_Resource get() { return resource_; }

 private:
  PP_Resource resource_;

  DISALLOW_COPY_AND_ASSIGN(LockingResourceReleaser);
};

}  
}  

#endif  
