// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_FOCUS_RULES_H_
#define UI_VIEWS_COREWM_FOCUS_RULES_H_

#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace views {
namespace corewm {

class VIEWS_EXPORT FocusRules {
 public:
  virtual ~FocusRules() {}

  
  
  
  
  virtual bool IsToplevelWindow(aura::Window* window) const = 0;
  
  virtual bool CanActivateWindow(aura::Window* window) const = 0;
  
  
  virtual bool CanFocusWindow(aura::Window* window) const = 0;

  
  
  
  
  
  virtual aura::Window* GetToplevelWindow(aura::Window* window) const = 0;
  
  
  
  
  
  
  
  
  
  virtual aura::Window* GetActivatableWindow(aura::Window* window) const = 0;
  virtual aura::Window* GetFocusableWindow(aura::Window* window) const = 0;

  
  
  
  
  
  
  
  
  
  virtual aura::Window* GetNextActivatableWindow(
      aura::Window* ignore) const = 0;
};

}  
}  

#endif  
