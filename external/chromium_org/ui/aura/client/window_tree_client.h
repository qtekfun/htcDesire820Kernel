// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_WINDOW_TREE_CLIENT_H_
#define UI_AURA_CLIENT_WINDOW_TREE_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace gfx {
class Rect;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT WindowTreeClient {
 public:
  virtual ~WindowTreeClient() {}

  
  
  
  
  
  
  
  
  
  virtual Window* GetDefaultParent(
      Window* context,
      Window* window,
      const gfx::Rect& bounds) = 0;
};

AURA_EXPORT void SetWindowTreeClient(Window* window,
                                     WindowTreeClient* window_tree_client);
WindowTreeClient* GetWindowTreeClient(Window* window);

AURA_EXPORT void ParentWindowWithContext(Window* window,
                                         Window* context,
                                         const gfx::Rect& screen_bounds);

}  
}  

#endif  
