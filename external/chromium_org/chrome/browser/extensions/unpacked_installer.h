// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UNPACKED_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_UNPACKED_INSTALLER_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/extension_installer.h"

class ExtensionService;

namespace extensions {

class Extension;
class RequirementsChecker;

class UnpackedInstaller
    : public base::RefCountedThreadSafe<UnpackedInstaller> {
 public:
  static scoped_refptr<UnpackedInstaller> Create(
      ExtensionService* extension_service);

  
  
  
  
  void Load(const base::FilePath& extension_path);

  
  
  
  
  
  
  
  bool LoadFromCommandLine(const base::FilePath& extension_path,
                           std::string* extension_id);

  
  bool prompt_for_plugins() { return prompt_for_plugins_; }
  void set_prompt_for_plugins(bool val) { prompt_for_plugins_ = val; }

  
  
  bool require_modern_manifest_version() const {
    return require_modern_manifest_version_;
  }
  void set_require_modern_manifest_version(bool val) {
    require_modern_manifest_version_ = val;
  }

 private:
  friend class base::RefCountedThreadSafe<UnpackedInstaller>;

  explicit UnpackedInstaller(ExtensionService* extension_service);
  virtual ~UnpackedInstaller();

  
  void ShowInstallPrompt();

  
  void CallCheckRequirements();

  
  void OnRequirementsChecked(std::vector<std::string> requirement_errors);

  
  bool IsLoadingUnpackedAllowed() const;

  
  
  
  
  
  
  
  void GetAbsolutePath();
  void CheckExtensionFileAccess();
  void LoadWithFileAccess(int flags);

  
  void ReportExtensionLoadError(const std::string& error);

  
  void ConfirmInstall();

  
  int GetFlags();

  
  base::WeakPtr<ExtensionService> service_weak_;

  
  
  base::FilePath extension_path_;

  
  
  bool prompt_for_plugins_;

  
  
  bool require_modern_manifest_version_;

  
  ExtensionInstaller installer_;

  DISALLOW_COPY_AND_ASSIGN(UnpackedInstaller);
};

}  

#endif  
