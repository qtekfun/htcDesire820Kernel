// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_PANELS_PANEL_MOUSE_WATCHER_H_
#define CHROME_BROWSER_UI_PANELS_PANEL_MOUSE_WATCHER_H_

#include "base/gtest_prod_util.h"
#include "base/observer_list.h"

namespace gfx {
class Point;
}

class PanelMouseWatcherObserver;

class PanelMouseWatcher {
 public:
  
  static PanelMouseWatcher* Create();

  
  virtual ~PanelMouseWatcher();

  void AddObserver(PanelMouseWatcherObserver* observer);
  void RemoveObserver(PanelMouseWatcherObserver* observer);

  
  
  virtual gfx::Point GetMousePosition() const = 0;

 protected:
  PanelMouseWatcher();

  
  virtual void NotifyMouseMovement(const gfx::Point& mouse_position);

  
  virtual bool IsActive() const = 0;

 private:
  friend class PanelMouseWatcherTest;
  FRIEND_TEST_ALL_PREFIXES(PanelMouseWatcherTest, StartStopWatching);
  friend class BasePanelBrowserTest;

  
  virtual void Start() = 0;
  virtual void Stop() = 0;

  ObserverList<PanelMouseWatcherObserver> observers_;
};

#endif  
