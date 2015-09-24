// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_UI_UTIL_H_
#define CHROME_BROWSER_SYNC_SYNC_UI_UTIL_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "base/values.h"
#include "chrome/browser/sync/profile_sync_service.h"

class Browser;
class Profile;
class ListValue;
class DictionaryValue;

namespace sync_ui_util {

enum MessageType {
  PRE_SYNCED,  
  SYNCED,      
  SYNC_ERROR,  
  SYNC_PROMO,  
               
};


MessageType GetStatusLabels(ProfileSyncService* service,
                            string16* status_label,
                            string16* link_label);

MessageType GetStatusLabelsForNewTabPage(ProfileSyncService* service,
                                         string16* status_label,
                                         string16* link_label);

MessageType GetStatus(ProfileSyncService* service);

bool ShouldShowSyncErrorButton(ProfileSyncService* service);

string16 GetSyncMenuLabel(ProfileSyncService* service);

void OpenSyncMyBookmarksDialog(Profile* profile,
                               Browser* browser,
                               ProfileSyncService::SyncEventCodes code);

void AddBoolSyncDetail(ListValue* details,
                       const std::string& stat_name,
                       bool stat_value);

void ConstructAboutInformation(ProfileSyncService* service,
                               DictionaryValue* strings);

void AddIntSyncDetail(ListValue* details,
                      const std::string& stat_name,
                      int64 stat_value);
}  
#endif  
