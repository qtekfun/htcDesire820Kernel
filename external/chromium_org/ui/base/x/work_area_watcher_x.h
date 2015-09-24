// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_WORK_AREA_WATCHER_X_H_
#define UI_BASE_X_WORK_AREA_WATCHER_X_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/base/ui_export.h"
#include "ui/base/x/x11_util.h"

template <typename T> struct DefaultSingletonTraits;

namespace ui {

class WorkAreaWatcherObserver;

namespace internal {
class RootWindowPropertyWatcherX;
}

class UI_EXPORT WorkAreaWatcherX {
 public:
  static WorkAreaWatcherX* GetInstance();
  static void AddObserver(WorkAreaWatcherObserver* observer);
  static void RemoveObserver(WorkAreaWatcherObserver* observer);

 private:
  friend struct DefaultSingletonTraits<WorkAreaWatcherX>;
  friend class ui::internal::RootWindowPropertyWatcherX;

  WorkAreaWatcherX();
  ~WorkAreaWatcherX();

  
  
  static Atom GetPropertyAtom();

  
  static void Notify();

  
  void NotifyWorkAreaChanged();

  ObserverList<WorkAreaWatcherObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(WorkAreaWatcherX);
};

}  

#endif  
