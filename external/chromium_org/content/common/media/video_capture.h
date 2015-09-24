// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_MEDIA_VIDEO_CAPTURE_H_
#define CONTENT_COMMON_MEDIA_VIDEO_CAPTURE_H_

namespace content {

enum VideoCaptureState {
  VIDEO_CAPTURE_STATE_STARTING,
  VIDEO_CAPTURE_STATE_STARTED,
  VIDEO_CAPTURE_STATE_PAUSED,
  VIDEO_CAPTURE_STATE_STOPPING,
  VIDEO_CAPTURE_STATE_STOPPED,
  VIDEO_CAPTURE_STATE_ERROR,
  VIDEO_CAPTURE_STATE_ENDED,
};

}  

#endif  
