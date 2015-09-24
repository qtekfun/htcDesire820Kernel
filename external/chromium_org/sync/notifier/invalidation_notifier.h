// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_INVALIDATION_NOTIFIER_H_
#define SYNC_NOTIFIER_INVALIDATION_NOTIFIER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/notifier/invalidation_state_tracker.h"
#include "sync/notifier/invalidator.h"
#include "sync/notifier/invalidator_registrar.h"
#include "sync/notifier/sync_invalidation_listener.h"

namespace notifier {
class PushClient;
}  

namespace syncer {

class SYNC_EXPORT_PRIVATE InvalidationNotifier
    : public Invalidator,
      public SyncInvalidationListener::Delegate,
      public base::NonThreadSafe {
 public:
  
  InvalidationNotifier(
      scoped_ptr<notifier::PushClient> push_client,
      const std::string& invalidator_client_id,
      const UnackedInvalidationsMap& saved_invalidations,
      const std::string& invalidation_bootstrap_data,
      const WeakHandle<InvalidationStateTracker>&
          invalidation_state_tracker,
      const std::string& client_info);

  virtual ~InvalidationNotifier();

  
  virtual void RegisterHandler(InvalidationHandler* handler) OVERRIDE;
  virtual void UpdateRegisteredIds(InvalidationHandler* handler,
                                   const ObjectIdSet& ids) OVERRIDE;
  virtual void UnregisterHandler(InvalidationHandler* handler) OVERRIDE;
  virtual InvalidatorState GetInvalidatorState() const OVERRIDE;
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) OVERRIDE;

  
  virtual void OnInvalidate(
      const ObjectIdInvalidationMap& invalidation_map) OVERRIDE;
  virtual void OnInvalidatorStateChange(InvalidatorState state) OVERRIDE;

 private:
  
  
  
  
  enum State {
    STOPPED,
    CONNECTING,
    STARTED
  };
  State state_;

  InvalidatorRegistrar registrar_;

  
  const UnackedInvalidationsMap saved_invalidations_;

  
  const WeakHandle<InvalidationStateTracker>
      invalidation_state_tracker_;

  
  const std::string client_info_;

  
  const std::string invalidator_client_id_;

  
  const std::string invalidation_bootstrap_data_;

  
  SyncInvalidationListener invalidation_listener_;

  DISALLOW_COPY_AND_ASSIGN(InvalidationNotifier);
};

}  

#endif  
