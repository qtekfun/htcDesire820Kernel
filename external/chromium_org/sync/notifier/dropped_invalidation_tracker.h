// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_DROPPED_INVALIDATION_TRACKER_H_
#define SYNC_NOTIFIER_DROPPED_INVALIDATION_TRACKER_H_

#include "google/cacheinvalidation/include/types.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/ack_handle.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/notifier/ack_handler.h"

namespace syncer {

class Invalidation;

class SYNC_EXPORT DroppedInvalidationTracker {
 public:
  explicit DroppedInvalidationTracker(const invalidation::ObjectId& id);
  ~DroppedInvalidationTracker();

  const invalidation::ObjectId& object_id() const;

  
  
  
  
  
  void RecordDropEvent(WeakHandle<AckHandler> handler, AckHandle handle);

  
  bool IsRecoveringFromDropEvent() const;

  
  void RecordRecoveryFromDropEvent();

 private:
  invalidation::ObjectId id_;
  AckHandle drop_ack_handle_;

  
  
  
  
  
  
  WeakHandle<AckHandler> drop_ack_handler_;

  DISALLOW_COPY_AND_ASSIGN(DroppedInvalidationTracker);
};

}  

#endif  
