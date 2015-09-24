// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_X11_H_
#define CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_X11_H_

#include <set>
#include <X11/Xlib.h>

#include "base/lazy_instance.h"
#include "base/message_loop/message_pump_dispatcher.h"
#include "chrome/browser/extensions/global_shortcut_listener.h"

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#include "ui/base/gtk/gtk_signal.h"
#endif  

namespace extensions {

class GlobalShortcutListenerX11 : public GlobalShortcutListener,
                                  public base::MessagePumpDispatcher {
 public:
  virtual ~GlobalShortcutListenerX11();

  
  virtual void StartListening() OVERRIDE;
  virtual void StopListening() OVERRIDE;

  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

 private:
  friend struct base::DefaultLazyInstanceTraits<GlobalShortcutListenerX11>;

  GlobalShortcutListenerX11();

  
  virtual void RegisterAccelerator(
      const ui::Accelerator& accelerator,
      GlobalShortcutListener::Observer* observer) OVERRIDE;
  
  virtual void UnregisterAccelerator(
      const ui::Accelerator& accelerator,
      GlobalShortcutListener::Observer* observer) OVERRIDE;

#if defined(TOOLKIT_GTK)
  
  CHROMEG_CALLBACK_1(GlobalShortcutListenerX11, GdkFilterReturn,
                     OnXEvent, GdkXEvent*, GdkEvent*);
#endif

  
  void OnXKeyPressEvent(::XEvent* x_event);

  
  bool is_listening_;

  
  ::Display* x_display_;
  ::Window x_root_window_;

  
  typedef std::set<ui::Accelerator> RegisteredHotKeys;
  RegisteredHotKeys registered_hot_keys_;

  DISALLOW_COPY_AND_ASSIGN(GlobalShortcutListenerX11);
};

}  

#endif  
