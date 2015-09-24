// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_APP_PACK_UPDATER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_APP_PACK_UPDATER_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/extensions/external_cache.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"

namespace extensions {
class ExternalLoader;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class AppPackExternalLoader;
class EnterpriseInstallAttributes;

class AppPackUpdater : public chromeos::ExternalCache::Delegate {
 public:
  
  typedef base::Callback<void(const base::FilePath&)> ScreenSaverUpdateCallback;

  
  AppPackUpdater(net::URLRequestContextGetter* request_context,
                 EnterpriseInstallAttributes* install_attributes);
  virtual ~AppPackUpdater();

  
  
  bool created_external_loader() const { return created_extension_loader_; }

  
  
  
  extensions::ExternalLoader* CreateExternalLoader();

  
  
  
  
  void SetScreenSaverUpdateCallback(const ScreenSaverUpdateCallback& callback);

  
  
  
  void OnDamagedFileDetected(const base::FilePath& path);

 private:
  
  virtual void OnExtensionListsUpdated(
      const base::DictionaryValue* prefs) OVERRIDE;

  
  void AppPackChanged();

  
  void LoadPolicy();

  
  
  void UpdateExtensionLoader();

  
  
  void SetScreenSaverPath(const base::FilePath& path);

  base::WeakPtrFactory<AppPackUpdater> weak_ptr_factory_;

  
  
  std::string screen_saver_id_;
  base::FilePath screen_saver_path_;

  
  
  ScreenSaverUpdateCallback screen_saver_update_callback_;

  
  
  bool created_extension_loader_;
  base::WeakPtr<AppPackExternalLoader> extension_loader_;

  
  EnterpriseInstallAttributes* install_attributes_;

  
  chromeos::ExternalCache external_cache_;

  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      app_pack_subscription_;

  DISALLOW_COPY_AND_ASSIGN(AppPackUpdater);
};

}  

#endif  
