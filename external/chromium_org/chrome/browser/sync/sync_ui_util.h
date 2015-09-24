// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_UI_UTIL_H_
#define CHROME_BROWSER_SYNC_SYNC_UI_UTIL_H_

#include "base/strings/string16.h"

class ProfileSyncService;
class SigninManagerBase;

namespace sync_ui_util {

enum MessageType {
  PRE_SYNCED,  
  SYNCED,      
  SYNC_ERROR,  
  SYNC_PROMO,  
               
};

enum StatusLabelStyle {
  PLAIN_TEXT,  
  WITH_HTML    
};


MessageType GetStatusLabels(ProfileSyncService* service,
                            const SigninManagerBase& signin,
                            StatusLabelStyle style,
                            base::string16* status_label,
                            base::string16* link_label);

MessageType GetStatusLabelsForNewTabPage(ProfileSyncService* service,
                                         const SigninManagerBase& signin,
                                         base::string16* status_label,
                                         base::string16* link_label);

void GetStatusLabelsForSyncGlobalError(ProfileSyncService* service,
                                       const SigninManagerBase& signin,
                                       base::string16* menu_item_label,
                                       base::string16* bubble_message,
                                       base::string16* bubble_accept_label);

MessageType GetStatus(ProfileSyncService* service,
                      const SigninManagerBase& signin);

}  
#endif  
