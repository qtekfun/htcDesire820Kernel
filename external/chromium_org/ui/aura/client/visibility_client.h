// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_VISIBILITY_CLIENT_H_
#define UI_AURA_CLIENT_VISIBILITY_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;
namespace client {

class AURA_EXPORT VisibilityClient {
 public:
  
  
  
  virtual void UpdateLayerVisibility(Window* window, bool visible) = 0;

 protected:
  virtual ~VisibilityClient() {}
};

AURA_EXPORT void SetVisibilityClient(Window* window, VisibilityClient* client);

AURA_EXPORT VisibilityClient* GetVisibilityClient(Window* window);

}  
}  

#endif  
