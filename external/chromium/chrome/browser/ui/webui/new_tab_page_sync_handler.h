// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NEW_TAB_PAGE_SYNC_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NEW_TAB_PAGE_SYNC_HANDLER_H_
#pragma once

#include <string>

#include "chrome/browser/sync/profile_sync_service.h"
#include "chrome/browser/sync/sync_ui_util.h"
#include "content/browser/webui/web_ui.h"

class ListValue;

class NewTabPageSyncHandler : public WebUIMessageHandler,
                              public ProfileSyncServiceObserver {
 public:
  NewTabPageSyncHandler();
  virtual ~NewTabPageSyncHandler();

  
  virtual WebUIMessageHandler* Attach(WebUI* web_ui);
  virtual void RegisterMessages();

  
  void HandleGetSyncMessage(const ListValue* args);
  
  void HandleSyncLinkClicked(const ListValue* args);

  
  virtual void OnStateChanged();

 private:
  enum MessageType {
    HIDE,
    SYNC_ERROR,
    SYNC_PROMO
  };
  
  void SendSyncMessageToPage(MessageType type,
                             std::string msg, std::string linktext);

  
  
  
  void BuildAndSendSyncStatus();

  
  void HideSyncStatusSection();

  
  static MessageType FromSyncStatusMessageType(
      sync_ui_util::MessageType type);

  
  ProfileSyncService* sync_service_;

  
  
  bool waiting_for_initial_page_load_;

  DISALLOW_COPY_AND_ASSIGN(NewTabPageSyncHandler);
};

#endif  
