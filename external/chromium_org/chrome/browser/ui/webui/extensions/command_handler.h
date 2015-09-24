// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_EXTENSIONS_COMMAND_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_EXTENSIONS_COMMAND_HANDLER_H_

#include "base/compiler_specific.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace content {
class WebUIDataSource;
}

namespace extensions {
class Command;
class CommandService;
}

class Extension;
class Profile;

namespace extensions {

class CommandHandler : public content::WebUIMessageHandler,
                       public content::NotificationObserver {
 public:
  explicit CommandHandler(Profile* profile);
  virtual ~CommandHandler();

  
  void GetLocalizedValues(content::WebUIDataSource* source);

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
   
  void UpdateCommandDataOnPage();

  
  
  
  void HandleRequestExtensionsData(const base::ListValue* args);

  
  
  void HandleSetExtensionCommandShortcut(const base::ListValue* args);

  
  
  void HandleSetCommandScope(const base::ListValue* args);

  
  
  void HandleSetShortcutHandlingSuspended(const base::ListValue* args);

  
  
  void GetAllCommands(base::DictionaryValue* commands);

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(CommandHandler);
};

}  

#endif  
