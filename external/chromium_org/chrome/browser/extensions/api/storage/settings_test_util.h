// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_TEST_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_API_STORAGE_SETTINGS_TEST_UTIL_H_

#include <set>
#include <string>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/storage/settings_namespace.h"
#include "chrome/browser/extensions/api/storage/settings_storage_factory.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/test_extension_service.h"
#include "chrome/browser/extensions/test_extension_system.h"
#include "chrome/test/base/testing_profile.h"
#include "extensions/browser/event_router.h"
#include "extensions/common/extension.h"

class ValueStore;

namespace extensions {

class SettingsFrontend;
namespace settings_test_util {

ValueStore* GetStorage(
    const std::string& extension_id,
    settings_namespace::Namespace setting_namespace,
    SettingsFrontend* frontend);

ValueStore* GetStorage(
    const std::string& extension_id,
    SettingsFrontend* frontend);

class MockExtensionService : public TestExtensionService {
 public:
  MockExtensionService();
  virtual ~MockExtensionService();

  
  void AddExtensionWithId(const std::string& id, Manifest::Type type);

  
  
  void AddExtensionWithIdAndPermissions(
      const std::string& id,
      Manifest::Type type,
      const std::set<std::string>& permissions);

  virtual const Extension* GetExtensionById(
      const std::string& id, bool include_disabled) const OVERRIDE;

 private:
  std::map<std::string, scoped_refptr<Extension> > extensions_;
};

class MockExtensionSystem : public TestExtensionSystem {
 public:
  explicit MockExtensionSystem(Profile* profile);
  virtual ~MockExtensionSystem();

  virtual EventRouter* event_router() OVERRIDE;
  virtual ExtensionService* extension_service() OVERRIDE;

 private:
  scoped_ptr<EventRouter> event_router_;
  MockExtensionService extension_service_;

  DISALLOW_COPY_AND_ASSIGN(MockExtensionSystem);
};

class MockProfile : public TestingProfile {
 public:
  explicit MockProfile(const base::FilePath& file_path);
  virtual ~MockProfile();
};

class ScopedSettingsStorageFactory : public SettingsStorageFactory {
 public:
  ScopedSettingsStorageFactory();

  explicit ScopedSettingsStorageFactory(
      const scoped_refptr<SettingsStorageFactory>& delegate);

  
  void Reset(const scoped_refptr<SettingsStorageFactory>& delegate);

  
  virtual ValueStore* Create(const base::FilePath& base_path,
                             const std::string& extension_id) OVERRIDE;

 private:
  
  virtual ~ScopedSettingsStorageFactory();

  scoped_refptr<SettingsStorageFactory> delegate_;
};

}  

}  

#endif  
