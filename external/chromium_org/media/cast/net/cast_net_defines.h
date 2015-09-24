// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_NET_CAST_NET_DEFINES_H_
#define MEDIA_CAST_NET_CAST_NET_DEFINES_H_

#include "base/basictypes.h"

namespace media {
namespace cast {

class FrameIdWrapHelper {
 public:
  FrameIdWrapHelper()
      : first_(true),
        frame_id_wrap_count_(0),
        range_(kLowRange) {}

  uint32 MapTo32bitsFrameId(const uint8 over_the_wire_frame_id) {
    if (first_) {
      first_ = false;
      if (over_the_wire_frame_id == 0xff) {
        
        return kStartFrameId;
      }
    }

    uint32 wrap_count = frame_id_wrap_count_;
    switch (range_) {
      case kLowRange:
        if (over_the_wire_frame_id > kLowRangeThreshold &&
            over_the_wire_frame_id < kHighRangeThreshold) {
          range_ = kMiddleRange;
        }
        if (over_the_wire_frame_id > kHighRangeThreshold) {
          
          
          --wrap_count;
        }
        break;
      case kMiddleRange:
        if (over_the_wire_frame_id > kHighRangeThreshold) {
          range_ = kHighRange;
        }
        break;
      case kHighRange:
        if (over_the_wire_frame_id < kLowRangeThreshold) {
          
          range_ = kLowRange;
          ++frame_id_wrap_count_;
          
          
          ++wrap_count;
        }
        break;
    }
    return (wrap_count << 8) + over_the_wire_frame_id;
  }

 private:
  enum Range {
    kLowRange,
    kMiddleRange,
    kHighRange,
  };

  static const uint8 kLowRangeThreshold = 0x0f;
  static const uint8 kHighRangeThreshold = 0xf0;
  static const uint32 kStartFrameId = GG_UINT32_C(0xffffffff);

  bool first_;
  uint32 frame_id_wrap_count_;
  Range range_;
};


}  
}  

#endif  
