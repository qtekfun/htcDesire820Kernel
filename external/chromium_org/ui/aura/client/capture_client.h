// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_CAPTURE_CLIENT_H_
#define UI_AURA_CLIENT_CAPTURE_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;

namespace client {

class AURA_EXPORT CaptureClient {
 public:
  
  virtual void SetCapture(Window* window) = 0;

  
  virtual void ReleaseCapture(Window* window) = 0;

  
  
  
  
  
  virtual Window* GetCaptureWindow() = 0;

  
  virtual Window* GetGlobalCaptureWindow() = 0;

 protected:
  virtual ~CaptureClient() {}
};

AURA_EXPORT void SetCaptureClient(Window* root_window,
                                  CaptureClient* client);
AURA_EXPORT CaptureClient* GetCaptureClient(Window* root_window);

AURA_EXPORT Window* GetCaptureWindow(Window* window);

}  
}  

#endif  
