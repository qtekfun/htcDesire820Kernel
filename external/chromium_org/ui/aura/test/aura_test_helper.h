// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_AURA_TEST_HELPER_H_
#define UI_AURA_TEST_AURA_TEST_HELPER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/root_window.h"

namespace base {
class MessageLoopForUI;
}

namespace gfx {
class SurfaceFactoryOzone;
}

namespace ui {
class InputMethod;
class ScopedAnimationDurationScaleMode;
}

namespace aura {
class TestScreen;
namespace client {
class DefaultActivationClient;
class DefaultCaptureClient;
class FocusClient;
}
namespace test {
class TestWindowTreeClient;

class AuraTestHelper {
 public:
  explicit AuraTestHelper(base::MessageLoopForUI* message_loop);
  ~AuraTestHelper();

  
  void SetUp();

  
  
  void TearDown();

  
  void RunAllPendingInMessageLoop();

  Window* root_window() { return root_window_->window(); }
  RootWindow* dispatcher() { return root_window_.get(); }

  TestScreen* test_screen() { return test_screen_.get(); }

 private:
  base::MessageLoopForUI* message_loop_;
  bool setup_called_;
  bool teardown_called_;
  bool owns_root_window_;
  scoped_ptr<RootWindow> root_window_;
  scoped_ptr<TestWindowTreeClient> stacking_client_;
  scoped_ptr<client::DefaultActivationClient> activation_client_;
  scoped_ptr<client::DefaultCaptureClient> capture_client_;
  scoped_ptr<ui::InputMethod> test_input_method_;
  scoped_ptr<client::FocusClient> focus_client_;
  scoped_ptr<TestScreen> test_screen_;
  scoped_ptr<ui::ScopedAnimationDurationScaleMode> zero_duration_mode_;

#if defined(USE_OZONE)
  scoped_ptr<gfx::SurfaceFactoryOzone> surface_factory_;
#endif

  DISALLOW_COPY_AND_ASSIGN(AuraTestHelper);
};

}  
}  

#endif  
