// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_STICKY_KEYS_H_
#define ASH_WM_STICKY_KEYS_H_

#include "ash/ash_export.h"
#include "base/memory/scoped_ptr.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_handler.h"

namespace ui {
class Event;
class KeyEvent;
class MouseEvent;
}  

namespace aura {
class Window;
}  

namespace ash {

class StickyKeysHandler;

class ASH_EXPORT StickyKeys : public ui::EventHandler {
 public:
  StickyKeys();
  virtual ~StickyKeys();

  
  void Enable(bool enabled);

 private:
  
  bool HandleKeyEvent(ui::KeyEvent* event);

  
  bool HandleMouseEvent(ui::MouseEvent* event);

  
  bool HandleScrollEvent(ui::ScrollEvent* event);

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;

  
  bool enabled_;

  
  scoped_ptr<StickyKeysHandler> shift_sticky_key_;
  scoped_ptr<StickyKeysHandler> alt_sticky_key_;
  scoped_ptr<StickyKeysHandler> ctrl_sticky_key_;

  DISALLOW_COPY_AND_ASSIGN(StickyKeys);
};

class ASH_EXPORT StickyKeysHandler {
 public:
  class StickyKeysHandlerDelegate {
   public:
    StickyKeysHandlerDelegate();
    virtual ~StickyKeysHandlerDelegate();

    
    virtual void DispatchKeyEvent(ui::KeyEvent* event,
                                  aura::Window* target) = 0;

    
    virtual void DispatchMouseEvent(ui::MouseEvent* event,
                                    aura::Window* target) = 0;

    
    virtual void DispatchScrollEvent(ui::ScrollEvent* event,
                                     aura::Window* target) = 0;
  };
  
  enum StickyKeyState {
    
    
    DISABLED,
    
    
    
    ENABLED,
    
    
    LOCKED,
  };

  
  StickyKeysHandler(ui::EventFlags modifier_flag,
                    StickyKeysHandlerDelegate* delegate);
  ~StickyKeysHandler();

  
  bool HandleKeyEvent(ui::KeyEvent* event);

  
  bool HandleMouseEvent(ui::MouseEvent* event);

  
  bool HandleScrollEvent(ui::ScrollEvent* event);

  
  StickyKeyState current_state() const { return current_state_; }

 private:
  
  enum KeyEventType {
    TARGET_MODIFIER_DOWN,  
    TARGET_MODIFIER_UP,  
    NORMAL_KEY_DOWN,  
    NORMAL_KEY_UP,  
    OTHER_MODIFIER_DOWN,  
    OTHER_MODIFIER_UP,  
  };

  
  KeyEventType TranslateKeyEvent(ui::KeyEvent* event);

  
  bool HandleDisabledState(ui::KeyEvent* event);

  
  bool HandleEnabledState(ui::KeyEvent* event);

  
  bool HandleLockedState(ui::KeyEvent* event);

  
  
  
  
  void DispatchEventAndReleaseModifier(ui::Event* event);

  
  void AppendNativeEventMask(unsigned int* state);

  
  void AppendModifier(ui::KeyEvent* event);
  void AppendModifier(ui::MouseEvent* event);
  void AppendModifier(ui::ScrollEvent* event);

  
  const ui::EventFlags modifier_flag_;

  
  StickyKeyState current_state_;

  
  bool event_from_myself_;

  
  
  
  
  bool preparing_to_enable_;

  
  
  
  int scroll_delta_;

  
  scoped_ptr<ui::KeyEvent> modifier_up_event_;

  scoped_ptr<StickyKeysHandlerDelegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(StickyKeysHandler);
};

}  

#endif  
