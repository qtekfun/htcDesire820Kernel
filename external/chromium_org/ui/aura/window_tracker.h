// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_TRACKER_H_
#define UI_AURA_WINDOW_TRACKER_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/window_observer.h"

namespace aura {

class AURA_EXPORT WindowTracker : public WindowObserver {
 public:
  typedef std::set<Window*> Windows;

  WindowTracker();
  virtual ~WindowTracker();

  
  const std::set<Window*>& windows() const { return windows_; }

  
  void Add(Window* window);

  
  void Remove(Window* window);

  
  
  bool Contains(Window* window);

  
  virtual void OnWindowDestroying(Window* window) OVERRIDE;

 private:
  Windows windows_;

  DISALLOW_COPY_AND_ASSIGN(WindowTracker);
};

}  

#endif  
