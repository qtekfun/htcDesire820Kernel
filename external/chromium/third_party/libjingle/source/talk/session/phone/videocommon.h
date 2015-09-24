// Copyright 2011, Google Inc.
//  1. Redistributions of source code must retain the above copyright notice,
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     derived from this software without specific prior written permission.

#ifndef TALK_SESSION_PHONE_VIDEOCOMMON_H_
#define TALK_SESSION_PHONE_VIDEOCOMMON_H_

#include <string>

#include "talk/base/basictypes.h"

namespace cricket {

#define FOURCC(a, b, c, d) (\
    (static_cast<uint32>(a)) | (static_cast<uint32>(b) << 8) | \
    (static_cast<uint32>(c) << 16) | (static_cast<uint32>(d) << 24))

inline std::string GetFourccName(uint32 fourcc) {
  std::string name;
  name.push_back(static_cast<char>(fourcc & 0xFF));
  name.push_back(static_cast<char>((fourcc >> 8) & 0xFF));
  name.push_back(static_cast<char>((fourcc >> 16) & 0xFF));
  name.push_back(static_cast<char>((fourcc >> 24) & 0xFF));
  return name;
}

enum FourCC {
  
  FOURCC_I420 = FOURCC('I', '4', '2', '0'),
  FOURCC_YUY2 = FOURCC('Y', 'U', 'Y', '2'),
  FOURCC_UYVY = FOURCC('U', 'Y', 'V', 'Y'),
  FOURCC_M420 = FOURCC('M', '4', '2', '0'),
  FOURCC_24BG = FOURCC('2', '4', 'B', 'G'),
  FOURCC_ABGR = FOURCC('A', 'B', 'G', 'R'),
  FOURCC_BGRA = FOURCC('B', 'G', 'R', 'A'),
  FOURCC_ARGB = FOURCC('A', 'R', 'G', 'B'),
  FOURCC_MJPG = FOURCC('M', 'J', 'P', 'G'),
  FOURCC_RAW  = FOURCC('r', 'a', 'w', ' '),
  FOURCC_NV21 = FOURCC('N', 'V', '2', '1'),
  FOURCC_NV12 = FOURCC('N', 'V', '1', '2'),
  
  
  FOURCC_RGGB = FOURCC('R', 'G', 'G', 'B'),
  FOURCC_BGGR = FOURCC('B', 'G', 'G', 'R'),
  FOURCC_GRBG = FOURCC('G', 'R', 'B', 'G'),
  FOURCC_GBRG = FOURCC('G', 'B', 'R', 'G'),

  
  
  FOURCC_IYUV = FOURCC('I', 'Y', 'U', 'V'),  
  FOURCC_YU12 = FOURCC('Y', 'U', '1', '2'),  
  FOURCC_YUYV = FOURCC('Y', 'U', 'Y', 'V'),  
  FOURCC_YUVS = FOURCC('y', 'u', 'v', 's'),  
  FOURCC_HDYC = FOURCC('H', 'D', 'Y', 'C'),  
  FOURCC_2VUY = FOURCC('2', 'v', 'u', 'y'),  
  FOURCC_JPEG = FOURCC('J', 'P', 'E', 'G'),  
  FOURCC_BA81 = FOURCC('B', 'A', '8', '1'),  

  
  FOURCC_ANY  = 0xFFFFFFFF,
};

uint32 CanonicalFourCC(uint32 fourcc);


static const int64 kNumNanosecsPerSec = 1000000000;

struct VideoFormat {
  static const int64 kMinimumInterval = kNumNanosecsPerSec / 10000;  

  VideoFormat() : width(0), height(0), interval(0), fourcc(0) {}

  VideoFormat(int w, int h, int64 interval_ns, uint32 cc)
      : width(w),
        height(h),
        interval(interval_ns),
        fourcc(cc) {
  }

  VideoFormat(const VideoFormat& format)
      : width(format.width),
        height(format.height),
        interval(format.interval),
        fourcc(format.fourcc) {
  }

  static int64 FpsToInterval(int fps) {
    return fps ? kNumNanosecsPerSec / fps : kMinimumInterval;
  }

  static int IntervalToFps(int64 interval) {
    
    interval = talk_base::_max(interval, kMinimumInterval);
    return static_cast<int>(kNumNanosecsPerSec / interval);
  }

  bool operator==(const VideoFormat& format) const {
    return width == format.width && height == format.height &&
        interval == format.interval && fourcc == format.fourcc;
  }

  bool operator!=(const VideoFormat& format) const {
    return !(*this == format);
  }

  bool operator<(const VideoFormat& format) const {
    return (fourcc < format.fourcc) ||
        (fourcc == format.fourcc && width < format.width) ||
        (fourcc == format.fourcc && width == format.width &&
            height < format.height) ||
        (fourcc == format.fourcc && width == format.width &&
            height == format.height && interval > format.interval);
  }

  int framerate() const { return IntervalToFps(interval); }

  int    width;     
  int    height;    
  int64  interval;  
  uint32 fourcc;    
};

enum CaptureResult {
  CR_SUCCESS,    
  CR_PENDING,    
  CR_FAILURE,    
  CR_NO_DEVICE,  
};

}  

#endif  
