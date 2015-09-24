// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_ENTERPRISE_ENROLLMENT_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_ENTERPRISE_ENROLLMENT_UI_H_
#pragma once

#include "base/compiler_specific.h"
#include "content/browser/webui/web_ui.h"

namespace chromeos {

class EnterpriseEnrollmentUI : public WebUI {
 public:
  
  
  class Controller {
   public:
    virtual ~Controller() {}

    virtual void OnAuthSubmitted(const std::string& user,
                                 const std::string& password,
                                 const std::string& captcha,
                                 const std::string& access_code) = 0;
    virtual void OnAuthCancelled() = 0;
    virtual void OnConfirmationClosed() = 0;
    virtual bool GetInitialUser(std::string* user) = 0;
  };

  explicit EnterpriseEnrollmentUI(TabContents* contents);
  virtual ~EnterpriseEnrollmentUI();

  
  virtual void RenderViewCreated(RenderViewHost* render_view_host) OVERRIDE;

  
  static GURL GetURL();

  
  
  static Controller* GetController(WebUI* web_ui);

  
  static void SetController(TabContents* contents, Controller* controller);

 private:
  DISALLOW_COPY_AND_ASSIGN(EnterpriseEnrollmentUI);
};

}  

#endif  
