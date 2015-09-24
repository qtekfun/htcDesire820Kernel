// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SESSIONS_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SESSIONS_HELPER_H_

#include <algorithm>
#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/sync/glue/synced_session.h"
#include "chrome/browser/sync/test/integration/sync_test.h"
#include "sync/syncable/nigori_util.h"

class GURL;

namespace sessions_helper {

typedef std::vector<const browser_sync::SyncedSession*> SyncedSessionVector;
typedef browser_sync::SyncedSession::SyncedWindowMap SessionWindowMap;

class ScopedWindowMap {
 public:
  ScopedWindowMap();
  explicit ScopedWindowMap(SessionWindowMap* windows);
  ~ScopedWindowMap();

  const SessionWindowMap* Get() const;
  SessionWindowMap* GetMutable();
  void Reset(SessionWindowMap* windows);
 private:
  SessionWindowMap windows_;
};

bool GetLocalWindows(int index, SessionWindowMap* local_windows);

bool OpenTabAndGetLocalWindows(int index,
                               const GURL& url,
                               SessionWindowMap* local_windows);

bool CheckInitialState(int index);

int GetNumWindows(int index);

int GetNumForeignSessions(int index);

bool GetSessionData(int index, SyncedSessionVector* sessions);

bool CompareSyncedSessions(const browser_sync::SyncedSession* lhs,
                           const browser_sync::SyncedSession* rhs);

void SortSyncedSessions(SyncedSessionVector* sessions);

bool NavigationEquals(const sessions::SerializedNavigationEntry& expected,
                      const sessions::SerializedNavigationEntry& actual);

bool WindowsMatch(const SessionWindowMap& win1,
                  const SessionWindowMap& win2);

bool CheckForeignSessionsAgainst(
    int index,
    const std::vector<ScopedWindowMap>& windows);

bool OpenTab(int index, const GURL& url);

bool OpenMultipleTabs(int index, const std::vector<GURL>& urls);

bool WaitForTabsToLoad(int index, const std::vector<GURL>& urls);

bool ModelAssociatorHasTabWithUrl(int index, const GURL& url);

bool GetLocalSession(int index, const browser_sync::SyncedSession** session);

void DeleteForeignSession(int index, std::string session_tag);

}  

#endif  
