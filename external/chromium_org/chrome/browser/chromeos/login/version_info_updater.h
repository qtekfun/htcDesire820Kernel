// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_VERSION_INFO_UPDATER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_VERSION_INFO_UPDATER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/boot_times_loader.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/chromeos/version_loader.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"

namespace chromeos {

class CrosSettings;

class VersionInfoUpdater : public policy::CloudPolicyStore::Observer {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnOSVersionLabelTextUpdated(
        const std::string& os_version_label_text) = 0;

    
    virtual void OnEnterpriseInfoUpdated(
        const std::string& enterprise_info) = 0;
  };

  explicit VersionInfoUpdater(Delegate* delegate);
  virtual ~VersionInfoUpdater();

  
  void set_delegate(Delegate* delegate) { delegate_ = delegate; }

  
  
  void StartUpdate(bool is_official_build);

 private:
  
  virtual void OnStoreLoaded(policy::CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(policy::CloudPolicyStore* store) OVERRIDE;

  
  void UpdateVersionLabel();

  
  void UpdateEnterpriseInfo();

  
  void SetEnterpriseInfo(const std::string& domain_name);

  
  void OnVersion(const std::string& version);

  
  VersionLoader version_loader_;
  
  BootTimesLoader boot_times_loader_;
  
  CancelableTaskTracker tracker_;

  
  std::string version_text_;

  
  std::string os_version_label_text_;

  ScopedVector<CrosSettings::ObserverSubscription> subscriptions_;

  chromeos::CrosSettings* cros_settings_;

  Delegate* delegate_;

  
  
  
  base::WeakPtrFactory<VersionInfoUpdater> weak_pointer_factory_;

  DISALLOW_COPY_AND_ASSIGN(VersionInfoUpdater);
};

}  

#endif  
