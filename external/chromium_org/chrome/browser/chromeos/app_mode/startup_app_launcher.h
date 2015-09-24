// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_APP_MODE_STARTUP_APP_LAUNCHER_H_
#define CHROME_BROWSER_CHROMEOS_APP_MODE_STARTUP_APP_LAUNCHER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/app_mode/kiosk_app_launch_error.h"
#include "google_apis/gaia/oauth2_token_service.h"

class Profile;

namespace extensions {
class WebstoreStandaloneInstaller;
}

namespace chromeos {

class StartupAppLauncher
    : public base::SupportsWeakPtr<StartupAppLauncher>,
      public OAuth2TokenService::Observer {
 public:
  class Delegate {
   public:
    
    
    virtual void InitializeNetwork() = 0;

    virtual void OnLoadingOAuthFile() = 0;
    virtual void OnInitializingTokenService() = 0;
    virtual void OnInstallingApp() = 0;
    virtual void OnReadyToLaunch() = 0;
    virtual void OnLaunchSucceeded() = 0;
    virtual void OnLaunchFailed(KioskAppLaunchError::Error error) = 0;

   protected:
    virtual ~Delegate() {}
  };

  StartupAppLauncher(Profile* profile,
                     const std::string& app_id,
                     Delegate* delegate);

  virtual ~StartupAppLauncher();

  
  void Initialize();

  
  void ContinueWithNetworkReady();

  
  void LaunchApp();

 private:
  
  struct KioskOAuthParams {
    std::string refresh_token;
    std::string client_id;
    std::string client_secret;
  };

  void OnLaunchSuccess();
  void OnLaunchFailure(KioskAppLaunchError::Error error);

  void MaybeInstall();

  
  void OnUpdateCheckFinished();

  void BeginInstall();
  void InstallCallback(bool success, const std::string& error);
  void OnReadyToLaunch();
  void UpdateAppData();

  void InitializeTokenService();
  void InitializeNetwork();

  void StartLoadingOAuthFile();
  static void LoadOAuthFileOnBlockingPool(KioskOAuthParams* auth_params);
  void OnOAuthFileLoaded(KioskOAuthParams* auth_params);

  
  virtual void OnRefreshTokenAvailable(const std::string& account_id) OVERRIDE;
  virtual void OnRefreshTokensLoaded() OVERRIDE;

  Profile* profile_;
  const std::string app_id_;
  Delegate* delegate_;
  bool install_attempted_;
  bool ready_to_launch_;

  scoped_refptr<extensions::WebstoreStandaloneInstaller> installer_;
  KioskOAuthParams auth_params_;

  DISALLOW_COPY_AND_ASSIGN(StartupAppLauncher);
};

}  

#endif  