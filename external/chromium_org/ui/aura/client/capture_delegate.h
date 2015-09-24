// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_CAPTURE_DELEGATE_H_
#define UI_AURA_CLIENT_CAPTURE_DELEGATE_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;
namespace client {

class AURA_EXPORT CaptureDelegate {
 public:
  
  
  
  
  virtual void UpdateCapture(aura::Window* old_capture,
                             aura::Window* new_capture) = 0;

  
  virtual void OnOtherRootGotCapture() = 0;

  
  virtual void SetNativeCapture() = 0;
  virtual void ReleaseNativeCapture() = 0;

 protected:
  virtual ~CaptureDelegate() {}
};

}  
}  

#endif  
