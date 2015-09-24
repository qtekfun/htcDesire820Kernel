// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_WINDOW_WATCHER_H_
#define ASH_SHELF_SHELF_WINDOW_WATCHER_H_

#include "ash/shelf/scoped_observer_with_duplicated_sources.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/scoped_observer.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/display_observer.h"

namespace aura {

class Window;

namespace client {
class ActivationClient;
}

}  

namespace ash {

class ShelfModel;
class ShelfItemDelegateManager;

namespace internal {
class ShelfWindowWatcher : public aura::client::ActivationChangeObserver,
                           public aura::WindowObserver,
                           public gfx::DisplayObserver {
 public:
  ShelfWindowWatcher(ShelfModel* model,
                     ShelfItemDelegateManager* item_delegate_manager);
  virtual ~ShelfWindowWatcher();

 private:
  class RootWindowObserver : public aura::WindowObserver {
   public:
    explicit RootWindowObserver(ShelfWindowWatcher* window_watcher);
    virtual ~RootWindowObserver();

   private:
    
    virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

    
    ShelfWindowWatcher* window_watcher_;

    DISALLOW_COPY_AND_ASSIGN(RootWindowObserver);
  };

  
  void AddLauncherItem(aura::Window* window);

  
  void RemoveLauncherItem(aura::Window* window);

  
  void OnRootWindowAdded(aura::Window* root_window);

  
  void OnRootWindowRemoved(aura::Window* root_window);

  
  void UpdateLauncherItemStatus(aura::Window* window, bool is_active);

  
  int GetLauncherItemIndexForWindow(aura::Window* window) const;

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  
  virtual void OnWindowAdded(aura::Window* window) OVERRIDE;
  virtual void OnWillRemoveWindow(aura::Window* window) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;

  
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

  
  ShelfModel* model_;
  ShelfItemDelegateManager* item_delegate_manager_;

  RootWindowObserver root_window_observer_;

  
  ScopedObserver<aura::Window, aura::WindowObserver> observed_windows_;

  
  ScopedObserver<aura::Window, aura::WindowObserver> observed_root_windows_;

  
  ScopedObserverWithDuplicatedSources<aura::client::ActivationClient,
      aura::client::ActivationChangeObserver> observed_activation_clients_;

  DISALLOW_COPY_AND_ASSIGN(ShelfWindowWatcher);
};

}  
}  

#endif  
