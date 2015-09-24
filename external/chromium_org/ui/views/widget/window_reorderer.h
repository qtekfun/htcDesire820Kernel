// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_WINDOW_REORDERER_H_
#define UI_VIEWS_WIDGET_WINDOW_REORDERER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"

namespace aura {
class Window;
}

namespace views {
class View;

class WindowReorderer : public aura::WindowObserver {
 public:
  WindowReorderer(aura::Window* window, View* root_view);
  virtual ~WindowReorderer();

  
  
  
  
  
  
  void ReorderChildWindows();

 private:
  
  virtual void OnWindowAdded(aura::Window* new_window) OVERRIDE;
  virtual void OnWillRemoveWindow(aura::Window* window) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  
  
  aura::Window* parent_window_;
  View* root_view_;

  
  
  class AssociationObserver;
  scoped_ptr<AssociationObserver> association_observer_;

  DISALLOW_COPY_AND_ASSIGN(WindowReorderer);
};

}  

#endif  
