// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_INSTALLER_H_

#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/requirements_checker.h"
#include "extensions/common/extension.h"

class Profile;

namespace content {
class WebContents;
}

namespace extensions {


class ExtensionInstaller {
 public:
  typedef base::Callback<void(std::vector<std::string>)> RequirementsCallback;

  explicit ExtensionInstaller(Profile* profile);
  ~ExtensionInstaller();

  
  void CheckRequirements(const RequirementsCallback& callback);

  
  base::string16 CheckManagementPolicy();

  Profile* profile() const {
    return profile_;
  }

  void set_extension(const Extension* extension) {
    extension_ = extension;
  }

  scoped_refptr<const Extension> extension() {
    return extension_;
  }

 private:
  scoped_ptr<RequirementsChecker> requirements_checker_;

  
  Profile* profile_;

  
  
  scoped_refptr<const Extension> extension_;

  base::WeakPtrFactory<ExtensionInstaller> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstaller);
};

}  

#endif  
