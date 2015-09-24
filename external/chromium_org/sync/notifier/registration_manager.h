// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_NOTIFIER_REGISTRATION_MANAGER_H_
#define SYNC_NOTIFIER_REGISTRATION_MANAGER_H_

#include <map>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "google/cacheinvalidation/include/invalidation-listener.h"
#include "google/cacheinvalidation/include/types.h"
#include "sync/base/sync_export.h"
#include "sync/notifier/invalidation_util.h"

namespace syncer {

using ::invalidation::InvalidationListener;

class SYNC_EXPORT_PRIVATE RegistrationManager : public base::NonThreadSafe {
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
  
  
  typedef std::map<invalidation::ObjectId,
                   PendingRegistrationInfo,
                   ObjectIdLessThan>
      PendingRegistrationMap;

  
  explicit RegistrationManager(
      invalidation::InvalidationClient* invalidation_client);

  virtual ~RegistrationManager();

  
  
  
  ObjectIdSet UpdateRegisteredIds(const ObjectIdSet& ids);

  
  
  void MarkRegistrationLost(const invalidation::ObjectId& id);

  
  void MarkAllRegistrationsLost();

  
  
  void DisableId(const invalidation::ObjectId& id);

  
  static double CalculateBackoff(double retry_interval,
                                 double initial_retry_interval,
                                 double min_retry_interval,
                                 double max_retry_interval,
                                 double backoff_exponent,
                                 double jitter,
                                 double max_jitter);

  

  
  ObjectIdSet GetRegisteredIdsForTest() const;

  
  PendingRegistrationMap GetPendingRegistrationsForTest() const;

  
  void FirePendingRegistrationsForTest();

 protected:
  
  virtual double GetJitter();

 private:
  struct RegistrationStatus {
    RegistrationStatus(const invalidation::ObjectId& id,
                       RegistrationManager* manager);
    ~RegistrationStatus();

    
    
    
    void DoRegister();

    
    void Disable();

    
    const invalidation::ObjectId id;
    
    RegistrationManager* const registration_manager;

    
    
    bool enabled;
    
    InvalidationListener::RegistrationState state;
    
    base::Time last_registration_request;
    
    base::Time last_registration_attempt;
    
    
    base::TimeDelta delay;
    
    
    base::TimeDelta next_delay;
    
    base::OneShotTimer<RegistrationStatus> registration_timer;

    DISALLOW_COPY_AND_ASSIGN(RegistrationStatus);
  };
  typedef std::map<invalidation::ObjectId,
                   RegistrationStatus*,
                   ObjectIdLessThan>
      RegistrationStatusMap;

  
  
  
  
  
  void TryRegisterId(const invalidation::ObjectId& id,
                     bool is_retry);

  
  
  
  
  void DoRegisterId(const invalidation::ObjectId& id);

  
  void UnregisterId(const invalidation::ObjectId& id);

  
  ObjectIdSet GetRegisteredIds() const;

  
  bool IsIdRegistered(const invalidation::ObjectId& id) const;

  RegistrationStatusMap registration_statuses_;
  
  invalidation::InvalidationClient* invalidation_client_;

  DISALLOW_COPY_AND_ASSIGN(RegistrationManager);
};

}  

#endif  
