// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_INVALIDATOR_H_
#define SYNC_NOTIFIER_INVALIDATOR_H_

#include <string>

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/notifier/invalidation_util.h"
#include "sync/notifier/invalidator_state.h"

namespace syncer {
class InvalidationHandler;

class SYNC_EXPORT Invalidator {
 public:
  Invalidator() {}
  virtual ~Invalidator() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  virtual void RegisterHandler(InvalidationHandler* handler) = 0;

  
  
  
  virtual void UpdateRegisteredIds(InvalidationHandler* handler,
                                   const ObjectIdSet& ids) = 0;

  
  
  
  virtual void UnregisterHandler(InvalidationHandler* handler) = 0;

  
  
  
  virtual InvalidatorState GetInvalidatorState() const = 0;

  
  
  
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) = 0;
};
}  

#endif  
