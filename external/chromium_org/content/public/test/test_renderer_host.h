// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_TEST_RENDERER_HOST_H_
#define CONTENT_PUBLIC_TEST_TEST_RENDERER_HOST_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/test/test_browser_thread_bundle.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(USE_AURA)
#include "ui/aura/test/aura_test_helper.h"
#endif

namespace aura {
namespace test {
class AuraTestHelper;
}
}

namespace ui {
class ScopedOleInitializer;
}

namespace content {

class BrowserContext;
class MockRenderProcessHost;
class MockRenderProcessHostFactory;
class NavigationController;
class RenderProcessHostFactory;
class RenderViewHostDelegate;
class TestRenderFrameHostFactory;
class TestRenderViewHostFactory;
class WebContents;

class RenderViewHostTester {
 public:
  
  
  
  
  static RenderViewHostTester* For(RenderViewHost* host);

  
  static RenderViewHost* GetPendingForController(
      NavigationController* controller);

  
  
  
  
  
  static bool IsRenderViewHostSwappedOut(RenderViewHost* rvh);

  
  
  static bool TestOnMessageReceived(RenderViewHost* rvh,
                                    const IPC::Message& msg);

  
  static bool HasTouchEventHandler(RenderViewHost* rvh);

  virtual ~RenderViewHostTester() {}

  
  virtual bool CreateRenderView(const base::string16& frame_name,
                                int opener_route_id,
                                int32 max_page_id) = 0;

  
  
  
  virtual void SendNavigate(int page_id, const GURL& url) = 0;
  virtual void SendFailedNavigate(int page_id, const GURL& url) = 0;

  
  
  
  
  virtual void SendNavigateWithTransition(int page_id, const GURL& url,
                                          PageTransition transition) = 0;

  
  virtual void SendShouldCloseACK(bool proceed) = 0;

  
  
  virtual void SetContentsMimeType(const std::string& mime_type) = 0;

  
  
  virtual void SimulateSwapOutACK() = 0;

  
  
  virtual void SimulateWasHidden() = 0;
  virtual void SimulateWasShown() = 0;
};

class RenderViewHostTestEnabler {
 public:
  RenderViewHostTestEnabler();
  ~RenderViewHostTestEnabler();

 private:
  DISALLOW_COPY_AND_ASSIGN(RenderViewHostTestEnabler);
  friend class RenderViewHostTestHarness;

  scoped_ptr<MockRenderProcessHostFactory> rph_factory_;
  scoped_ptr<TestRenderViewHostFactory> rvh_factory_;
  scoped_ptr<TestRenderFrameHostFactory> rfh_factory_;
};

class RenderViewHostTestHarness : public testing::Test {
 public:
  RenderViewHostTestHarness();
  virtual ~RenderViewHostTestHarness();

  NavigationController& controller();
  WebContents* web_contents();
  RenderViewHost* rvh();
  RenderViewHost* pending_rvh();
  RenderViewHost* active_rvh();
  RenderFrameHost* main_rfh();
  BrowserContext* browser_context();
  MockRenderProcessHost* process();

  
  void DeleteContents();

  
  
  void SetContents(WebContents* contents);

  
  
  WebContents* CreateTestWebContents();

  
  
  void NavigateAndCommit(const GURL& url);

  
  void Reload();
  void FailedReload();

 protected:
  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  
  
  
  virtual BrowserContext* CreateBrowserContext();

  
  
  void SetThreadBundleOptions(int options) {
    DCHECK(thread_bundle_.get() == NULL);
    thread_bundle_options_ = options;
  }

  TestBrowserThreadBundle* thread_bundle() { return thread_bundle_.get(); }

#if defined(USE_AURA)
  aura::Window* root_window() { return aura_test_helper_->root_window(); }
#endif

  
  void SetRenderProcessHostFactory(RenderProcessHostFactory* factory);

 private:
  scoped_ptr<BrowserContext> browser_context_;

  
  
  
  scoped_ptr<WebContents> contents_;
#if defined(OS_WIN)
  scoped_ptr<ui::ScopedOleInitializer> ole_initializer_;
#endif
#if defined(USE_AURA)
  scoped_ptr<aura::test::AuraTestHelper> aura_test_helper_;
#endif
  RenderViewHostTestEnabler rvh_test_enabler_;

  int thread_bundle_options_;
  scoped_ptr<TestBrowserThreadBundle> thread_bundle_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewHostTestHarness);
};

}  

#endif  
