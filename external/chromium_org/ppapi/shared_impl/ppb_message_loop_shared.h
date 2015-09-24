// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPB_MESSAGE_LOOP_SHARED_H_
#define PPAPI_SHARED_IMPL_PPB_MESSAGE_LOOP_SHARED_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/location.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"
#include "ppapi/shared_impl/resource.h"
#include "ppapi/thunk/ppb_message_loop_api.h"

namespace base {
class MessageLoopProxy;
}

namespace tracked_objects {
class Location;
}

namespace ppapi {

class PPAPI_SHARED_EXPORT MessageLoopShared
    : public Resource,
      public thunk::PPB_MessageLoop_API {
 public:
  explicit MessageLoopShared(PP_Instance instance);
  
  
  struct ForMainThread {};
  explicit MessageLoopShared(ForMainThread);
  virtual ~MessageLoopShared();

  
  
  
  
  
  virtual void PostClosure(const tracked_objects::Location& from_here,
                           const base::Closure& closure,
                           int64 delay_ms) = 0;

  virtual base::MessageLoopProxy* GetMessageLoopProxy() = 0;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopShared);
};

}  

#endif  
