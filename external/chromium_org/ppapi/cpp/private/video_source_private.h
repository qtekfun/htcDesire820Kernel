// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_VIDEO_SOURCE_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_VIDEO_SOURCE_PRIVATE_H_

#include <string>

#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class InstanceHandle;
class VideoFrame_Private;

class VideoSource_Private : public Resource {
 public:
  
  
  VideoSource_Private();

  
  
  explicit VideoSource_Private(const InstanceHandle& instance);

  
  
  
  VideoSource_Private(const VideoSource_Private& other);

  
  
  
  
  VideoSource_Private(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Open(const Var& stream_url,
               const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  int32_t GetFrame(
      const CompletionCallbackWithOutput<VideoFrame_Private>& cc);

  
  void Close();
};

}  

#endif  
