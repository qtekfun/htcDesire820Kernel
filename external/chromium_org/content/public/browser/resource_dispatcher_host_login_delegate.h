// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_LOGIN_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_RESOURCE_DISPATCHER_HOST_LOGIN_DELEGATE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT ResourceDispatcherHostLoginDelegate
    : public base::RefCountedThreadSafe<ResourceDispatcherHostLoginDelegate> {
 public:
  
  
  virtual void OnRequestCancelled() = 0;

 protected:
  friend class base::RefCountedThreadSafe<ResourceDispatcherHostLoginDelegate>;
  virtual ~ResourceDispatcherHostLoginDelegate() {}
};

}  

#endif  
