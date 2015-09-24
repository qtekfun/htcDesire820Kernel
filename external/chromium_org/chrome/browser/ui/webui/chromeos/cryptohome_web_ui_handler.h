// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_CRYPTOHOME_WEB_UI_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_CRYPTOHOME_WEB_UI_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/dbus_method_call_status.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {

class Value;

}  

namespace chromeos {

class CryptohomeWebUIHandler : public content::WebUIMessageHandler {
 public:
  CryptohomeWebUIHandler();

  virtual ~CryptohomeWebUIHandler();

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void OnPageLoaded(const base::ListValue* args);

  void DidGetNSSUtilInfoOnUIThread(bool is_tpm_token_ready);

  
  BoolDBusMethodCallback GetCryptohomeBoolCallback(
      const std::string& destination_id);

  
  void OnCryptohomeBoolProperty(const std::string& destination_id,
                                DBusMethodCallStatus call_status,
                                bool value);

  
  void SetCryptohomeProperty(const std::string& destination_id,
                             const base::Value& value);

  base::WeakPtrFactory<CryptohomeWebUIHandler> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(CryptohomeWebUIHandler);
};

}  

#endif  
