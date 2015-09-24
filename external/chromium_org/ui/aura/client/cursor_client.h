// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_CURSOR_CLIENT_H_
#define UI_AURA_CLIENT_CURSOR_CLIENT_H_

#include "base/strings/string16.h"
#include "ui/aura/aura_export.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Display;
}

namespace aura {
class Window;
namespace client {
class CursorClientObserver;

class AURA_EXPORT CursorClient {
 public:
  
  virtual void SetCursor(gfx::NativeCursor cursor) = 0;

  
  virtual gfx::NativeCursor GetCursor() const = 0;

  
  virtual void ShowCursor() = 0;

  
  
  virtual void HideCursor() = 0;

  
  virtual void SetScale(float scale) = 0;

  
  virtual float GetScale() const = 0;

  
  virtual void SetCursorSet(ui::CursorSetType cursor_set) = 0;

  
  virtual ui::CursorSetType GetCursorSet() const = 0;

  
  virtual bool IsCursorVisible() const = 0;

  
  
  virtual void EnableMouseEvents() = 0;

  
  virtual void DisableMouseEvents() = 0;

  
  virtual bool IsMouseEventsEnabled() const = 0;

  
  virtual void SetDisplay(const gfx::Display& display) = 0;

  
  
  virtual void LockCursor() = 0;

  
  
  
  
  virtual void UnlockCursor() = 0;

  
  virtual bool IsCursorLocked() const = 0;

  
  virtual void AddObserver(CursorClientObserver* observer) = 0;
  virtual void RemoveObserver(CursorClientObserver* observer) = 0;

 protected:
  virtual ~CursorClient() {}
};

AURA_EXPORT void SetCursorClient(Window* window,
                                 CursorClient* client);
AURA_EXPORT CursorClient* GetCursorClient(Window* window);

}  
}  

#endif  
