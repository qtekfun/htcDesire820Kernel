// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_CONTROLLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/chromeos/login/screens/screen_observer.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

class PrefRegistrySimple;
class PrefService;

namespace base {
class DictionaryValue;
}

namespace chromeos {

class EnrollmentScreen;
class ErrorScreen;
class EulaScreen;
class KioskAutolaunchScreen;
class KioskEnableScreen;
class LocallyManagedUserCreationScreen;
class LoginDisplayHost;
class LoginScreenContext;
class NetworkScreen;
class OobeDisplay;
class ResetScreen;
class TermsOfServiceScreen;
class UpdateScreen;
class UserImageScreen;
class WizardScreen;
class WrongHWIDScreen;

class WizardController : public ScreenObserver {
 public:
  
  class Observer {
   public:
    
    virtual void OnScreenChanged(WizardScreen* next_screen) = 0;

    
    virtual void OnSessionStart() = 0;
  };

  WizardController(LoginDisplayHost* host, OobeDisplay* oobe_display);
  virtual ~WizardController();

  
  static WizardController* default_controller() {
    return default_controller_;
  }

  
  
  static bool skip_post_login_screens() {
    return skip_post_login_screens_;
  }

  
  static void SetZeroDelays();

  
  static bool IsZeroDelayEnabled();

  
  
  static void SkipPostLoginScreensForTesting();

  
  static bool ShouldAutoStartEnrollment();

  
  
  void Init(const std::string& first_screen_name,
            scoped_ptr<base::DictionaryValue> screen_parameters);

  
  void AdvanceToScreen(const std::string& screen_name);

  
  
  void AdvanceToScreenWithParams(const std::string& first_screen_name,
                                 base::DictionaryValue* screen_parameters);

  
  void SkipToLoginForTesting(const LoginScreenContext& context);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void OnSessionStart();

  
  void SkipUpdateEnrollAfterEula();

  
  
  void EnableUserImageScreenReturnToPreviousHack();

  
  NetworkScreen* GetNetworkScreen();
  UpdateScreen* GetUpdateScreen();
  UserImageScreen* GetUserImageScreen();
  EulaScreen* GetEulaScreen();
  EnrollmentScreen* GetEnrollmentScreen();
  ResetScreen* GetResetScreen();
  KioskAutolaunchScreen* GetKioskAutolaunchScreen();
  KioskEnableScreen* GetKioskEnableScreen();
  TermsOfServiceScreen* GetTermsOfServiceScreen();
  WrongHWIDScreen* GetWrongHWIDScreen();
  LocallyManagedUserCreationScreen* GetLocallyManagedUserCreationScreen();

  
  
  WizardScreen* current_screen() const { return current_screen_; }

  
  bool login_screen_started() const { return login_screen_started_; }

  static const char kNetworkScreenName[];
  static const char kLoginScreenName[];
  static const char kUpdateScreenName[];
  static const char kUserImageScreenName[];
  static const char kOutOfBoxScreenName[];
  static const char kTestNoScreenName[];
  static const char kEulaScreenName[];
  static const char kEnrollmentScreenName[];
  static const char kResetScreenName[];
  static const char kKioskEnableScreenName[];
  static const char kKioskAutolaunchScreenName[];
  static const char kErrorScreenName[];
  static const char kTermsOfServiceScreenName[];
  static const char kWrongHWIDScreenName[];
  static const char kLocallyManagedUserCreationScreenName[];
  static const char kAppLaunchSplashScreenName[];

 private:
  
  void ShowNetworkScreen();
  void ShowUpdateScreen();
  void ShowUserImageScreen();
  void ShowEulaScreen();
  void ShowEnrollmentScreen();
  void ShowResetScreen();
  void ShowKioskAutolaunchScreen();
  void ShowKioskEnableScreen();
  void ShowTermsOfServiceScreen();
  void ShowWrongHWIDScreen();
  void ShowLocallyManagedUserCreationScreen();

  
  void ShowLoginScreen(const LoginScreenContext& context);

  
  void ResumeLoginScreen();

  
  void OnNetworkConnected();
  void OnNetworkOffline();
  void OnConnectionFailed();
  void OnUpdateCompleted();
  void OnEulaAccepted();
  void OnUpdateErrorCheckingForUpdate();
  void OnUpdateErrorUpdating();
  void OnUserImageSelected();
  void OnUserImageSkipped();
  void OnEnrollmentDone();
  void OnAutoEnrollmentDone();
  void OnResetCanceled();
  void OnKioskAutolaunchCanceled();
  void OnKioskAutolaunchConfirmed();
  void OnKioskEnableCompleted();
  void OnWrongHWIDWarningSkipped();
  void OnOOBECompleted();
  void OnTermsOfServiceDeclined();
  void OnTermsOfServiceAccepted();

  
  void LoadBrandCodeFromFile();

  
  void OnEulaBlockingTasksDone();

  
  void InitiateOOBEUpdate();

  
  
