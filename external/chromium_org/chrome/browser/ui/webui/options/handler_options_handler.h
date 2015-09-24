// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_HANDLER_OPTIONS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_HANDLER_OPTIONS_HANDLER_H_

#include <string>

#include "chrome/browser/custom_handlers/protocol_handler_registry.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "chrome/common/custom_handlers/protocol_handler.h"
#include "content/public/browser/notification_registrar.h"



namespace base {
class DictionaryValue;
}

namespace options {

class HandlerOptionsHandler : public OptionsPageUIHandler {
 public:
  HandlerOptionsHandler();
  virtual ~HandlerOptionsHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;
  virtual void InitializePage() OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  void SetHandlersEnabled(const ListValue* args);

  
  void SetDefault(const ListValue* args);

  
  
  void ClearDefault(const ListValue* args);

  
  
  ProtocolHandler ParseHandlerFromArgs(const ListValue* args) const;

  
  
  void GetHandlersForProtocol(const std::string& protocol,
                              base::DictionaryValue* value);

  
  void GetIgnoredHandlers(ListValue* handlers);

  
  void UpdateHandlerList();

  
  
  void RemoveHandler(const ListValue* args);

  
  
  void RemoveIgnoredHandler(const ListValue* args);

  ProtocolHandlerRegistry* GetProtocolHandlerRegistry();

  content::NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(HandlerOptionsHandler);
};

}  

#endif  
