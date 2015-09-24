// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FOREIGN_SESSION_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_FOREIGN_SESSION_HANDLER_H_
#pragma once

#include <vector>

#include "chrome/browser/sessions/session_service.h"
#include "chrome/browser/sync/glue/session_model_associator.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

namespace browser_sync {

class ForeignSessionHandler : public WebUIMessageHandler,
                              public NotificationObserver {
 public:
  
  virtual void RegisterMessages();

  ForeignSessionHandler();
  virtual ~ForeignSessionHandler() {}

 private:
  
  void Init();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  SessionModelAssociator* GetModelAssociator();

  
  
  
  void HandleOpenForeignSession(const ListValue* args);

  
  
  
  void HandleGetForeignSessions(const ListValue* args);

  
  bool SessionTabToValue(const SessionTab& tab, DictionaryValue* dictionary);
  bool SessionWindowToValue(const SessionWindow& window,
                            DictionaryValue* dictionary);

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(ForeignSessionHandler);
};

}  

#endif  
