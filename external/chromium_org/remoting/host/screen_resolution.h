// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SCREEN_RESOLUTION_H_
#define REMOTING_HOST_SCREEN_RESOLUTION_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace remoting {

class ScreenResolution {
 public:
  ScreenResolution();
  ScreenResolution(const webrtc::DesktopSize& dimensions,
                   const webrtc::DesktopVector& dpi);

  
  webrtc::DesktopSize ScaleDimensionsToDpi(
      const webrtc::DesktopVector& new_dpi) const;

  
  const webrtc::DesktopSize& dimensions() const { return dimensions_; }

  
  const webrtc::DesktopVector& dpi() const { return dpi_; }

  
  
  bool IsEmpty() const;

  
  bool Equals(const ScreenResolution& other) const;

 private:
  webrtc::DesktopSize dimensions_;
  webrtc::DesktopVector dpi_;
};

}  

#endif  
