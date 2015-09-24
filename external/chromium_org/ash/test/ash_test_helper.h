// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_ASH_TEST_HELPER_H_
#define ASH_TEST_ASH_TEST_HELPER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"

namespace aura {
class Window;
}  

namespace base {
class MessageLoopForUI;
}  

namespace ui {
class ScopedAnimationDurationScaleMode;
}  

namespace ash {
namespace test {

class TestScreenshotDelegate;
class TestShellDelegate;

class AshTestHelper {
 public:
  explicit AshTestHelper(base::MessageLoopForUI* message_loop);
  ~AshTestHelper();

  
  
  
  void SetUp(bool start_session);

  
  void TearDown();

  
  
  
  aura::Window* CurrentContext();

  void RunAllPendingInMessageLoop();

  base::MessageLoopForUI* message_loop() { return message_loop_; }
  TestShellDelegate* test_shell_delegate() { return test_shell_delegate_; }
  TestScreenshotDelegate* test_screenshot_delegate() {
    return test_screenshot_delegate_;
  }

 private:
  base::MessageLoopForUI* message_loop_;  
  TestShellDelegate* test_shell_delegate_;  
  scoped_ptr<ui::ScopedAnimationDurationScaleMode> zero_duration_mode_;

  
  TestScreenshotDelegate* test_screenshot_delegate_;

  DISALLOW_COPY_AND_ASSIGN(AshTestHelper);
};

}  
}  

#endif  
