// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_CONTROLLER_H_
#pragma once

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "chrome/browser/chromeos/login/screen_observer.h"
#include "chrome/browser/chromeos/login/wizard_screen.h"
#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "ui/gfx/rect.h"

class PrefService;
class WizardContentsView;
class WizardScreen;

namespace chromeos {
class AccountScreen;
class EnterpriseEnrollmentScreen;
class EulaScreen;
class ExistingUserController;
class HTMLPageScreen;
class LoginDisplayHost;
class NetworkScreen;
class RegistrationScreen;
class UpdateScreen;
class UserImageScreen;
class WizardInProcessBrowserTest;
}

namespace gfx {
class Rect;
}

namespace views {
class Views;
class Widget;
class WidgetGtk;
}

class WizardController : public chromeos::ScreenObserver,
                         public WizardScreenDelegate {
 public:
  explicit WizardController(chromeos::LoginDisplayHost* host,
                            const gfx::Rect& screen_bounds);
  ~WizardController();

  
  static WizardController* default_controller() {
    return default_controller_;
  }

  
  static bool IsEulaAccepted();

  
  static bool IsOobeCompleted();

  
  static void MarkEulaAccepted();

  
  static void MarkOobeCompleted();

  
  static bool IsDeviceRegistered();

  
  static bool IsRegisterScreenDefined();

  
  static void MarkDeviceRegistered();

  
  static std::string GetInitialLocale();

  
  static void SetInitialLocale(const std::string& locale);

  
  
  
  void Init(const std::string& first_screen_name);

  
  views::View* contents() { return contents_; }

  
  void CancelOOBEUpdate();

  
  chromeos::NetworkScreen* GetNetworkScreen();
  chromeos::AccountScreen* GetAccountScreen();
  chromeos::UpdateScreen* GetUpdateScreen();
  chromeos::UserImageScreen* GetUserImageScreen();
  chromeos::EulaScreen* GetEulaScreen();
  chromeos::RegistrationScreen* GetRegistrationScreen();
  chromeos::HTMLPageScreen* GetHTMLPageScreen();
  chromeos::EnterpriseEnrollmentScreen* GetEnterpriseEnrollmentScreen();

  
  void ShowNetworkScreen();
  void ShowAccountScreen();
  void ShowUpdateScreen();
  void ShowUserImageScreen();
  void ShowEulaScreen();
  void ShowRegistrationScreen();
  void ShowHTMLPageScreen();
  void ShowEnterpriseEnrollmentScreen();

  
  void ShowLoginScreen();

  
  
  WizardScreen* current_screen() const { return current_screen_; }

  
  void set_observer(ScreenObserver* observer) { observer_ = observer; }

  
  void set_start_url(const GURL& start_url) { start_url_ = start_url; }

  
  void SkipRegistration();

  
  static void RegisterPrefs(PrefService* local_state);

  static const char kNetworkScreenName[];
  static const char kLoginScreenName[];
  static const char kAccountScreenName[];
  static const char kUpdateScreenName[];
  static const char kUserImageScreenName[];
  static const char kRegistrationScreenName[];
  static const char kOutOfBoxScreenName[];
  static const char kTestNoScreenName[];
  static const char kEulaScreenName[];
  static const char kHTMLPageScreenName[];
  static const char kEnterpriseEnrollmentScreenName[];

 private:
  
  void OnNetworkConnected();
  void OnNetworkOffline();
  void OnAccountCreateBack();
  void OnAccountCreated();
  void OnConnectionFailed();
  void OnUpdateCompleted();
  void OnEulaAccepted();
  void OnUpdateErrorCheckingForUpdate();
  void OnUpdateErrorUpdating();
  void OnUserImageSelected();
  void OnUserImageSkipped();
  void OnRegistrationSuccess();
  void OnRegistrationSkipped();
  void OnEnterpriseEnrollmentDone();
  void OnOOBECompleted();

  
  void InitiateOOBEUpdate();

  
  virtual void OnExit(ExitCodes exit_code);
  virtual void OnSetUserNamePassword(const std::string& username,
                                     const std::string& password);
  virtual void set_usage_statistics_reporting(bool val) {
    usage_statistics_reporting_ = val;
  }
  virtual bool usage_statistics_reporting() const {
    return usage_statistics_reporting_;
  }

  
  
  
  views::Widget* CreateScreenWindow(const gfx::Rect& bounds,
                                    bool initial_show);

  
  
  gfx::Rect GetWizardScreenBounds(int screen_width, int screen_height) const;

  
  void SetCurrentScreen(WizardScreen* screen);

  
  
  void SetCurrentScreenSmooth(WizardScreen* screen, bool use_smoothing);

  
  void SetStatusAreaVisible(bool visible);

  
  virtual views::View* GetWizardView();
  virtual chromeos::ScreenObserver* GetObserver(WizardScreen* screen);
  virtual void ShowCurrentScreen();

  
  
  void ShowFirstScreen(const std::string& first_screen_name);

  
  void Login(const std::string& username, const std::string& password);

  
  static void SetZeroDelays();

  
  views::Widget* widget_;

  
  views::View* contents_;

  
  gfx::Rect screen_bounds_;

  
  scoped_ptr<chromeos::NetworkScreen> network_screen_;
  scoped_ptr<chromeos::AccountScreen> account_screen_;
  scoped_ptr<chromeos::UpdateScreen> update_screen_;
  scoped_ptr<chromeos::UserImageScreen> user_image_screen_;
  scoped_ptr<chromeos::EulaScreen> eula_screen_;
  scoped_ptr<chromeos::RegistrationScreen> registration_screen_;
  scoped_ptr<chromeos::HTMLPageScreen> html_page_screen_;
  scoped_ptr<chromeos::EnterpriseEnrollmentScreen>
      enterprise_enrollment_screen_;

  
  WizardScreen* current_screen_;

  
  bool initial_show_;

  std::string username_;
  std::string password_;

  
  bool is_active_;

  
  bool is_official_build_;

  
  bool is_out_of_box_;

  
  std::string first_screen_name_;

  
  chromeos::LoginDisplayHost* host_;

  
  ScreenObserver* observer_;

  
  static WizardController* default_controller_;

  
  GURL start_url_;

  base::OneShotTimer<WizardController> smooth_show_timer_;

  
  
  bool usage_statistics_reporting_;

  FRIEND_TEST_ALL_PREFIXES(WizardControllerTest, SwitchLanguage);
  friend class WizardControllerFlowTest;
  friend class chromeos::WizardInProcessBrowserTest;

  DISALLOW_COPY_AND_ASSIGN(WizardController);
};

#endif  
