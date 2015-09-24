// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_MENU_MODEL_TEST_H_
#define CHROME_TEST_BASE_MENU_MODEL_TEST_H_

#include "ui/base/accelerators/accelerator.h"
#include "ui/base/models/simple_menu_model.h"

class MenuModelTest {
 public:
  MenuModelTest() {}
  virtual ~MenuModelTest() {}

 protected:
  
  
  class Delegate : public ui::SimpleMenuModel::Delegate,
                   public ui::AcceleratorProvider {
   public:
    Delegate() : execute_count_(0), enable_count_(0) {}

    virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
    virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
    virtual bool GetAcceleratorForCommandId(
        int command_id,
        ui::Accelerator* accelerator) OVERRIDE;
    virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

    int execute_count_;
    mutable int enable_count_;
  };

  
  
  
  
  void CountEnabledExecutable(ui::MenuModel* model, int* count);

  Delegate delegate_;
};

#endif  