  void PerformPostEulaActions();

  
  void PerformPostUpdateActions();

  
  virtual void OnExit(ExitCodes exit_code) OVERRIDE;
  virtual void ShowCurrentScreen() OVERRIDE;
  virtual void OnSetUserNamePassword(const std::string& username,
                                     const std::string& password) OVERRIDE;
  virtual void SetUsageStatisticsReporting(bool val) OVERRIDE;
  virtual bool GetUsageStatisticsReporting() const OVERRIDE;
  virtual ErrorScreen* GetErrorScreen() OVERRIDE;
  virtual void ShowErrorScreen() OVERRIDE;
  virtual void HideErrorScreen(WizardScreen* parent_screen) OVERRIDE;

  
  void SetCurrentScreen(WizardScreen* screen);

  
  
  void SetCurrentScreenSmooth(WizardScreen* screen, bool use_smoothing);

  
  void SetStatusAreaVisible(bool visible);

  
  void Login(const std::string& username, const std::string& password);

  
  void AutoLaunchKioskApp();

  
  bool CanExitEnrollment() const;

  
  void OnLocalStateInitialized(bool );

  
  PrefService* GetLocalState();

  static void set_local_state_for_testing(PrefService* local_state) {
    local_state_for_testing_ = local_state;
  }

  
  
  static bool skip_post_login_screens_;

  static bool zero_delay_enabled_;

  
  scoped_ptr<NetworkScreen> network_screen_;
  scoped_ptr<UpdateScreen> update_screen_;
  scoped_ptr<UserImageScreen> user_image_screen_;
  scoped_ptr<EulaScreen> eula_screen_;
  scoped_ptr<ResetScreen> reset_screen_;
  scoped_ptr<KioskAutolaunchScreen> autolaunch_screen_;
  scoped_ptr<KioskEnableScreen> kiosk_enable_screen_;
  scoped_ptr<EnrollmentScreen> enrollment_screen_;
  scoped_ptr<ErrorScreen> error_screen_;
  scoped_ptr<TermsOfServiceScreen> terms_of_service_screen_;
  scoped_ptr<WrongHWIDScreen> wrong_hwid_screen_;
  scoped_ptr<LocallyManagedUserCreationScreen>
      locally_managed_user_creation_screen_;

  
  WizardScreen* current_screen_;

  
  WizardScreen* previous_screen_;

  std::string username_;
  std::string password_;

  
  bool is_official_build_;

  
  bool is_out_of_box_;

  
  std::string first_screen_name_;

  
  LoginDisplayHost* host_;

  
  static WizardController* default_controller_;

  
  scoped_ptr<base::DictionaryValue> screen_parameters_;

  base::OneShotTimer<WizardController> smooth_show_timer_;

  OobeDisplay* oobe_display_;

  
  
  bool usage_statistics_reporting_;

  
  
  bool skip_update_enroll_after_eula_;

  
  
  base::Time time_eula_accepted_;

  ObserverList<Observer> observer_list_;

  bool login_screen_started_;

  
  
  bool user_image_screen_return_to_previous_hack_;

  
  static PrefService* local_state_for_testing_;

  FRIEND_TEST_ALL_PREFIXES(EnrollmentScreenTest, TestCancel);
  FRIEND_TEST_ALL_PREFIXES(WizardControllerFlowTest, Accelerators);
  friend class WizardControllerFlowTest;
  friend class WizardInProcessBrowserTest;
  friend class WizardControllerBrokenLocalStateTest;

  base::WeakPtrFactory<WizardController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WizardController);
};

}  

#endif  
