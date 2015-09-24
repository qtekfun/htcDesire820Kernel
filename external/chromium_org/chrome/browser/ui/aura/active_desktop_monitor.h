// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AURA_ACTIVE_DESKTOP_MONITOR_H_
#define CHROME_BROWSER_UI_AURA_ACTIVE_DESKTOP_MONITOR_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/host_desktop.h"
#include "ui/aura/env_observer.h"

class ActiveDesktopMonitor : public aura::EnvObserver {
 public:
  
  
  explicit ActiveDesktopMonitor(chrome::HostDesktopType initial_desktop);
  virtual ~ActiveDesktopMonitor();

  
  
  
  
  static chrome::HostDesktopType GetLastActivatedDesktopType();

 private:
  
  static bool IsDesktopWindow(aura::RootWindow* root_window);

  
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;
  virtual void OnRootWindowActivated(aura::RootWindow* root_window) OVERRIDE;

  static ActiveDesktopMonitor* g_instance_;
  chrome::HostDesktopType last_activated_desktop_;

  DISALLOW_COPY_AND_ASSIGN(ActiveDesktopMonitor);
};

#endif  
