// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_KIOSK_APP_MANAGER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/app_mode/kiosk_app_data_delegate.h"
#include "chrome/browser/chromeos/policy/enterprise_install_attributes.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "ui/gfx/image/image_skia.h"

class PrefRegistrySimple;
class Profile;

namespace base {
class RefCountedString;
}

namespace extensions {
class Extension;
}

namespace chromeos {

class KioskAppData;
class KioskAppManagerObserver;

class KioskAppManager : public KioskAppDataDelegate {
 public:
  enum ConsumerKioskModeStatus {
    
    CONSUMER_KIOSK_MODE_CONFIGURABLE,
    
    CONSUMER_KIOSK_MODE_ENABLED,
    
    
    CONSUMER_KIOSK_MODE_DISABLED,
  };

  typedef base::Callback<void(bool success)> EnableKioskModeCallback;
  typedef base::Callback<void(ConsumerKioskModeStatus status)>
      GetConsumerKioskModeStatusCallback;

  
  struct App {
    explicit App(const KioskAppData& data);
    App();
    ~App();

    std::string app_id;
    std::string user_id;
    std::string name;
    gfx::ImageSkia icon;
    bool is_loading;
  };
  typedef std::vector<App> Apps;

  
  
  
  
  
  static const char kKioskDictionaryName[];
  static const char kKeyApps[];
  static const char kKeyAutoLoginState[];

  
  static const char kIconCacheDir[];

  
  static KioskAppManager* Get();

  
  static void Shutdown();

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  void GetConsumerKioskModeStatus(
      const GetConsumerKioskModeStatusCallback& callback);

  
  
  void EnableConsumerModeKiosk(const EnableKioskModeCallback& callback);

  
  std::string GetAutoLaunchApp() const;

  
  void SetAutoLaunchApp(const std::string& app_id);

  
  bool IsAutoLaunchRequested() const;

  
  bool IsAutoLaunchEnabled() const;

  
  void SetEnableAutoLaunch(bool value);

  
  
  void AddApp(const std::string& app_id);
  void RemoveApp(const std::string& app_id);

  
  void GetApps(Apps* apps) const;

  
  
  bool GetApp(const std::string& app_id, App* app) const;

  
  
  const base::RefCountedString* GetAppRawIcon(const std::string& app_id) const;

  
  bool GetDisableBailoutShortcut() const;

  
  void ClearAppData(const std::string& app_id);

  
  
  
  
  
  void UpdateAppDataFromProfile(const std::string& app_id,
                                Profile* profile,
                                const extensions::Extension* app);

  void AddObserver(KioskAppManagerObserver* observer);
  void RemoveObserver(KioskAppManagerObserver* observer);

 private:
  friend struct base::DefaultLazyInstanceTraits<KioskAppManager>;
  friend struct base::DefaultDeleter<KioskAppManager>;
  friend class KioskAppManagerTest;
  friend class KioskTest;

  enum AutoLoginState {
    AUTOLOGIN_NONE      = 0,
    AUTOLOGIN_REQUESTED = 1,
    AUTOLOGIN_APPROVED  = 2,
    AUTOLOGIN_REJECTED  = 3,
  };

  KioskAppManager();
  virtual ~KioskAppManager();

  
  void CleanUp();

  
  const KioskAppData* GetAppData(const std::string& app_id) const;
  KioskAppData* GetAppDataMutable(const std::string& app_id);

  
  void UpdateAppData();

  
  virtual void GetKioskAppIconCacheDir(base::FilePath* cache_dir) OVERRIDE;
  virtual void OnKioskAppDataChanged(const std::string& app_id) OVERRIDE;
  virtual void OnKioskAppDataLoadFailure(const std::string& app_id) OVERRIDE;

  
  
  void OnLockDevice(
      const EnableKioskModeCallback& callback,
      policy::EnterpriseInstallAttributes::LockResult result);

  
  
  void OnReadImmutableAttributes(
      const GetConsumerKioskModeStatusCallback& callback);

  
  void OnOwnerFileChecked(
      const GetConsumerKioskModeStatusCallback& callback,
      bool* owner_present);

  
  AutoLoginState GetAutoLoginState() const;
  void SetAutoLoginState(AutoLoginState state);

  
  bool ownership_established_;
  ScopedVector<KioskAppData> apps_;
  std::string auto_launch_app_id_;
  ObserverList<KioskAppManagerObserver, true> observers_;

  scoped_ptr<CrosSettings::ObserverSubscription>
      local_accounts_subscription_;
  scoped_ptr<CrosSettings::ObserverSubscription>
      local_account_auto_login_id_subscription_;

  DISALLOW_COPY_AND_ASSIGN(KioskAppManager);
};

}  

#endif  
