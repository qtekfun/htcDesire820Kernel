// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNC_STARTUP_TRACKER_H_
#define CHROME_BROWSER_SYNC_SYNC_STARTUP_TRACKER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"

class Profile;

class SyncStartupTracker : public ProfileSyncServiceObserver {
 public:
  
  class Observer {
   public:
    virtual void SyncStartupCompleted() = 0;
    virtual void SyncStartupFailed() = 0;
  };

  SyncStartupTracker(Profile* profile, Observer* observer);
  virtual ~SyncStartupTracker();

  enum SyncServiceState {
    
    SYNC_STARTUP_PENDING,
    
    
    SYNC_STARTUP_ERROR,
    
    
    SYNC_STARTUP_COMPLETE
  };

  
  static SyncServiceState GetSyncServiceState(Profile* profile);

  
  virtual void OnStateChanged() OVERRIDE;

 private:
  
  
  
  void CheckServiceState();

  
  Profile* profile_;

  
  Observer* observer_;

  DISALLOW_COPY_AND_ASSIGN(SyncStartupTracker);
};

#endif  
