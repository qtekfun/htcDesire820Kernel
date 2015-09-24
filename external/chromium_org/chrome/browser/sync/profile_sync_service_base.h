// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_BASE_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_BASE_H_

#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "sync/internal_api/public/base/model_type.h"

namespace content {
class BrowserContext;
}

class ProfileSyncServiceBase {
 public:
  
  
  static ProfileSyncServiceBase* FromBrowserContext(
      content::BrowserContext* context);

  typedef ProfileSyncServiceObserver Observer;

  virtual ~ProfileSyncServiceBase() {}

  
  virtual bool HasSyncSetupCompleted() const = 0;

  
  
  virtual bool ShouldPushChanges() = 0;

  
  
  virtual syncer::ModelTypeSet GetActiveDataTypes() const = 0;

  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual bool HasObserver(Observer* observer) const = 0;
};

#endif  
