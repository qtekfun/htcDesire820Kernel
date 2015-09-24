// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_USER_ACTIONS_USER_ACTIONS_UI_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_USER_ACTIONS_USER_ACTIONS_UI_HANDLER_H_

#include "content/public/browser/user_metrics.h"
#include "content/public/browser/web_ui_message_handler.h"

class UserActionsUIHandler : public content::WebUIMessageHandler {
 public:
  UserActionsUIHandler();
  virtual ~UserActionsUIHandler();

  
  
  virtual void RegisterMessages() OVERRIDE;

 private:
  void OnUserAction(const std::string& action);

  content::ActionCallback action_callback_;

  DISALLOW_COPY_AND_ASSIGN(UserActionsUIHandler);
};

#endif  
