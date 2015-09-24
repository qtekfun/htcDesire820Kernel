// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_OOBE_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_OOBE_UI_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/login/oobe_display.h"
#include "chrome/browser/ui/webui/chromeos/login/core_oobe_handler.h"
#include "content/public/browser/web_ui_controller.h"

namespace base {
class DictionaryValue;
}  

namespace chromeos {
class AppLaunchSplashScreenActor;
class BaseScreenHandler;
class CoreOobeHandler;
class ErrorScreenHandler;
class KioskAppMenuHandler;
class KioskEnableScreenActor;
class LoginScreenContext;
class NativeWindowDelegate;
class NetworkDropdownHandler;
class NetworkStateInformer;
class GaiaScreenHandler;
class SigninScreenHandler;
class SigninScreenHandlerDelegate;
class UpdateScreenHandler;

class OobeUI : public OobeDisplay,
               public content::WebUIController,
               public CoreOobeHandler::Delegate {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnCurrentScreenChanged(
        Screen current_screen, Screen new_screen) = 0;
  };

  
  static const char kScreenOobeNetwork[];
  static const char kScreenOobeEula[];
  static const char kScreenOobeUpdate[];
  static const char kScreenOobeEnrollment[];
  static const char kScreenGaiaSignin[];
  static const char kScreenAccountPicker[];
  static const char kScreenKioskAutolaunch[];
  static const char kScreenKioskEnable[];
  static const char kScreenErrorMessage[];
  static const char kScreenUserImagePicker[];
  static const char kScreenTpmError[];
  static const char kScreenPasswordChanged[];
  static const char kScreenManagedUserCreationFlow[];
  static const char kScreenTermsOfService[];
  static const char kScreenWrongHWID[];
  static const char kScreenAppLaunchSplash[];
  static const char kScreenConfirmPassword[];
  static const char kScreenMessageBox[];

  OobeUI(content::WebUI* web_ui, const GURL& url);
  virtual ~OobeUI();

  
  virtual void ShowScreen(WizardScreen* screen) OVERRIDE;
  virtual void HideScreen(WizardScreen* screen) OVERRIDE;
  virtual UpdateScreenActor* GetUpdateScreenActor() OVERRIDE;
  virtual NetworkScreenActor* GetNetworkScreenActor() OVERRIDE;
  virtual EulaScreenActor* GetEulaScreenActor() OVERRIDE;
  virtual EnrollmentScreenActor* GetEnrollmentScreenActor() OVERRIDE;
  virtual ResetScreenActor* GetResetScreenActor() OVERRIDE;
  virtual KioskAutolaunchScreenActor* GetKioskAutolaunchScreenActor() OVERRIDE;
  virtual KioskEnableScreenActor* GetKioskEnableScreenActor() OVERRIDE;
  virtual TermsOfServiceScreenActor*
      GetTermsOfServiceScreenActor() OVERRIDE;
  virtual UserImageScreenActor* GetUserImageScreenActor() OVERRIDE;
  virtual ErrorScreenActor* GetErrorScreenActor() OVERRIDE;
  virtual WrongHWIDScreenActor* GetWrongHWIDScreenActor() OVERRIDE;
  virtual LocallyManagedUserCreationScreenHandler*
      GetLocallyManagedUserCreationScreenActor() OVERRIDE;
  virtual AppLaunchSplashScreenActor*
      GetAppLaunchSplashScreenActor() OVERRIDE;
  virtual bool IsJSReady(const base::Closure& display_is_ready_callback)
      OVERRIDE;

  
  void GetLocalizedStrings(base::DictionaryValue* localized_strings);

  
  void InitializeHandlers();

  
  
  void OnScreenAssetsLoaded(const std::string& async_assets_load_id);

  
  void ShowOobeUI(bool show);

  
  
  
  void ShowRetailModeLoginSpinner();

  
  void ShowSigninScreen(const LoginScreenContext& context,
                        SigninScreenHandlerDelegate* delegate,
                        NativeWindowDelegate* native_window_delegate);

  
  void ShowAppLaunchSplashScreen();

  
  void ResetSigninScreenHandlerDelegate();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  Screen current_screen() const { return current_screen_; }

  const std::string& GetScreenName(Screen screen) const;

  SigninScreenHandler* signin_screen_handler_for_test() {
    return signin_screen_handler_;
  }

 private:
  
  void InitializeScreenMaps();

  void AddScreenHandler(BaseScreenHandler* handler);

  
  virtual void OnCurrentScreenChanged(const std::string& screen) OVERRIDE;

  
  std::string display_type_;

  
  
  scoped_refptr<NetworkStateInformer> network_state_informer_;

  
  CoreOobeHandler* core_handler_;

  
  
  NetworkDropdownHandler* network_dropdown_handler_;

  
  UpdateScreenHandler* update_screen_handler_;
  NetworkScreenActor* network_screen_actor_;
  EulaScreenActor* eula_screen_actor_;
  EnrollmentScreenActor* enrollment_screen_actor_;
  ResetScreenActor* reset_screen_actor_;
  KioskAutolaunchScreenActor* autolaunch_screen_actor_;
  KioskEnableScreenActor* kiosk_enable_screen_actor_;
  WrongHWIDScreenActor* wrong_hwid_screen_actor_;
  LocallyManagedUserCreationScreenHandler*
      locally_managed_user_creation_screen_actor_;
  AppLaunchSplashScreenActor* app_launch_splash_screen_actor_;

  
  
  ErrorScreenHandler* error_screen_handler_;

  
  
  GaiaScreenHandler* gaia_screen_handler_;

  
  
  SigninScreenHandler* signin_screen_handler_;

  TermsOfServiceScreenActor* terms_of_service_screen_actor_;
  UserImageScreenActor* user_image_screen_actor_;

  std::vector<BaseScreenHandler*> handlers_;  

  KioskAppMenuHandler* kiosk_app_menu_handler_;  

  
  Screen current_screen_;

  
  std::map<std::string, Screen> screen_ids_;

  
  std::vector<std::string> screen_names_;

  
  
  bool ready_;

  
  std::vector<base::Closure> ready_callbacks_;

  
  ObserverList<Observer> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(OobeUI);
};

}  

#endif  
