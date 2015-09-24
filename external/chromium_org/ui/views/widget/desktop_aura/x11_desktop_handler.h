// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_HANDLER_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_X11_DESKTOP_HANDLER_H_

#include <X11/Xlib.h>
#undef RootWindow

#include "base/message_loop/message_loop.h"
#include "ui/aura/env_observer.h"
#include "ui/gfx/x/x11_atom_cache.h"
#include "ui/gfx/x/x11_types.h"
#include "ui/views/views_export.h"

template <typename T> struct DefaultSingletonTraits;

namespace views {

class VIEWS_EXPORT X11DesktopHandler : public base::MessageLoop::Dispatcher,
                                       public aura::EnvObserver {
 public:
  
  static X11DesktopHandler* get();

  
  void ActivateWindow(::Window window);

  
  bool IsActiveWindow(::Window window) const;

  
  
  
  void ProcessXEvent(const base::NativeEvent& event);

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;
  virtual void OnWillDestroyEnv() OVERRIDE;

 private:
  explicit X11DesktopHandler();
  virtual ~X11DesktopHandler();

  
  void OnActiveWindowChanged(::Window window);

  
  XDisplay* xdisplay_;

  
  ::Window x_root_window_;

  
  ::Window current_window_;

  ui::X11AtomCache atom_cache_;

  bool wm_supports_active_window_;

  DISALLOW_COPY_AND_ASSIGN(X11DesktopHandler);
};

}  

#endif  
