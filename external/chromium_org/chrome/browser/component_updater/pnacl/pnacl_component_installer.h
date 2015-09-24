// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_PNACL_PNACL_COMPONENT_INSTALLER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_PNACL_PNACL_COMPONENT_INSTALLER_H_

#include <list>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/version.h"
#include "chrome/browser/component_updater/component_updater_service.h"
#include "chrome/browser/component_updater/pnacl/pnacl_profile_observer.h"

class CommandLine;

namespace base {
class DictionaryValue;
}

namespace pnacl {
bool NeedsOnDemandUpdate();
}

class PnaclComponentInstaller : public ComponentInstaller {
 public:
  PnaclComponentInstaller();

  virtual ~PnaclComponentInstaller();

  virtual void OnUpdateError(int error) OVERRIDE;

  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) OVERRIDE;

  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

  
  void RegisterPnaclComponent(ComponentUpdateService* cus,
                              const CommandLine& command_line);

  
  
  void ReRegisterPnacl();

  CrxComponent GetCrxComponent();

  
  bool per_user() const { return per_user_; }

  
  void OnProfileChange();

  
  bool updates_disabled() const { return updates_disabled_; }

  
  base::FilePath GetPnaclBaseDirectory();

  base::Version current_version() const { return current_version_; }

  void set_current_version(const base::Version& current_version) {
    current_version_ = current_version;
  }

  std::string current_fingerprint() const { return current_fingerprint_; }

  void set_current_fingerprint(const std::string& current_fingerprint) {
    current_fingerprint_ = current_fingerprint;
  }

  ComponentUpdateService* cus() const { return cus_; }

 private:
  bool per_user_;
  bool updates_disabled_;
  scoped_ptr<PnaclProfileObserver> profile_observer_;
  base::FilePath current_profile_path_;
  base::Version current_version_;
  std::string current_fingerprint_;
  ComponentUpdateService* cus_;
  DISALLOW_COPY_AND_ASSIGN(PnaclComponentInstaller);
};

#endif  
