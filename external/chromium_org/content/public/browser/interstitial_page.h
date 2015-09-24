// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_INTERSTITIAL_PAGE_H_
#define CONTENT_PUBLIC_BROWSER_INTERSTITIAL_PAGE_H_

#include "content/common/content_export.h"

class GURL;

namespace gfx {
class Size;
}

namespace content {

class InterstitialPageDelegate;
class RenderViewHost;
class WebContents;


class InterstitialPage {
 public:
  
  
  
  
  
  
  
  
  
  CONTENT_EXPORT static InterstitialPage* Create(
      WebContents* web_contents,
      bool new_navigation,
      const GURL& url,
      InterstitialPageDelegate* delegate);

  
  
  CONTENT_EXPORT static InterstitialPage* GetInterstitialPage(
      WebContents* web_contents);

  virtual ~InterstitialPage() {}

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  
  
  
  
  virtual void DontProceed() = 0;

  
  
  
  virtual void Proceed() = 0;

  
  virtual void SetSize(const gfx::Size& size) = 0;

  
  virtual void Focus() = 0;

  virtual RenderViewHost* GetRenderViewHostForTesting() const = 0;
  virtual InterstitialPageDelegate* GetDelegateForTesting() = 0;
  virtual void DontCreateViewForTesting() = 0;
};

}  

#endif  
