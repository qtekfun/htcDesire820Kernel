// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_MANAGER_DELEGATE_H_
#define UI_VIEWS_FOCUS_FOCUS_MANAGER_DELEGATE_H_

#include "ui/views/views_export.h"

namespace ui {
class Accelerator;
class AcceleratorTarget;
}

namespace views {

class VIEWS_EXPORT FocusManagerDelegate {
 public:
  virtual ~FocusManagerDelegate() {}

  
  
  
  
  
  
  virtual bool ProcessAccelerator(const ui::Accelerator& accelerator) = 0;

  
  
  
  virtual ui::AcceleratorTarget* GetCurrentTargetForAccelerator(
      const ui::Accelerator& accelerator) const = 0;
};

}  

#endif  
