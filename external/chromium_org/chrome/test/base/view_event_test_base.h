// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_VIEW_EVENT_TEST_BASE_H_
#define CHROME_TEST_BASE_VIEW_EVENT_TEST_BASE_H_

#if defined(HAS_OUT_OF_PROC_TEST_RUNNER)

#include "base/bind.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "base/threading/thread.h"
#include "chrome/browser/ui/views/chrome_views_delegate.h"
#include "content/public/test/test_browser_thread_bundle.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/views/widget/widget_delegate.h"

#if defined(OS_WIN)
#include "ui/base/win/scoped_ole_initializer.h"
#endif

namespace aura {
namespace test {
class AuraTestHelper;
}
}

namespace gfx {
class Size;
}


class ViewEventTestBase : public views::WidgetDelegate,
                          public testing::Test {
 public:
  ViewEventTestBase();

  
  
  void Done();

  
  virtual void SetUp() OVERRIDE;

  
  virtual void TearDown() OVERRIDE;

  
  virtual bool CanResize() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;

  
  void AddRef() {}
  void Release() {}

 protected:
  virtual ~ViewEventTestBase();

  
  virtual views::View* CreateContentsView() = 0;

  
  virtual void DoTestOnMessageLoop() = 0;

  
  
  void StartMessageLoopAndRunTest();

  
  
  
  virtual gfx::Size GetPreferredSize();

  
  
  
  template <class T, class Method>
  base::Closure CreateEventTask(T* target, Method method) {
    return base::Bind(&ViewEventTestBase::RunTestMethod, this,
                      base::Bind(method, target));
  }

  
  void ScheduleMouseMoveInBackground(int x, int y);

  views::Widget* window_;

 private:
  
  void StopBackgroundThread();

  
  
  void RunTestMethod(const base::Closure& task);

  
  views::View* content_view_;

  
  scoped_ptr<base::Thread> dnd_thread_;

  content::TestBrowserThreadBundle thread_bundle_;

#if defined(OS_WIN)
  ui::ScopedOleInitializer ole_initializer_;
#endif

#if defined(USE_AURA)
  scoped_ptr<aura::test::AuraTestHelper> aura_test_helper_;
#endif

  ChromeViewsDelegate views_delegate_;

  DISALLOW_COPY_AND_ASSIGN(ViewEventTestBase);
};

#define VIEW_TEST(test_class, name) \
  TEST_F(test_class, name) {\
    StartMessageLoopAndRunTest();\
  }

#endif  

#endif  
