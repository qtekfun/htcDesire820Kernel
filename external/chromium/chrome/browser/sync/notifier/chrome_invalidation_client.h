// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_CHROME_INVALIDATION_CLIENT_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_CHROME_INVALIDATION_CLIENT_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_callback_factory.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync/notifier/chrome_system_resources.h"
#include "chrome/browser/sync/notifier/state_writer.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/syncable/model_type_payload_map.h"
#include "google/cacheinvalidation/invalidation-client.h"


namespace talk_base {
class Task;
}  

namespace sync_notifier {

class CacheInvalidationPacketHandler;
class RegistrationManager;

class ChromeInvalidationClient
    : public invalidation::InvalidationListener,
      public StateWriter {
 public:
  class Listener {
   public:
    virtual ~Listener();

    virtual void OnInvalidate(
        const syncable::ModelTypePayloadMap& type_payloads) = 0;

    virtual void OnSessionStatusChanged(bool has_session) = 0;
  };

  ChromeInvalidationClient();

  
  virtual ~ChromeInvalidationClient();

  
  
  void Start(
      const std::string& client_id, const std::string& client_info,
      const std::string& state, Listener* listener,
      StateWriter* state_writer, base::WeakPtr<talk_base::Task> base_task);

  void Stop();

  
  
  
  void ChangeBaseTask(base::WeakPtr<talk_base::Task> base_task);

  
  
  void RegisterTypes(const syncable::ModelTypeSet& types);

  
  virtual void Invalidate(const invalidation::Invalidation& invalidation,
                          invalidation::Closure* callback) OVERRIDE;
  virtual void InvalidateAll(invalidation::Closure* callback) OVERRIDE;
  virtual void RegistrationStateChanged(
      const invalidation::ObjectId& object_id,
      invalidation::RegistrationState new_state,
      const invalidation::UnknownHint& unknown_hint) OVERRIDE;
  virtual void AllRegistrationsLost(invalidation::Closure* callback) OVERRIDE;
  virtual void SessionStatusChanged(bool has_session) OVERRIDE;

  
  virtual void WriteState(const std::string& state) OVERRIDE;

 private:
  friend class ChromeInvalidationClientTest;

  
  void HandleOutboundPacket(
      invalidation::NetworkEndpoint* const& network_endpoint);
  void EmitInvalidation(
      const syncable::ModelTypeSet& types, const std::string& payload);

  base::NonThreadSafe non_thread_safe_;
  ChromeSystemResources chrome_system_resources_;
  base::ScopedCallbackFactory<ChromeInvalidationClient>
      scoped_callback_factory_;
  scoped_ptr<invalidation::NetworkCallback> handle_outbound_packet_callback_;
  Listener* listener_;
  StateWriter* state_writer_;
  scoped_ptr<invalidation::InvalidationClient> invalidation_client_;
  scoped_ptr<CacheInvalidationPacketHandler>
      cache_invalidation_packet_handler_;
  scoped_ptr<RegistrationManager> registration_manager_;
  std::map<syncable::ModelType, int64> max_invalidation_versions_;
  
  syncable::ModelTypeSet registered_types_;

  DISALLOW_COPY_AND_ASSIGN(ChromeInvalidationClient);
};

}  

#endif  
