// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCED_WINDOW_DELEGATE_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNCED_WINDOW_DELEGATE_H_

#include <set>

#include "chrome/browser/sessions/session_id.h"

namespace browser_sync {

class SyncedTabDelegate;

class SyncedWindowDelegate {
 public:
  

  
  static const std::set<SyncedWindowDelegate*> GetSyncedWindowDelegates();

  
  static const SyncedWindowDelegate* FindSyncedWindowDelegateWithId(
      SessionID::id_type id);

  

  
  
  
  virtual bool HasWindow() const = 0;

  
  virtual SessionID::id_type GetSessionId() const = 0;

  
  virtual int GetTabCount() const = 0;

  
  virtual int GetActiveIndex() const = 0;

  
  virtual bool IsApp() const = 0;

  
  virtual bool IsTypeTabbed() const = 0;

  
  virtual bool IsTypePopup() const = 0;

  

  
  virtual bool IsTabPinned(const SyncedTabDelegate* tab) const = 0;

  
  virtual SyncedTabDelegate* GetTabAt(int index) const = 0;

  
  virtual SessionID::id_type GetTabIdAt(int index) const = 0;

  
  virtual bool IsSessionRestoreInProgress() const = 0;

 protected:
  virtual ~SyncedWindowDelegate() {}
};

}  

#endif  
