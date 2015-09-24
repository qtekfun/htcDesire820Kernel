// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_INVALIDATION_STATE_TRACKER_H_
#define SYNC_NOTIFIER_INVALIDATION_STATE_TRACKER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "google/cacheinvalidation/include/types.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/invalidation.h"
#include "sync/notifier/invalidation_util.h"
#include "sync/notifier/unacked_invalidation_set.h"

namespace base {
class TaskRunner;
}  

namespace syncer {

class InvalidationStateTracker {
 public:
  InvalidationStateTracker() {}

  
  
  
  virtual void SetInvalidatorClientId(const std::string& data) = 0;
  virtual std::string GetInvalidatorClientId() const = 0;

  
  
  
  
  virtual void SetBootstrapData(const std::string& data) = 0;
  virtual std::string GetBootstrapData() const = 0;

  
  
  
  virtual void SetSavedInvalidations(const UnackedInvalidationsMap& states) = 0;
  virtual UnackedInvalidationsMap GetSavedInvalidations() const = 0;

  
  virtual void Clear() = 0;

 protected:
  virtual ~InvalidationStateTracker() {}
};

}  

#endif  
