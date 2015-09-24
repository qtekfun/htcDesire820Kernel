// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_AW_BROWSER_DELEGATE_H_
#define ANDROID_WEBVIEW_AW_BROWSER_DELEGATE_H_

#include "base/basictypes.h"

namespace content {
class BrowserContext;
class JavaScriptDialogManager;
class WebContents;
}

namespace android_webview {

class AwContentsContainer;

class AwBrowserDependencyFactory {
 public:
  virtual ~AwBrowserDependencyFactory();

  
  
  static void SetInstance(AwBrowserDependencyFactory* delegate);

  
  static AwBrowserDependencyFactory* GetInstance();

  
  virtual content::BrowserContext* GetBrowserContext() = 0;

  
  virtual content::WebContents* CreateWebContents() = 0;

 protected:
  AwBrowserDependencyFactory();

 private:
  DISALLOW_COPY_AND_ASSIGN(AwBrowserDependencyFactory);
};

}  

#endif  
