// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_DEFAULT_APPS_H_
#define CHROME_BROWSER_EXTENSIONS_DEFAULT_APPS_H_

#include "base/basictypes.h"
#include "chrome/browser/extensions/external_provider_impl.h"
#include "extensions/common/manifest.h"

class Profile;

namespace base {
class DictionaryValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace default_apps {

enum InstallState {
  kUnknown,
  
  kProvideLegacyDefaultApps,
  kNeverInstallDefaultApps,
  kAlreadyInstalledDefaultApps
};

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

class Provider : public extensions::ExternalProviderImpl {
 public:
  Provider(Profile* profile,
           VisitorInterface* service,
           extensions::ExternalLoader* loader,
           extensions::Manifest::Location crx_location,
           extensions::Manifest::Location download_location,
           int creation_flags);

  bool ShouldInstallInProfile();

  
  virtual void VisitRegisteredExtension() OVERRIDE;
  virtual void SetPrefs(base::DictionaryValue* prefs) OVERRIDE;

 private:
  Profile* profile_;
  bool is_migration_;

  DISALLOW_COPY_AND_ASSIGN(Provider);
};

}  

#endif  
