// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_BACKGROUND_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_BACKGROUND_VIEW_H_
#pragma once

#include "chrome/browser/chromeos/boot_times_loader.h"
#include "chrome/browser/chromeos/cros/cros_library.h"
#include "chrome/browser/chromeos/login/login_html_dialog.h"
#include "chrome/browser/chromeos/status/status_area_host.h"
#include "chrome/browser/chromeos/version_loader.h"
#include "chrome/browser/policy/cloud_policy_subsystem.h"
#include "views/view.h"

namespace views {
class Label;
class TextButton;
class Widget;
class WindowDelegate;
}

class DOMView;
class GURL;
class Profile;

namespace chromeos {

class OobeProgressBar;
class ShutdownButton;
class StatusAreaView;

class BackgroundView : public views::View,
                       public StatusAreaHost,
                       public chromeos::LoginHtmlDialog::Delegate,
                       public policy::CloudPolicySubsystem::Observer {
 public:
  enum LoginStep {
    SELECT_NETWORK,
    EULA,
    SIGNIN,
    REGISTRATION,
    PICTURE,

    
    STEPS_COUNT
  };

  BackgroundView();

  
  
  void Init(const GURL& background_url);

  
  void EnableShutdownButton(bool enable);

  
  
  
  
  static views::Widget* CreateWindowContainingView(
      const gfx::Rect& bounds,
      const GURL& background_url,
      BackgroundView** view);

  
  void CreateModalPopup(views::WindowDelegate* view);

  
  virtual gfx::NativeWindow GetNativeWindow() const;

  
  void SetStatusAreaVisible(bool visible);

  
  void SetStatusAreaEnabled(bool enable);

  
  void SetOobeProgressBarVisible(bool visible);

  
  bool IsOobeProgressBarVisible();

  
  void SetOobeProgress(LoginStep step);

  
  void ShowScreenSaver();

  
  void HideScreenSaver();

  
  bool IsScreenSaverVisible();

  
  bool ScreenSaverEnabled();

 protected:
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void OnLocaleChanged() OVERRIDE;

  
  virtual Profile* GetProfile() const OVERRIDE { return NULL; }
  virtual void ExecuteBrowserCommand(int id) const OVERRIDE {}
  virtual bool ShouldOpenButtonOptions(
      const views::View* button_view) const OVERRIDE;
  virtual void OpenButtonOptions(const views::View* button_view) OVERRIDE;
  virtual ScreenMode GetScreenMode() const OVERRIDE;
  virtual TextStyle GetTextStyle() const OVERRIDE;

  
  virtual void OnDialogClosed() OVERRIDE {}

 private:
  
  void InitStatusArea();
  
  void InitInfoLabels();
  
  void InitProgressBar();

  
  
  void UpdateWindowType();

  
  void UpdateVersionLabel();

  
  void UpdateEnterpriseInfo();

  
  void SetEnterpriseInfo(const std::string& domain_name,
                         const std::string& status_text);

  
  void OnVersion(VersionLoader::Handle handle, std::string version);
  
  void OnBootTimes(
      BootTimesLoader::Handle handle, BootTimesLoader::BootTimes boot_times);

  
  void OnPolicyStateChanged(
      policy::CloudPolicySubsystem::PolicySubsystemState state,
      policy::CloudPolicySubsystem::ErrorDetails error_details);

  
  StatusAreaView* status_area_;
  views::Label* os_version_label_;
  views::Label* boot_times_label_;
  OobeProgressBar* progress_bar_;
  ShutdownButton* shutdown_button_;

  
  VersionLoader version_loader_;
  
  CancelableRequestConsumer version_consumer_;

  
  BootTimesLoader boot_times_loader_;
  
  CancelableRequestConsumer boot_times_consumer_;

  
  
  
  bool did_paint_;

  
  bool is_official_build_;

  
  DOMView* background_area_;

  
  std::string version_text_;
  std::string enterprise_domain_text_;
  std::string enterprise_status_text_;

  
  scoped_ptr<LoginHtmlDialog> proxy_settings_dialog_;

  
  scoped_ptr<policy::CloudPolicySubsystem::ObserverRegistrar>
      cloud_policy_registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundView);
};

}  

#endif  
