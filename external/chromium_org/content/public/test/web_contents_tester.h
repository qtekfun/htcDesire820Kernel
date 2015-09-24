// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_TEST_WEB_CONTENTS_TESTER_H_
#define CONTENT_PUBLIC_TEST_WEB_CONTENTS_TESTER_H_

#include "content/public/common/page_transition_types.h"

class GURL;
struct WebPreferences;

namespace content {

class BrowserContext;
struct Referrer;
class RenderViewHost;
class SiteInstance;
class WebContents;

class WebContentsTester {
 public:
  
  
  
  
  static WebContentsTester* For(WebContents* contents);

  
  static WebContents* CreateTestWebContents(
      BrowserContext* browser_context,
      SiteInstance* instance);

  
  
  virtual void CommitPendingNavigation() = 0;

  virtual RenderViewHost* GetPendingRenderViewHost() const = 0;

  
  
  
  virtual void NavigateAndCommit(const GURL& url) = 0;

  
  virtual void TestSetIsLoading(bool value) = 0;

  
  
  
  virtual void ProceedWithCrossSiteNavigation() = 0;

  virtual void TestDidNavigate(RenderViewHost* render_view_host,
                               int page_id,
                               const GURL& url,
                               PageTransition transition) = 0;

  virtual void TestDidNavigateWithReferrer(
      RenderViewHost* render_view_host,
      int page_id,
      const GURL& url,
      const Referrer& referrer,
      PageTransition transition) = 0;

  
  virtual WebPreferences TestGetWebkitPrefs() = 0;
};

}  

#endif  
