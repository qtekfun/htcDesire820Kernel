// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WORK_AREA_WATCHER_OBSERVER_H_
#define UI_BASE_WORK_AREA_WATCHER_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT WorkAreaWatcherObserver {
 public:
  virtual void WorkAreaChanged() = 0;

 protected:
  virtual ~WorkAreaWatcherObserver() {}
};

}  

#endif  
