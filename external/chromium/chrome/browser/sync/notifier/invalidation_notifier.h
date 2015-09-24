// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_INVALIDATION_NOTIFIER_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_INVALIDATION_NOTIFIER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync/notifier/chrome_invalidation_client.h"
#include "chrome/browser/sync/notifier/state_writer.h"
#include "chrome/browser/sync/notifier/sync_notifier.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "jingle/notifier/base/notifier_options.h"
#include "jingle/notifier/communicator/login.h"

namespace sync_notifier {

class InvalidationNotifier
    : public SyncNotifier,
      public notifier::LoginDelegate,
      public ChromeInvalidationClient::Listener,
      public StateWriter {
 public:
  InvalidationNotifier(
      const notifier::NotifierOptions& notifier_options,
      const std::string& client_info);

  virtual ~InvalidationNotifier();

  
  virtual void AddObserver(SyncNotifierObserver* observer) OVERRIDE;
  virtual void RemoveObserver(SyncNotifierObserver* observer) OVERRIDE;
  virtual void SetState(const std::string& state) OVERRIDE;
  virtual void UpdateCredentials(
      const std::string& email, const std::string& token) OVERRIDE;
  virtual void UpdateEnabledTypes(
      const syncable::ModelTypeSet& types) OVERRIDE;
  virtual void SendNotification() OVERRIDE;

  
  virtual void OnConnect(base::WeakPtr<talk_base::Task> base_task) OVERRIDE;
  virtual void OnDisconnect() OVERRIDE;

  
  virtual void OnInvalidate(
      const syncable::ModelTypePayloadMap& type_payloads) OVERRIDE;
  virtual void OnSessionStatusChanged(bool has_session) OVERRIDE;

  
  virtual void WriteState(const std::string& state) OVERRIDE;

 private:
  base::NonThreadSafe non_thread_safe_;

  
  
  
  
  enum State {
    STOPPED,
    CONNECTING,
    STARTED
  };
  State state_;

  
  const notifier::NotifierOptions notifier_options_;

  
  const std::string client_info_;

  
  ObserverList<SyncNotifierObserver> observers_;

  
  std::string invalidation_state_;

  
  scoped_ptr<notifier::Login> login_;

  
  ChromeInvalidationClient invalidation_client_;

  DISALLOW_COPY_AND_ASSIGN(InvalidationNotifier);
};

}  

#endif  
