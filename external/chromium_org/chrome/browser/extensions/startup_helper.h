// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_STARTUP_HELPER_H_
#define CHROME_BROWSER_EXTENSIONS_STARTUP_HELPER_H_

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/pack_extension_job.h"

class CommandLine;
class Profile;

namespace extensions {

class StartupHelper : public PackExtensionJob::Client {
 public:
  StartupHelper();
  virtual ~StartupHelper();

  virtual void OnPackSuccess(
      const base::FilePath& crx_path,
      const base::FilePath& output_private_key_path) OVERRIDE;
  virtual void OnPackFailure(const std::string& error_message,
                             ExtensionCreator::ErrorType type) OVERRIDE;

  
  
  bool PackExtension(const CommandLine& cmd_line);

  
  
  // If the return value is false, a description of the problem may be written
  
  bool ValidateCrx(const CommandLine& cmd_line, std::string* error);

  
  
  
  bool UninstallExtension(const CommandLine& cmd_line, Profile* profile);

  
  
  
  bool InstallFromWebstore(const CommandLine& cmd_line, Profile* profile);

  
  
  
  
  
  
  void LimitedInstallFromWebstore(const CommandLine& cmd_line, Profile* profile,
                                  base::Callback<void()> done_callback);

  
  
  std::string WebStoreIdFromLimitedInstallCmdLine(const CommandLine& cmd_line);
 private:
  scoped_refptr<PackExtensionJob> pack_job_;
  bool pack_job_succeeded_;

  DISALLOW_COPY_AND_ASSIGN(StartupHelper);
};

}  

#endif  
