// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_WIN_H_
#define CHROME_BROWSER_EXTENSIONS_GLOBAL_SHORTCUT_LISTENER_WIN_H_

#include <windows.h>

#include "base/lazy_instance.h"
#include "chrome/browser/extensions/global_shortcut_listener.h"
#include "ui/gfx/win/singleton_hwnd.h"

namespace extensions {

class GlobalShortcutListenerWin : public GlobalShortcutListener,
                                  public gfx::SingletonHwnd::Observer {
 public:
  virtual ~GlobalShortcutListenerWin();

  virtual void StartListening() OVERRIDE;
  virtual void StopListening() OVERRIDE;

 private:
  friend struct base::DefaultLazyInstanceTraits<GlobalShortcutListenerWin>;

  GlobalShortcutListenerWin();

  
  virtual void OnWndProc(HWND hwnd,
                         UINT message,
                         WPARAM wparam,
                         LPARAM lparam) OVERRIDE;

  
  virtual void RegisterAccelerator(
      const ui::Accelerator& accelerator,
      GlobalShortcutListener::Observer* observer) OVERRIDE;
  
  virtual void UnregisterAccelerator(
      const ui::Accelerator& accelerator,
      GlobalShortcutListener::Observer* observer) OVERRIDE;

  
  bool is_listening_;

  
  typedef std::map< ui::Accelerator, int > HotkeyIdMap;
  HotkeyIdMap hotkey_ids_;

  DISALLOW_COPY_AND_ASSIGN(GlobalShortcutListenerWin);
};

}  

#endif  
