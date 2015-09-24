// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_INVALIDATION_HANDLER_H_
#define SYNC_NOTIFIER_INVALIDATION_HANDLER_H_

#include "sync/base/sync_export.h"
#include "sync/notifier/invalidator_state.h"

namespace syncer {

class ObjectIdInvalidationMap;

class SYNC_EXPORT InvalidationHandler {
 public:
  
  virtual void OnInvalidatorStateChange(InvalidatorState state) = 0;

  
  
  
  virtual void OnIncomingInvalidation(
      const ObjectIdInvalidationMap& invalidation_map) = 0;

 protected:
  virtual ~InvalidationHandler() {}
};

}  

#endif  
