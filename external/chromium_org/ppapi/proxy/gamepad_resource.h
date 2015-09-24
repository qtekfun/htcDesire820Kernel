// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_GAMEPAD_RESOURCE_H_
#define PPAPI_PROXY_GAMEPAD_RESOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "ppapi/c/ppb_gamepad.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/shared_impl/ppb_gamepad_shared.h"
#include "ppapi/thunk/ppb_gamepad_api.h"

struct PP_GamepadsSampleData;

namespace base {
class SharedMemory;
}

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT GamepadResource
      : public PluginResource,
        public thunk::PPB_Gamepad_API {
 public:
  GamepadResource(Connection connection, PP_Instance instance);
  virtual ~GamepadResource();

  
  virtual thunk::PPB_Gamepad_API* AsPPB_Gamepad_API() OVERRIDE;

  
  virtual void Sample(PP_Instance instance,
                      PP_GamepadsSampleData* data) OVERRIDE;

 private:
  void OnPluginMsgSendMemory(const ResourceMessageReplyParams& params);

  scoped_ptr<base::SharedMemory> shared_memory_;
  const ContentGamepadHardwareBuffer* buffer_;

  
  PP_GamepadsSampleData last_read_;

  DISALLOW_COPY_AND_ASSIGN(GamepadResource);
};

}  
}  

#endif  
