// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_NETWORK_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_NETWORK_SCREEN_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/login/screens/network_screen_actor.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "ui/gfx/point.h"

namespace chromeos {

class CoreOobeActor;

struct NetworkScreenHandlerOnLanguageChangedCallbackData;

class NetworkScreenHandler : public NetworkScreenActor,
                             public BaseScreenHandler {
 public:
  explicit NetworkScreenHandler(CoreOobeActor* core_oobe_actor);
  virtual ~NetworkScreenHandler();

  
  virtual void SetDelegate(NetworkScreenActor::Delegate* screen) OVERRIDE;
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ShowError(const base::string16& message) OVERRIDE;
  virtual void ClearErrors() OVERRIDE;
  virtual void ShowConnectingStatus(bool connecting,
                                    const base::string16& network_id) OVERRIDE;
  virtual void EnableContinue(bool enabled) OVERRIDE;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void GetAdditionalParameters(base::DictionaryValue* dict) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleOnExit();

  
  void HandleOnLanguageChanged(const std::string& locale);

  
  static void OnLanguageChangedCallback(
      scoped_ptr<NetworkScreenHandlerOnLanguageChangedCallbackData> context,
      const std::string& requested_locale,
      const std::string& loaded_locale,
      const bool success);

  
  void HandleOnInputMethodChanged(const std::string& id);

  
  void HandleOnTimezoneChanged(const std::string& timezone);

  
  void OnSystemTimezoneChanged();

  
  
  static base::ListValue* GetLanguageList();

  
  
  static base::ListValue* GetInputMethods();

  
  static base::ListValue* GetTimezoneList();

  NetworkScreenActor::Delegate* screen_;
  CoreOobeActor* core_oobe_actor_;

  bool is_continue_enabled_;

  
  bool show_on_init_;

  
  gfx::Point network_control_pos_;

  scoped_ptr<CrosSettings::ObserverSubscription> timezone_subscription_;

  base::WeakPtrFactory<NetworkScreenHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkScreenHandler);
};

}  

#endif  
