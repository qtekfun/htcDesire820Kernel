// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_EULA_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_EULA_SCREEN_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/screens/eula_screen_actor.h"
#include "chrome/browser/chromeos/login/tpm_password_fetcher.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "content/public/browser/web_ui.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class CoreOobeActor;
class HelpAppLauncher;

class EulaScreenHandler : public EulaScreenActor,
                          public BaseScreenHandler,
                          public TpmPasswordFetcherDelegate {
 public:
  explicit EulaScreenHandler(CoreOobeActor* core_oobe_actor);
  virtual ~EulaScreenHandler();

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void SetDelegate(Delegate* delegate) OVERRIDE;
  virtual void OnPasswordFetched(const std::string& tpm_password) OVERRIDE;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void GetAdditionalParameters(base::DictionaryValue* dict) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleOnExit(bool accepted, bool usager_stats_enabled);
  void HandleOnLearnMore();
  void HandleOnInstallationSettingsPopupOpened();

  EulaScreenActor::Delegate* delegate_;
  CoreOobeActor* core_oobe_actor_;

  
  scoped_refptr<HelpAppLauncher> help_app_;

  
  bool show_on_init_;

  DISALLOW_COPY_AND_ASSIGN(EulaScreenHandler);
};

}  

#endif  
