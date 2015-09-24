// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_TERMS_OF_SERVICE_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_TERMS_OF_SERVICE_SCREEN_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/login/screens/terms_of_service_screen_actor.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"

namespace chromeos {

class TermsOfServiceScreenHandler : public BaseScreenHandler,
                                    public TermsOfServiceScreenActor {
 public:
  TermsOfServiceScreenHandler();
  virtual ~TermsOfServiceScreenHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;

  
  virtual void SetDelegate(Delegate* screen) OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void SetDomain(const std::string& domain) OVERRIDE;
  virtual void OnLoadError() OVERRIDE;
  virtual void OnLoadSuccess(const std::string& terms_of_service) OVERRIDE;

 private:
  
  virtual void Initialize() OVERRIDE;

  
  void UpdateDomainInUI();

  
  
  
  void UpdateTermsOfServiceInUI();

  
  
  void HandleBack();

  
  
  void HandleAccept();

  TermsOfServiceScreenHandler::Delegate* screen_;

  
  bool show_on_init_;

  
  std::string domain_;

  
  bool load_error_;

  
  std::string terms_of_service_;

  DISALLOW_COPY_AND_ASSIGN(TermsOfServiceScreenHandler);
};

}  

#endif  
