// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_BASE_FOCUS_RULES_H_
#define UI_VIEWS_COREWM_BASE_FOCUS_RULES_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/corewm/focus_rules.h"

namespace views {
namespace corewm {

class VIEWS_EXPORT BaseFocusRules : public FocusRules {
 protected:
  BaseFocusRules();
  virtual ~BaseFocusRules();

  
  virtual bool SupportsChildActivation(aura::Window* window) const = 0;

  
  virtual bool IsWindowConsideredVisibleForActivation(
      aura::Window* window) const;

  
  virtual bool IsToplevelWindow(aura::Window* window) const OVERRIDE;
  virtual bool CanActivateWindow(aura::Window* window) const OVERRIDE;
  virtual bool CanFocusWindow(aura::Window* window) const OVERRIDE;
  virtual aura::Window* GetToplevelWindow(aura::Window* window) const OVERRIDE;
  virtual aura::Window* GetActivatableWindow(
      aura::Window* window) const OVERRIDE;
  virtual aura::Window* GetFocusableWindow(aura::Window* window) const OVERRIDE;
  virtual aura::Window* GetNextActivatableWindow(
      aura::Window* ignore) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(BaseFocusRules);
};

}  
}  

#endif  
