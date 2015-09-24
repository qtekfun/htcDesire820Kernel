// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_REGISTRATION_MANAGER_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_REGISTRATION_MANAGER_H_
#pragma once

#include <map>

#include "base/basictypes.h"
#include "base/time.h"
#include "base/timer.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "google/cacheinvalidation/invalidation-client.h"

namespace sync_notifier {

class RegistrationManager {
 public:
  
  static const int kInitialRegistrationDelaySeconds;
  static const int kRegistrationDelayExponent;
  static const double kRegistrationDelayMaxJitter;
  static const int kMinRegistrationDelaySeconds;
  static const int kMaxRegistrationDelaySeconds;

  
  struct PendingRegistrationInfo {
    PendingRegistrationInfo();

    
    base::Time last_registration_request;
    
    base::Time registration_attempt;
    
    
    base::TimeDelta delay;
    
    base::TimeDelta actual_delay;
  };
  
  
  typedef std::map<syncable::ModelType, PendingRegistrationInfo>
      PendingRegistrationMap;

  
  explicit RegistrationManager(
      invalidation::InvalidationClient* invalidation_client);

  virtual ~RegistrationManager();

  
  
  void SetRegisteredTypes(const syncable::ModelTypeSet& types);

  
  
  void MarkRegistrationLost(syncable::ModelType model_type);

  
  void MarkAllRegistrationsLost();

  

  
  syncable::ModelTypeSet GetRegisteredTypes() const;

  
  PendingRegistrationMap GetPendingRegistrations() const;

  
  void FirePendingRegistrationsForTest();

  
  
  static double CalculateBackoff(double retry_interval,
                                 double initial_retry_interval,
                                 double min_retry_interval,
                                 double max_retry_interval,
                                 double backoff_exponent,
                                 double jitter,
                                 double max_jitter);

 protected:
  
  virtual double GetJitter();

 private:
  struct RegistrationStatus {
    RegistrationStatus();
    ~RegistrationStatus();

    
    
    void DoRegister();

    
    syncable::ModelType model_type;
    
    RegistrationManager* registration_manager;

    
    invalidation::RegistrationState state;
    
    base::Time last_registration_request;
    
    base::Time last_registration_attempt;
    
    
    base::TimeDelta delay;
    
    
    base::TimeDelta next_delay;
    
    base::OneShotTimer<RegistrationStatus> registration_timer;
  };

  
  
  
  
  void TryRegisterType(syncable::ModelType model_type,
                       bool is_retry);

  
  
  
  
  void DoRegisterType(syncable::ModelType model_type);

  
  void UnregisterType(syncable::ModelType model_type);

  
  bool IsTypeRegistered(syncable::ModelType model_type) const;

  base::NonThreadSafe non_thread_safe_;
  RegistrationStatus registration_statuses_[syncable::MODEL_TYPE_COUNT];
  
  invalidation::InvalidationClient* invalidation_client_;

  DISALLOW_COPY_AND_ASSIGN(RegistrationManager);
};

}  

#endif  
