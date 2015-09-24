// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_POLICY_NOTIFIER_H_
#define CHROME_BROWSER_POLICY_POLICY_NOTIFIER_H_
#pragma once

#include "base/observer_list.h"
#include "chrome/browser/policy/cloud_policy_subsystem.h"

namespace policy {

class PolicyNotifier {
 public:
  typedef CloudPolicySubsystem::PolicySubsystemState PolicySubsystemState;
  typedef CloudPolicySubsystem::ErrorDetails ErrorDetails;

  enum StatusSource {
    TOKEN_FETCHER,
    POLICY_CONTROLLER,
    POLICY_CACHE,
    NUM_SOURCES  
  };

  PolicyNotifier();
  ~PolicyNotifier();

  
  
  void Inform(PolicySubsystemState state,
              ErrorDetails error_details,
              StatusSource source);

  CloudPolicySubsystem::PolicySubsystemState state() const {
    return state_;
  }

  CloudPolicySubsystem::ErrorDetails error_details() const {
    return error_details_;
  }

 private:
  friend class CloudPolicyController;
  friend class CloudPolicySubsystem::ObserverRegistrar;

  void AddObserver(CloudPolicySubsystem::Observer* observer);
  void RemoveObserver(CloudPolicySubsystem::Observer* observer);

  void RecomputeState();

  PolicySubsystemState state_;
  ErrorDetails error_details_;

  PolicySubsystemState component_states_[NUM_SOURCES];
  ErrorDetails component_error_details_[NUM_SOURCES];

  ObserverList<CloudPolicySubsystem::Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(PolicyNotifier);
};

}  

#endif  
