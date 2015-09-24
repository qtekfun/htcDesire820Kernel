// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_FLASH_DRM_H_
#define PPAPI_CPP_PRIVATE_FLASH_DRM_H_

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class FileRef;

namespace flash {

class DRM : public Resource {
 public:
  DRM();
  explicit DRM(const InstanceHandle& instance);

  
  int32_t GetDeviceID(const CompletionCallbackWithOutput<Var>& callback);
  
  
  bool GetHmonitor(int64_t* hmonitor);
  
  int32_t GetVoucherFile(const CompletionCallbackWithOutput<FileRef>& callback);
  
  
  int32_t MonitorIsExternal(
      const CompletionCallbackWithOutput<PP_Bool>& callback);
};

}  
}  

#endif  
