// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_ACTIVATION_DELEGATE_H_
#define UI_AURA_CLIENT_ACTIVATION_DELEGATE_H_

#include "ui/aura/aura_export.h"

namespace ui {
class Event;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT ActivationDelegate {
 public:
  
  virtual bool ShouldActivate() const = 0;

 protected:
  virtual ~ActivationDelegate() {}
};

AURA_EXPORT void SetActivationDelegate(Window* window,
                                       ActivationDelegate* delegate);
AURA_EXPORT ActivationDelegate* GetActivationDelegate(Window* window);

}  
}  

#endif  
