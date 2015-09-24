// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_DEFAULT_COMPONENT_INSTALLER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_DEFAULT_COMPONENT_INSTALLER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/version.h"
#include "chrome/browser/component_updater/component_updater_service.h"

namespace base {
class DictionaryValue;
class FilePath;
}  

class ComponentInstallerTraits {
 public:
  virtual ~ComponentInstallerTraits();

  
  
  virtual bool VerifyInstallation(const base::FilePath& dir) const = 0;

  
  
  virtual bool CanAutoUpdate() const = 0;

  
  
  
  
  virtual bool OnCustomInstall(const base::DictionaryValue& manifest,
                               const base::FilePath& install_dir) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void ComponentReady(
      const base::Version& version,
      const base::FilePath& install_dir,
      scoped_ptr<base::DictionaryValue> manifest) = 0;

  
  
  virtual base::FilePath GetBaseDirectory() const = 0;

  
  virtual void GetHash(std::vector<uint8>* hash) const = 0;

  
  virtual std::string GetName() const = 0;
};

class DefaultComponentInstaller : public ComponentInstaller {
 public:
  explicit DefaultComponentInstaller(
      scoped_ptr<ComponentInstallerTraits> installer_traits);

  
  void Register(ComponentUpdateService* cus);

  
  virtual void OnUpdateError(int error) OVERRIDE;
  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) OVERRIDE;
  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

  virtual ~DefaultComponentInstaller();

 private:
  base::FilePath GetInstallDirectory();
  bool InstallHelper(const base::DictionaryValue& manifest,
                     const base::FilePath& unpack_path,
                     const base::FilePath& install_path);
  void StartRegistration(ComponentUpdateService* cus);
  void FinishRegistration(ComponentUpdateService* cus);

  base::Version current_version_;
  std::string current_fingerprint_;
  scoped_ptr<base::DictionaryValue> current_manifest_;
  scoped_ptr<ComponentInstallerTraits> installer_traits_;

  DISALLOW_COPY_AND_ASSIGN(DefaultComponentInstaller);
};

#endif  
