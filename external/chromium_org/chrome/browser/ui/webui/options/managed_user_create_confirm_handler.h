// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGED_USER_CREATE_CONFIRM_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGED_USER_CREATE_CONFIRM_HANDLER_H_

#include "chrome/browser/ui/webui/options/options_ui.h"

namespace base {
class DictionaryValue;
}

namespace options {

class ManagedUserCreateConfirmHandler : public OptionsPageUIHandler {
 public:
  ManagedUserCreateConfirmHandler();
  virtual ~ManagedUserCreateConfirmHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  
  
  void SwitchToProfile(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(ManagedUserCreateConfirmHandler);
};

}  

#endif  
