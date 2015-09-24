// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CHROME_EXTENSION_FUNCTION_H_
#define CHROME_BROWSER_EXTENSIONS_CHROME_EXTENSION_FUNCTION_H_

#include "extensions/browser/extension_function.h"

class Browser;
class Profile;

namespace content {
class WebContents;
}

namespace extensions {
class WindowController;
}

class ChromeAsyncExtensionFunction : public UIThreadExtensionFunction {
 public:
  ChromeAsyncExtensionFunction();

  Profile* GetProfile() const;

  
  
  bool CanOperateOnWindow(const extensions::WindowController* window_controller)
      const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Browser* GetCurrentBrowser();

  
  extensions::WindowController* GetExtensionWindowController();

  
  
  virtual content::WebContents* GetAssociatedWebContents() OVERRIDE;

 protected:
  virtual ~ChromeAsyncExtensionFunction();
};

class ChromeSyncExtensionFunction : public ChromeAsyncExtensionFunction {
 public:
  ChromeSyncExtensionFunction();

  virtual void Run() OVERRIDE;

 protected:
  virtual ~ChromeSyncExtensionFunction();
};

#endif  
