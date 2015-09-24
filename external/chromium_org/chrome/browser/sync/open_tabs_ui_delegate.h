// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_OPEN_TABS_UI_DELEGATE_H_
#define CHROME_BROWSER_SYNC_OPEN_TABS_UI_DELEGATE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/glue/synced_session.h"

namespace browser_sync {

class OpenTabsUIDelegate {
 public:
  
  
  virtual bool GetSyncedFaviconForPageURL(
      const std::string& pageurl,
      scoped_refptr<base::RefCountedMemory>* favicon_png) const = 0;

  
  
  
  virtual bool GetAllForeignSessions(
      std::vector<const SyncedSession*>* sessions) = 0;

  
  
  
  virtual bool GetForeignTab(const std::string& tag,
                             const SessionID::id_type tab_id,
                             const SessionTab** tab) = 0;

  
  virtual void DeleteForeignSession(const std::string& tag) = 0;

  
  
  
  virtual bool GetForeignSession(
      const std::string& tag,
      std::vector<const SessionWindow*>* windows) = 0;
 protected:
  virtual ~OpenTabsUIDelegate();
};

}  

#endif  
