// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_KIOSK_ENABLE_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_KIOSK_ENABLE_SCREEN_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/app_mode/kiosk_app_manager.h"
#include "chrome/browser/chromeos/login/screens/kiosk_enable_screen_actor.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"

namespace chromeos {

class KioskEnableScreenHandler : public KioskEnableScreenActor,
                                 public BaseScreenHandler {
 public:
  KioskEnableScreenHandler();
  virtual ~KioskEnableScreenHandler();

  
  virtual void Show() OVERRIDE;
  virtual void SetDelegate(Delegate* delegate) OVERRIDE;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleOnClose();
  void HandleOnEnable();

  
  void OnEnableConsumerModeKiosk(bool success);

  
  void OnGetConsumerKioskModeStatus(
      KioskAppManager::ConsumerKioskModeStatus status);

  Delegate* delegate_;

  
  bool show_on_init_;

  
  bool is_configurable_;

  base::WeakPtrFactory<KioskEnableScreenHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(KioskEnableScreenHandler);
};

}  

#endif  
