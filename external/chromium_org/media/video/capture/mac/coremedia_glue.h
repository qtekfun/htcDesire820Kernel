// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_VIDEO_CAPTURE_MAC_COREMEDIA_GLUE_H_
#define MEDIA_VIDEO_CAPTURE_MAC_COREMEDIA_GLUE_H_

#import <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h>

#include "base/basictypes.h"
#include "media/base/media_export.h"

class MEDIA_EXPORT CoreMediaGlue {
 public:
  
  typedef int64_t CMTimeValue;
  typedef int32_t CMTimeScale;
  typedef int64_t CMTimeEpoch;
  typedef uint32_t CMTimeFlags;
  typedef struct {
    CMTimeValue value;
    CMTimeScale timescale;
    CMTimeFlags flags;
    CMTimeEpoch epoch;
  } CMTime;

  
  typedef struct OpaqueCMSampleBuffer* CMSampleBufferRef;

  
  static CMTime CMTimeMake(int64_t value, int32_t timescale);

  
  static CVImageBufferRef CMSampleBufferGetImageBuffer(
      CMSampleBufferRef buffer);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(CoreMediaGlue);
};

#endif  
