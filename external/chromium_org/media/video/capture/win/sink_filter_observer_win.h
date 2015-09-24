// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_VIDEO_CAPTURE_WIN_SINK_FILTER_OBSERVER_WIN_H_
#define MEDIA_VIDEO_CAPTURE_WIN_SINK_FILTER_OBSERVER_WIN_H_

namespace media {

class SinkFilterObserver {
 public:
  
  
  virtual void FrameReceived(const uint8* buffer, int length) = 0;
 protected:
  virtual ~SinkFilterObserver();
};

}  

#endif  
