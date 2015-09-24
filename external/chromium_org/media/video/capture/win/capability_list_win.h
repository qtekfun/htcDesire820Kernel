// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_WIN_CAPABILITY_LIST_WIN_H_
#define MEDIA_VIDEO_CAPTURE_WIN_CAPABILITY_LIST_WIN_H_

#include <list>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "media/video/capture/video_capture_types.h"

namespace media {

struct VideoCaptureCapabilityWin {
  explicit VideoCaptureCapabilityWin(int index)
      : stream_index(index),
        frame_rate_numerator(0),
        frame_rate_denominator(1) {}
  int stream_index;
  
  
  int frame_rate_numerator;
  int frame_rate_denominator;
  VideoCaptureFormat supported_format;
};

class CapabilityList : public base::NonThreadSafe {
 public:
  CapabilityList();
  ~CapabilityList();

  bool empty() const { return capabilities_.empty(); }

  
  void Add(const VideoCaptureCapabilityWin& capability);

  
  
  
  const VideoCaptureCapabilityWin& GetBestMatchedFormat(
      int requested_width,
      int requested_height,
      int requested_frame_rate) const;

 private:
  typedef std::list<VideoCaptureCapabilityWin> Capabilities;
  Capabilities capabilities_;

  DISALLOW_COPY_AND_ASSIGN(CapabilityList);
};

}  

#endif  
