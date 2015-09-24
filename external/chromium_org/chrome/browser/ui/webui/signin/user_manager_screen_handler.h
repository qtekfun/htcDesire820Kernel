// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SIGNIN_USER_MANAGER_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_SIGNIN_USER_MANAGER_SCREEN_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/host_desktop.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class DictionaryValue;
class FilePath;
class ListValue;
}

class UserManagerScreenHandler : public content::WebUIMessageHandler {
 public:
  UserManagerScreenHandler();
  virtual ~UserManagerScreenHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  void GetLocalizedValues(base::DictionaryValue* localized_strings);

 private:
  
  
  class ProfileUpdateObserver;

  void HandleInitialize(const base::ListValue* args);
  void HandleAddUser(const base::ListValue* args);
  void HandleAuthenticatedLaunchUser(const base::ListValue* args);
  void HandleLaunchGuest(const base::ListValue* args);
  void HandleLaunchUser(const base::ListValue* args);
  void HandleRemoveUser(const base::ListValue* args);

  
  void SendUserList();

  
  
  scoped_ptr<ProfileUpdateObserver> profileInfoCacheObserver_;

  
  chrome::HostDesktopType desktop_type_;

  DISALLOW_COPY_AND_ASSIGN(UserManagerScreenHandler);
};

#endif  
