// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALL_PROMPT_TEST_UTILS_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INSTALL_PROMPT_TEST_UTILS_H_

#include "chrome/browser/extensions/extension_install_prompt.h"

namespace chrome {

class MockExtensionInstallPromptDelegate
    : public ExtensionInstallPrompt::Delegate {
 public:
  MockExtensionInstallPromptDelegate()
      : proceed_count_(0),
        abort_count_(0) {}

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  int proceed_count() { return proceed_count_; }
  int abort_count() { return abort_count_; }

 protected:
  int proceed_count_;
  int abort_count_;
};

scoped_refptr<extensions::Extension> LoadInstallPromptExtension(
    const char* extension_dir_name,
    const char* manifest_file);

scoped_refptr<extensions::Extension> LoadInstallPromptExtension();

gfx::Image LoadInstallPromptIcon();

ExtensionInstallPrompt::Prompt BuildExtensionInstallPrompt(
    extensions::Extension* extension);

ExtensionInstallPrompt::Prompt BuildExtensionPostInstallPermissionsPrompt(
    extensions::Extension* extension);

}  

#endif  
