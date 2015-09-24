// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_VIEWS_TEST_BASE_H_
#define UI_VIEWS_TEST_VIEWS_TEST_BASE_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/views/test/test_views_delegate.h"

#if defined(OS_WIN)
#include "ui/base/win/scoped_ole_initializer.h"
#endif

namespace aura {
namespace test {
class AuraTestHelper;
}
}

namespace views {

class ViewsTestBase : public testing::Test {
 public:
  ViewsTestBase();
  virtual ~ViewsTestBase();

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  void RunPendingMessages();

  
  
  Widget::InitParams CreateParams(Widget::InitParams::Type type);

 protected:
  TestViewsDelegate& views_delegate() const { return *views_delegate_.get(); }

  void set_views_delegate(TestViewsDelegate* views_delegate) {
    views_delegate_.reset(views_delegate);
  }

  base::MessageLoopForUI* message_loop() { return &message_loop_; }

  
  
  gfx::NativeView GetContext();

 private:
  base::MessageLoopForUI message_loop_;
  scoped_ptr<TestViewsDelegate> views_delegate_;
#if defined(USE_AURA)
  scoped_ptr<aura::test::AuraTestHelper> aura_test_helper_;
#endif
  bool setup_called_;
  bool teardown_called_;

#if defined(OS_WIN)
  ui::ScopedOleInitializer ole_initializer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(ViewsTestBase);
};

}  

#endif  
