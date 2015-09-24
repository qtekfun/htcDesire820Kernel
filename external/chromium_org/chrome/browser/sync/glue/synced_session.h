// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCED_SESSION_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNCED_SESSION_H_

#include <map>
#include <string>

#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"

namespace content {
class NavigationEntry;
}

namespace browser_sync {

struct SyncedSession {
  typedef std::map<SessionID::id_type, SessionWindow*> SyncedWindowMap;

  
  
  enum DeviceType {
    TYPE_UNSET = 0,
    TYPE_WIN = 1,
    TYPE_MACOSX = 2,
    TYPE_LINUX = 3,
    TYPE_CHROMEOS = 4,
    TYPE_OTHER = 5,
    TYPE_PHONE = 6,
    TYPE_TABLET = 7
  };

  SyncedSession();
  ~SyncedSession();

  
  std::string session_tag;
  
  std::string session_name;

  
  DeviceType device_type;

  
  base::Time modified_time;

  
  
  SyncedWindowMap windows;

  
  
  
  std::string DeviceTypeAsString() const {
    switch (device_type) {
      case SyncedSession::TYPE_WIN:
        return "win";
      case SyncedSession::TYPE_MACOSX:
        return "macosx";
      case SyncedSession::TYPE_LINUX:
        return "linux";
      case SyncedSession::TYPE_CHROMEOS:
        return "chromeos";
      case SyncedSession::TYPE_OTHER:
        return "other";
      case SyncedSession::TYPE_PHONE:
        return "phone";
      case SyncedSession::TYPE_TABLET:
        return "tablet";
      default:
        return std::string();
    }
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SyncedSession);
};

bool ShouldSyncSessionTab(const SessionTab& tab);

bool SessionWindowHasNoTabsToSync(const SessionWindow& window);

}  

#endif  
