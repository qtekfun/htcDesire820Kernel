// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_PAGE_SYNC_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_PAGE_SYNC_HANDLER_H_

#include <string>

#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/sync_ui_util.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
}

class NewTabPageSyncHandler : public content::WebUIMessageHandler,
                              public ProfileSyncServiceObserver {
 public:
  NewTabPageSyncHandler();
  virtual ~NewTabPageSyncHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleGetSyncMessage(const base::ListValue* args);
  
  void HandleSyncLinkClicked(const base::ListValue* args);

  
  virtual void OnStateChanged() OVERRIDE;

  void OnSigninAllowedPrefChange();

 private:
  friend class MockNewTabPageSyncHandler;
  FRIEND_TEST_ALL_PREFIXES(NewTabPageSyncHandlerBrowserTest,
                           ChangeSigninAllowedToFalse);

  enum MessageType {
    HIDE,
    SYNC_ERROR,
    SYNC_PROMO
  };
  
  virtual void SendSyncMessageToPage(MessageType type,
                                     std::string msg, std::string linktext);

  
  
  
  void BuildAndSendSyncStatus();

  
  void HideSyncStatusSection();

  
  static MessageType FromSyncStatusMessageType(
      sync_ui_util::MessageType type);

  
  ProfileSyncService* sync_service_;

  PrefChangeRegistrar profile_pref_registrar_;

  
  
  bool waiting_for_initial_page_load_;

  DISALLOW_COPY_AND_ASSIGN(NewTabPageSyncHandler);
};

#endif  
