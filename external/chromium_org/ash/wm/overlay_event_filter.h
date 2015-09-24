// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERLAY_EVENT_FILTER_H_
#define ASH_WM_OVERLAY_EVENT_FILTER_H_

#include "ash/shell_observer.h"
#include "base/compiler_specific.h"
#include "ui/aura/window.h"
#include "ui/events/event_handler.h"

namespace ash {
namespace internal {

class OverlayEventFilter : public ui::EventHandler,
                           public ShellObserver {
 public:
  
  class Delegate {
   public:
    
    virtual void Cancel() = 0;

    
    virtual bool IsCancelingKeyEvent(ui::KeyEvent* event) = 0;

    
    
    virtual aura::Window* GetWindow() = 0;
  };

  OverlayEventFilter();
  virtual ~OverlayEventFilter();

  
  
  
  
  void Activate(Delegate* delegate);

  
  void Deactivate();

  
  void Cancel();

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;

  
  virtual void OnLoginStateChanged(user::LoginStatus status) OVERRIDE;
  virtual void OnAppTerminating() OVERRIDE;
  virtual void OnLockStateChanged(bool locked) OVERRIDE;

 private:
  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(OverlayEventFilter);
};

}  
}  

#endif  
