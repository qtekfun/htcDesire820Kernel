// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_WINDOW_MOVE_CLIENT_H_
#define UI_AURA_CLIENT_WINDOW_MOVE_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/gfx/vector2d.h"

namespace gfx {
class Point;
}

namespace aura {
class Window;
namespace client {

enum WindowMoveResult {
  MOVE_SUCCESSFUL,  
  MOVE_CANCELED    
};

enum WindowMoveSource {
  WINDOW_MOVE_SOURCE_MOUSE,
  WINDOW_MOVE_SOURCE_TOUCH,
};

class AURA_EXPORT WindowMoveClient {
 public:
  
  
  
  
  virtual WindowMoveResult RunMoveLoop(Window* window,
                                       const gfx::Vector2d& drag_offset,
                                       WindowMoveSource source) = 0;

  
  virtual void EndMoveLoop() = 0;

 protected:
  virtual ~WindowMoveClient() {}
};

AURA_EXPORT void SetWindowMoveClient(Window* window,
                                     WindowMoveClient* client);
AURA_EXPORT WindowMoveClient* GetWindowMoveClient(Window* window);

}  
}  

#endif  
