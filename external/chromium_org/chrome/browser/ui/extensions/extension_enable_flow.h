// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_EXTENSIONS_EXTENSION_ENABLE_FLOW_H_
#define CHROME_BROWSER_UI_EXTENSIONS_EXTENSION_ENABLE_FLOW_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/page_navigator.h"

class ExtensionEnableFlowDelegate;

namespace content {
class PageNavigator;
class WebContents;
}

class ExtensionEnableFlow : public ExtensionInstallPrompt::Delegate,
                            public content::PageNavigator,
                            public content::NotificationObserver {
 public:
  ExtensionEnableFlow(Profile* profile,
                      const std::string& extension_id,
                      ExtensionEnableFlowDelegate* delegate);
  virtual ~ExtensionEnableFlow();

  
  
  
  
  
  
  
  void StartForWebContents(content::WebContents* parent_contents);
  void StartForNativeWindow(gfx::NativeWindow parent_window);
  void StartForCurrentlyNonexistentWindow(
      base::Callback<gfx::NativeWindow(void)> window_getter);

  const std::string& extension_id() const { return extension_id_; }

 private:
  
  
  
  
  void Run();

  
  
  
  
  void CheckPermissionAndMaybePromptUser();

  
  void CreatePrompt();

  
  void StartObserving();
  void StopObserving();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  
  virtual content::WebContents* OpenURL(
      const content::OpenURLParams& params) OVERRIDE;

  Profile* const profile_;
  const std::string extension_id_;
  ExtensionEnableFlowDelegate* const delegate_;  

  
  
  content::WebContents* parent_contents_;

  
  
  gfx::NativeWindow parent_window_;

  
  
  base::Callback<gfx::NativeWindow(void)> window_getter_;

  scoped_ptr<ExtensionInstallPrompt> prompt_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionEnableFlow);
};

#endif  
