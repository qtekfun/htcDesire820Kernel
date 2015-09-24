// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_VIDEO_FRAME_PRIVATE_H_
#define PPAPI_CPP_PRIVATE_VIDEO_FRAME_PRIVATE_H_

#include <string.h>

#include "ppapi/c/pp_time.h"
#include "ppapi/c/private/pp_video_frame_private.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/pass_ref.h"


namespace pp {

class VideoFrame_Private {
 public:
  
  VideoFrame_Private();

  
  
  
  VideoFrame_Private(PassRef, const PP_VideoFrame_Private& pp_video_frame);

  
  
  VideoFrame_Private(const ImageData& image_data, PP_TimeTicks timestamp);

  
  
  
  VideoFrame_Private(const VideoFrame_Private& other);

  ~VideoFrame_Private();

  
  
  
  VideoFrame_Private& operator=(const VideoFrame_Private& other);

  const PP_VideoFrame_Private& pp_video_frame() const {
    return video_frame_;
  }

  ImageData image_data() const {
    return image_data_;
  }
  void set_image_data(const ImageData& image_data) {
    image_data_ = image_data;
    
    
    video_frame_.image_data = image_data_.pp_resource();
  }

  PP_TimeTicks timestamp() const { return video_frame_.timestamp; }
  void set_timestamp(PP_TimeTicks timestamp) {
    video_frame_.timestamp = timestamp;
  }

 private:
  ImageData image_data_;  
  PP_VideoFrame_Private video_frame_;
};

namespace internal {

template<>
struct CallbackOutputTraits<pp::VideoFrame_Private> {
  typedef PP_VideoFrame_Private* APIArgType;
  typedef PP_VideoFrame_Private StorageType;

  static inline APIArgType StorageToAPIArg(StorageType& t) {
    return &t;
  }

  static inline pp::VideoFrame_Private StorageToPluginArg(StorageType& t) {
    return pp::VideoFrame_Private(PASS_REF, t);
  }

  static inline void Initialize(StorageType* t) {
    VideoFrame_Private dummy;
    *t = dummy.pp_video_frame();
  }
};

}  

}  

#endif  
