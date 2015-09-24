// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_ACTIVE_WINDOW_WATCHER_X_H_
#define UI_BASE_X_ACTIVE_WINDOW_WATCHER_X_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/base/ui_export.h"
#include "ui/base/x/x11_util.h"

template <typename T> struct DefaultSingletonTraits;

namespace ui {

class ActiveWindowWatcherXObserver;

namespace internal {
class RootWindowPropertyWatcherX;
}

class UI_EXPORT ActiveWindowWatcherX {
 public:
  static ActiveWindowWatcherX* GetInstance();
  static void AddObserver(ActiveWindowWatcherXObserver* observer);
  static void RemoveObserver(ActiveWindowWatcherXObserver* observer);

  
  
  static bool WMSupportsActivation();

 private:
  friend struct DefaultSingletonTraits<ActiveWindowWatcherX>;
  friend class ui::internal::RootWindowPropertyWatcherX;

  ActiveWindowWatcherX();
  ~ActiveWindowWatcherX();

  
  
  static Atom GetPropertyAtom();

  
  static void Notify();

  
  void NotifyActiveWindowChanged();

  ObserverList<ActiveWindowWatcherXObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(ActiveWindowWatcherX);
};

}  

#endif  
