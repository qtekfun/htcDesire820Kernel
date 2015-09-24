// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_SYNC_NOTIFIER_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_SYNC_NOTIFIER_H_

#include <string>

#include "chrome/browser/sync/syncable/model_type.h"

namespace sync_notifier {
class SyncNotifierObserver;

class SyncNotifier {
 public:
  SyncNotifier() {}
  virtual ~SyncNotifier() {}

  virtual void AddObserver(SyncNotifierObserver* observer) = 0;
  virtual void RemoveObserver(SyncNotifierObserver* observer) = 0;

  
  virtual void SetState(const std::string& state) = 0;

  
  
  
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) = 0;

  virtual void UpdateEnabledTypes(const syncable::ModelTypeSet& types) = 0;

  
  
  
  
  virtual void SendNotification() = 0;
};
}  

#endif  

