// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_MOCK_ACK_HANDLER_H_
#define SYNC_NOTIFIER_MOCK_ACK_HANDLER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/notifier/ack_handler.h"

namespace syncer {

class Invalidation;

class SYNC_EXPORT MockAckHandler
  : public AckHandler,
    public base::SupportsWeakPtr<MockAckHandler> {
 public:
  MockAckHandler();
  virtual ~MockAckHandler();

  
  
  void RegisterInvalidation(Invalidation* invalidation);

  
  
  void RegisterUnsentInvalidation(Invalidation* invalidation);

  
  
  bool IsUnacked(const Invalidation& invalidation) const;

  
  bool IsUnsent(const Invalidation& invalidation) const;

  
  virtual void Acknowledge(
      const invalidation::ObjectId& id,
      const AckHandle& handle) OVERRIDE;
  virtual void Drop(
      const invalidation::ObjectId& id,
      const AckHandle& handle) OVERRIDE;

 private:
  typedef std::vector<syncer::Invalidation> InvalidationVector;

  WeakHandle<AckHandler> WeakHandleThis();

  InvalidationVector unsent_invalidations_;
  InvalidationVector unacked_invalidations_;
  InvalidationVector acked_invalidations_;
};

}  

#endif  
