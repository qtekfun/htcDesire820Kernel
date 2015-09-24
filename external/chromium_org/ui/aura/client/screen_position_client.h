// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_SCREEN_POSITION_CLIENT_H_
#define UI_AURA_SCREEN_POSITION_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"

namespace gfx {
class Display;
class Rect;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT ScreenPositionClient {
 public:
  virtual ~ScreenPositionClient() {}

  
  
  virtual void ConvertPointToScreen(const Window* window,
                                    gfx::Point* point) = 0;
  virtual void ConvertPointFromScreen(const Window* window,
                                      gfx::Point* point) = 0;
  
  
  
  
  
  virtual void ConvertHostPointToScreen(Window* root_window,
                                        gfx::Point* point) = 0;
  
  
  virtual void SetBounds(Window* window,
                         const gfx::Rect& bounds,
                         const gfx::Display& display) = 0;
};

AURA_EXPORT void SetScreenPositionClient(Window* root_window,
                                         ScreenPositionClient* client);
AURA_EXPORT ScreenPositionClient* GetScreenPositionClient(
    const Window* root_window);

}  
}  

#endif  
