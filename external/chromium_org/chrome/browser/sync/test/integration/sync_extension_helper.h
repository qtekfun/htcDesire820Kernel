// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_EXTENSION_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_EXTENSION_HELPER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "extensions/common/manifest.h"

class Profile;
class SyncTest;

namespace extensions {
class Extension;
}

class SyncExtensionHelper {
 public:
  
  static SyncExtensionHelper* GetInstance();

  
  
  void SetupIfNecessary(SyncTest* test);

  
  
  std::string InstallExtension(Profile* profile,
                               const std::string& name,
                               extensions::Manifest::Type type);

  
  void UninstallExtension(Profile* profile, const std::string& name);

  
  
  std::vector<std::string> GetInstalledExtensionNames(Profile* profile) const;

  
  void EnableExtension(Profile* profile, const std::string& name);

  
  void DisableExtension(Profile* profile, const std::string& name);

  
  bool IsExtensionEnabled(Profile* profile, const std::string& name) const;

  
  void IncognitoEnableExtension(Profile* profile, const std::string& name);

  
  void IncognitoDisableExtension(Profile* profile, const std::string& name);

  
  bool IsIncognitoEnabled(Profile* profile, const std::string& name) const;

  
  
  bool IsExtensionPendingInstallForSync(
      Profile* profile, const std::string& id) const;

  
  
  void InstallExtensionsPendingForSync(Profile* profile);

  
  
  static bool ExtensionStatesMatch(Profile* profile1, Profile* profile2);

 private:
  struct ExtensionState {
    enum EnabledState { DISABLED, PENDING, ENABLED };

    ExtensionState();
    ~ExtensionState();
    bool Equals(const ExtensionState &other) const;

    EnabledState enabled_state;
    bool incognito_enabled;
  };

  typedef std::map<std::string, ExtensionState> ExtensionStateMap;
  typedef std::map<std::string, scoped_refptr<extensions::Extension> >
      ExtensionNameMap;
  typedef std::map<Profile*, ExtensionNameMap> ProfileExtensionNameMap;
  typedef std::map<std::string, std::string> StringMap;
  typedef std::map<std::string, extensions::Manifest::Type> TypeMap;

  friend struct DefaultSingletonTraits<SyncExtensionHelper>;

  SyncExtensionHelper();
  ~SyncExtensionHelper();

  
  static ExtensionStateMap GetExtensionStates(Profile* profile);

  
  
  void SetupProfile(Profile* profile);

  
  
  
  scoped_refptr<extensions::Extension> GetExtension(
      Profile* profile, const std::string& name,
      extensions::Manifest::Type type) WARN_UNUSED_RESULT;

  ProfileExtensionNameMap profile_extensions_;
  StringMap id_to_name_;
  TypeMap id_to_type_;
  bool setup_completed_;

  DISALLOW_COPY_AND_ASSIGN(SyncExtensionHelper);
};

#endif  
