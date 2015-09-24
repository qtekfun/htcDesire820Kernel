// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_ACCELERATOR_CONTROLLER_H_
#define ASH_ACCELERATORS_ACCELERATOR_CONTROLLER_H_

#include <map>
#include <set>

#include "ash/accelerators/exit_warning_handler.h"
#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/accelerators/accelerator.h"

namespace ui {
class AcceleratorManager;
}

namespace ash {

struct AcceleratorData;
class BrightnessControlDelegate;
class ExitWarningHandler;
class ImeControlDelegate;
class KeyboardBrightnessControlDelegate;
class ScreenshotDelegate;
class VolumeControlDelegate;

class ASH_EXPORT AcceleratorControllerContext {
 public:
  AcceleratorControllerContext();
  ~AcceleratorControllerContext() {}

  
  
  void UpdateContext(const ui::Accelerator& accelerator);

  const ui::Accelerator& previous_accelerator() const {
    return previous_accelerator_;
  }
  bool repeated() const {
    return current_accelerator_ == previous_accelerator_ &&
        current_accelerator_.type() != ui::ET_UNKNOWN;
  }

 private:
  ui::Accelerator current_accelerator_;
  
  ui::Accelerator previous_accelerator_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratorControllerContext);
};

class ASH_EXPORT AcceleratorController : public ui::AcceleratorTarget {
 public:
  AcceleratorController();
  virtual ~AcceleratorController();

  
  
  
  void Register(const ui::Accelerator& accelerator,
                ui::AcceleratorTarget* target);

  
  void Unregister(const ui::Accelerator& accelerator,
                  ui::AcceleratorTarget* target);

  
  void UnregisterAll(ui::AcceleratorTarget* target);

  
  
  
  
  
  
  bool Process(const ui::Accelerator& accelerator);

  
  bool IsRegistered(const ui::Accelerator& accelerator) const;

  
  bool IsReservedAccelerator(const ui::Accelerator& accelerator) const;

  
  
  
  bool PerformAction(int action,
                     const ui::Accelerator& accelerator);

  
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool CanHandleAccelerators() const OVERRIDE;

  void SetBrightnessControlDelegate(
      scoped_ptr<BrightnessControlDelegate> brightness_control_delegate);
  void SetImeControlDelegate(
      scoped_ptr<ImeControlDelegate> ime_control_delegate);
  void SetScreenshotDelegate(
      scoped_ptr<ScreenshotDelegate> screenshot_delegate);
  BrightnessControlDelegate* brightness_control_delegate() const {
    return brightness_control_delegate_.get();
  }

  
  AcceleratorControllerContext* context() {
    return &context_;
  }

  
  ExitWarningHandler* GetExitWarningHandlerForTest() {
    return &exit_warning_handler_;
  }

 private:
  FRIEND_TEST_ALL_PREFIXES(AcceleratorControllerTest, GlobalAccelerators);

  
  void Init();

  
  void RegisterAccelerators(const AcceleratorData accelerators[],
                            size_t accelerators_length);

  void SetKeyboardBrightnessControlDelegate(
      scoped_ptr<KeyboardBrightnessControlDelegate>
      keyboard_brightness_control_delegate);

  scoped_ptr<ui::AcceleratorManager> accelerator_manager_;

  
  
  scoped_ptr<BrightnessControlDelegate> brightness_control_delegate_;
  scoped_ptr<ImeControlDelegate> ime_control_delegate_;
  scoped_ptr<KeyboardBrightnessControlDelegate>
      keyboard_brightness_control_delegate_;
  scoped_ptr<ScreenshotDelegate> screenshot_delegate_;

  
  AcceleratorControllerContext context_;

  
  
  ExitWarningHandler exit_warning_handler_;

  
  
  std::map<ui::Accelerator, int> accelerators_;

  
  std::set<int> actions_allowed_at_login_screen_;
  
  std::set<int> actions_allowed_at_lock_screen_;
  
  std::set<int> actions_allowed_at_modal_window_;
  
  std::set<int> reserved_actions_;
  
  std::set<int> nonrepeatable_actions_;
  
  std::set<int> actions_allowed_in_app_mode_;
  
  std::set<int> actions_needing_window_;

  DISALLOW_COPY_AND_ASSIGN(AcceleratorController);
};

}  

#endif  
