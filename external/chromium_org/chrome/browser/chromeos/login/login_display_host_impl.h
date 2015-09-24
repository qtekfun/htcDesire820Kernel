// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_HOST_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_HOST_IMPL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/login/app_launch_controller.h"
#include "chrome/browser/chromeos/login/auth_prewarmer.h"
#include "chrome/browser/chromeos/login/existing_user_controller.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "chrome/browser/chromeos/login/login_display_host.h"
#include "chrome/browser/chromeos/login/wizard_controller.h"
#include "chrome/browser/chromeos/settings/device_settings_service.h"
#include "chromeos/audio/cras_audio_handler.h"
#include "chromeos/dbus/session_manager_client.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "ui/gfx/rect.h"

class PrefService;

namespace policy {
class AutoEnrollmentClient;
}  

namespace chromeos {

class FocusRingController;
class KeyboardDrivenOobeKeyHandler;
class OobeUI;
class WebUILoginDisplay;
class WebUILoginView;

class LoginDisplayHostImpl : public LoginDisplayHost,
                             public content::NotificationObserver,
                             public content::WebContentsObserver,
                             public chromeos::SessionManagerClient::Observer,
                             public chromeos::CrasAudioHandler::AudioObserver {
 public:
  explicit LoginDisplayHostImpl(const gfx::Rect& background_bounds);
  virtual ~LoginDisplayHostImpl();

  
  static LoginDisplayHost* default_host() {
    return default_host_;
  }

  
  virtual LoginDisplay* CreateLoginDisplay(
      LoginDisplay::Delegate* delegate) OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() const OVERRIDE;
  virtual WebUILoginView* GetWebUILoginView() const OVERRIDE;
  virtual void BeforeSessionStart() OVERRIDE;
  virtual void Finalize() OVERRIDE;
  virtual void OnCompleteLogin() OVERRIDE;
  virtual void OpenProxySettings() OVERRIDE;
  virtual void SetStatusAreaVisible(bool visible) OVERRIDE;
  virtual void CheckForAutoEnrollment() OVERRIDE;
  virtual void GetAutoEnrollmentCheckResult(
      const GetAutoEnrollmentCheckResultCallback& callback) OVERRIDE;
  virtual void StartWizard(
      const std::string& first_screen_name,
      scoped_ptr<DictionaryValue> screen_parameters) OVERRIDE;
  virtual WizardController* GetWizardController() OVERRIDE;
  virtual AppLaunchController* GetAppLaunchController() OVERRIDE;
  virtual void StartUserAdding(
      const base::Closure& completion_callback) OVERRIDE;
  virtual void StartSignInScreen(const LoginScreenContext& context) OVERRIDE;
  virtual void ResumeSignInScreen() OVERRIDE;
  virtual void OnPreferencesChanged() OVERRIDE;
  virtual void PrewarmAuthentication() OVERRIDE;
  virtual void StartAppLaunch(const std::string& app_id) OVERRIDE;

  
  WizardController* CreateWizardController();

  
  void OnBrowserCreated();

  
  OobeUI* GetOobeUI() const;

  const gfx::Rect& background_bounds() const { return background_bounds_; }

  
  
  static const int kShowLoginWebUIid;

  views::Widget* login_window_for_test() { return login_window_; }

 protected:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  
  virtual void EmitLoginPromptVisibleCalled() OVERRIDE;

  
  virtual void OnActiveOutputNodeChanged() OVERRIDE;

 private:
  
  enum RestorePath {
    RESTORE_UNKNOWN,
    RESTORE_WIZARD,
    RESTORE_SIGN_IN,
    RESTORE_ADD_USER_INTO_SESSION,
  };

  
  enum FinalizeAnimationType {
    ANIMATION_NONE,       
    ANIMATION_WORKSPACE,  
                          
    ANIMATION_FADE_OUT,   
  };

  
  
  void ShutdownDisplayHost(bool post_quit_task);

  
  void ScheduleWorkspaceAnimation();

  
  void ScheduleFadeOutAnimation();

  
  void OnOwnershipStatusCheckDone(
      DeviceSettingsService::OwnershipStatus status);

  
  void OnAutoEnrollmentClientDone();

  
  void ForceAutoEnrollment();

  
  void LoadURL(const GURL& url);

  
  void ShowWebUI();

  
  
  void StartPostponedWebUI();

  
  void InitLoginWindowAndView();

  
  void ResetLoginWindowAndView();

  
  void OnAuthPrewarmDone();

  
  void SetOobeProgressBarVisible(bool visible);

  
  void NotifyAutoEnrollmentCheckResult(bool should_auto_enroll);

  
  
  
  void TryToPlayStartupSound();

  
  void OnLoginPromptVisible();

  
  gfx::Rect background_bounds_;

  content::NotificationRegistrar registrar_;

  base::WeakPtrFactory<LoginDisplayHostImpl> pointer_factory_;

  
  static LoginDisplayHost* default_host_;

  
  scoped_ptr<ExistingUserController> sign_in_controller_;

  
  scoped_ptr<WizardController> wizard_controller_;

  
  scoped_ptr<AppLaunchController> app_launch_controller_;

  
  scoped_ptr<policy::AutoEnrollmentClient> auto_enrollment_client_;

  
  
  
  bool shutting_down_;

  
  bool oobe_progress_bar_visible_;

  
  bool session_starting_;

  
  views::Widget* login_window_;

  
  WebUILoginView* login_view_;

  
  WebUILoginDisplay* webui_login_display_;

  
  bool is_showing_login_;

  
  
  bool is_wallpaper_loaded_;

  
  
  bool status_area_saved_visibility_;

  
  
  
  
  bool initialize_webui_hidden_;

  
  
  bool waiting_for_wallpaper_load_;

  
  
  bool waiting_for_user_pods_;

  
  int crash_count_;

  
  RestorePath restore_path_;

  
  std::string wizard_first_screen_name_;
  scoped_ptr<DictionaryValue> wizard_screen_parameters_;

  
  
  bool old_ignore_solo_window_frame_painter_policy_value_;

  
  base::Closure completion_callback_;

  
  scoped_ptr<AuthPrewarmer> auth_prewarmer_;

  
  
  scoped_ptr<FocusRingController> focus_ring_controller_;

  
  scoped_ptr<KeyboardDrivenOobeKeyHandler> keyboard_driven_oobe_key_handler_;

  
  bool auto_enrollment_check_done_;

  
  std::vector<GetAutoEnrollmentCheckResultCallback>
      get_auto_enrollment_result_callbacks_;

  FinalizeAnimationType finalize_animation_type_;

  base::WeakPtrFactory<LoginDisplayHostImpl> animation_weak_ptr_factory_;

  
  
  base::TimeTicks login_prompt_visible_time_;

  
  
  bool startup_sound_played_;

  
  
  
  bool startup_sound_honors_spoken_feedback_;

  DISALLOW_COPY_AND_ASSIGN(LoginDisplayHostImpl);
};

}  

#endif  
