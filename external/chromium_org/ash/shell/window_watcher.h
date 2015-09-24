// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_WINDOW_WATCHER_H_
#define ASH_SHELL_WINDOW_WATCHER_H_

#include <map>

#include "ash/launcher/launcher_types.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/display_observer.h"

namespace aura {
class Window;
}

namespace ash {
namespace shell {


class WindowWatcher : public aura::WindowObserver,
                      public gfx::DisplayObserver {
 public:
  WindowWatcher();
  virtual ~WindowWatcher();

  aura::Window* GetWindowByID(ash::LauncherID id);

  
  virtual void OnWindowAdded(aura::Window* new_window) OVERRIDE;
  virtual void OnWillRemoveWindow(aura::Window* window) OVERRIDE;

  
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

 private:
  class WorkspaceWindowWatcher;

  typedef std::map<ash::LauncherID, aura::Window*> IDToWindow;

  
  IDToWindow id_to_window_;

  scoped_ptr<WorkspaceWindowWatcher> workspace_window_watcher_;

  DISALLOW_COPY_AND_ASSIGN(WindowWatcher);
};

}  
}  

#endif  
