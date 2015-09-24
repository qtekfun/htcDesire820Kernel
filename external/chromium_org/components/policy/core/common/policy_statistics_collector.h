// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_STATISTICS_COLLECTOR_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_STATISTICS_COLLECTOR_H_

#include "base/basictypes.h"
#include "base/cancelable_callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "components/policy/core/common/policy_details.h"
#include "components/policy/core/common/schema.h"
#include "components/policy/policy_export.h"

class PrefService;
class PrefRegistrySimple;

namespace base {
class TaskRunner;
}

namespace policy {

class PolicyService;

class POLICY_EXPORT PolicyStatisticsCollector {
 public:
  
  static const int kStatisticsUpdateRate;

  
  
  PolicyStatisticsCollector(const GetChromePolicyDetailsCallback& get_details,
                            const Schema& chrome_schema,
                            PolicyService* policy_service,
                            PrefService* prefs,
                            const scoped_refptr<base::TaskRunner>& task_runner);
  virtual ~PolicyStatisticsCollector();

  
  void Initialize();

  static void RegisterPrefs(PrefRegistrySimple* registry);

 protected:
  
  virtual void RecordPolicyUse(int id);

 private:
  void CollectStatistics();
  void ScheduleUpdate(base::TimeDelta delay);

  GetChromePolicyDetailsCallback get_details_;
  Schema chrome_schema_;
  PolicyService* policy_service_;
  PrefService* prefs_;

  base::CancelableClosure update_callback_;

  const scoped_refptr<base::TaskRunner> task_runner_;

  DISALLOW_COPY_AND_ASSIGN(PolicyStatisticsCollector);
};

}  

#endif  
