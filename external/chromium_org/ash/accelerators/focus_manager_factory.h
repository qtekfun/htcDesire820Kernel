// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_FOCUS_MANAGER_FACTORY_H_
#define ASH_ACCELERATORS_FOCUS_MANAGER_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/focus/focus_manager_delegate.h"
#include "ui/views/focus/focus_manager_factory.h"

namespace ash {

class AshFocusManagerFactory : public views::FocusManagerFactory {
 public:
  AshFocusManagerFactory();
  virtual ~AshFocusManagerFactory();

 protected:
  
  virtual views::FocusManager* CreateFocusManager(
      views::Widget* widget,
      bool desktop_widget) OVERRIDE;

 private:
  class Delegate : public views::FocusManagerDelegate {
   public:
    
    virtual bool ProcessAccelerator(
        const ui::Accelerator& accelerator) OVERRIDE;
    virtual ui::AcceleratorTarget* GetCurrentTargetForAccelerator(
        const ui::Accelerator& accelerator) const OVERRIDE;
  };

  DISALLOW_COPY_AND_ASSIGN(AshFocusManagerFactory);
};

}  

#endif  
