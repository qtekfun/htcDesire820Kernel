// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_MRU_WINDOW_TRACKER_H_
#define ASH_WM_MRU_WINDOW_TRACKER_H_

#include <list>
#include <vector>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/window_observer.h"

namespace aura {
class RootWindow;
class Window;
namespace client {
class ActivationClient;
}
}

namespace ash {

ASH_EXPORT extern const int kSwitchableWindowContainerIds[];

ASH_EXPORT extern const size_t kSwitchableWindowContainerIdsLength;

class ASH_EXPORT MruWindowTracker
    : public aura::client::ActivationChangeObserver,
      public aura::WindowObserver {
 public:
  typedef std::vector<aura::Window*> WindowList;

  explicit MruWindowTracker(
      aura::client::ActivationClient* activation_client);
  virtual ~MruWindowTracker();

  
  
  
  
  
  
  
  static WindowList BuildWindowList(bool top_most_at_end);

  
  
  WindowList BuildMruWindowList();

  
  
  
  
  void SetIgnoreActivations(bool ignore);

 private:
  
  
  void SetActiveWindow(aura::Window* active_window);

  
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE;

  
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  
  std::list<aura::Window*> mru_windows_;

  aura::client::ActivationClient* activation_client_;

  bool ignore_window_activations_;

  DISALLOW_COPY_AND_ASSIGN(MruWindowTracker);
};

}  

#endif  
