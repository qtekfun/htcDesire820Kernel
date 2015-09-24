// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_CONTENT_BROWSER_TEST_H_
#define CONTENT_TEST_CONTENT_BROWSER_TEST_H_

#include "base/memory/scoped_ptr.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_base.h"

class GURL;

namespace content {
class ContentRendererClient;
class Shell;
class ShellMainDelegate;

class ContentBrowserTest : public BrowserTestBase {
 protected:
  ContentBrowserTest();
  virtual ~ContentBrowserTest();

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  virtual void RunTestOnMainThreadLoop() OVERRIDE;

 protected:
  
  Shell* CreateBrowser();

  
  Shell* CreateOffTheRecordBrowser();

  
  Shell* shell() const { return shell_; }

 private:
  scoped_ptr<ShellMainDelegate> shell_main_delegate_;

  Shell* shell_;

  bool setup_called_;

  
  scoped_ptr<ContentRendererClient> single_process_renderer_client_;
};

}  

#endif  
