// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_RESIZER_H_
#define REMOTING_HOST_DESKTOP_RESIZER_H_

#include <list>

#include "base/memory/scoped_ptr.h"
#include "remoting/host/screen_resolution.h"

namespace remoting {

class DesktopResizer {
 public:
  virtual ~DesktopResizer() {}

  
  static scoped_ptr<DesktopResizer> Create();

  
  virtual ScreenResolution GetCurrentResolution() = 0;

  
  
  
  
  
  
  
  
  virtual std::list<ScreenResolution> GetSupportedResolutions(
      const ScreenResolution& preferred) = 0;

  
  
  
  
  virtual void SetResolution(const ScreenResolution& resolution) = 0;

  
  
  
  
  virtual void RestoreResolution(const ScreenResolution& original) = 0;
};

}  

#endif  
