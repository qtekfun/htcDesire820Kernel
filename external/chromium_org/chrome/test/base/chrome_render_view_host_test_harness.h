// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_CHROME_RENDER_VIEW_HOST_TEST_HARNESS_H_
#define CHROME_TEST_BASE_CHROME_RENDER_VIEW_HOST_TEST_HARNESS_H_

#include "base/compiler_specific.h"
#include "content/public/test/test_renderer_host.h"

class TestingProfile;

namespace content {
class WebContents;
}

class ChromeRenderViewHostTestHarness
    : public content::RenderViewHostTestHarness {
 public:
  ChromeRenderViewHostTestHarness();
  virtual ~ChromeRenderViewHostTestHarness();

  TestingProfile* profile();

  content::RenderViewHostTester* rvh_tester();

 protected:
  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  virtual content::BrowserContext* CreateBrowserContext() OVERRIDE;
};

#endif  
