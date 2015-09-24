// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SHAPE_TRACKER_H_
#define REMOTING_HOST_DESKTOP_SHAPE_TRACKER_H_

#include "base/memory/scoped_ptr.h"

namespace webrtc {
class DesktopCaptureOptions;
class DesktopRegion;
}  

namespace remoting {

class DesktopShapeTracker {
 public:
  static scoped_ptr<DesktopShapeTracker> Create(
      webrtc::DesktopCaptureOptions options);

  DesktopShapeTracker() {}
  virtual ~DesktopShapeTracker() {}

  
  virtual void RefreshDesktopShape() = 0;

  
  virtual const webrtc::DesktopRegion& desktop_shape() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopShapeTracker);
};

}  

#endif  
