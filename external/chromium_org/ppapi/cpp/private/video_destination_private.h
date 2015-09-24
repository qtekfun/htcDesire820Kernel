// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_VIDEO_DESTINATION_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_VIDEO_DESTINATION_PRIVATE_H_

#include <string>

#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/pass_ref.h"
#include "ppapi/cpp/resource.h"


namespace pp {

class InstanceHandle;
class VideoFrame_Private;

class VideoDestination_Private : public Resource {
 public:
  
  
  VideoDestination_Private();

  
  
  explicit VideoDestination_Private(const InstanceHandle& instance);

  
  
  
  VideoDestination_Private(const VideoDestination_Private& other);

  
  
  
  
  VideoDestination_Private(PassRef, PP_Resource resource);

  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t Open(const Var& stream_url, const CompletionCallback& cc);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t PutFrame(const VideoFrame_Private& frame);

  
  void Close();
};

}  

#endif  
