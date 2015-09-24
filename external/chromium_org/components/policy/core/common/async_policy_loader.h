// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_ASYNC_POLICY_LOADER_H_
#define COMPONENTS_POLICY_CORE_COMMON_ASYNC_POLICY_LOADER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "components/policy/core/common/schema_map.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class PolicyBundle;

class POLICY_EXPORT AsyncPolicyLoader {
 public:
  explicit AsyncPolicyLoader(
      scoped_refptr<base::SequencedTaskRunner> task_runner);
  virtual ~AsyncPolicyLoader();

  
  base::SequencedTaskRunner* task_runner() const { return task_runner_.get(); }

  
  
  
  virtual scoped_ptr<PolicyBundle> Load() = 0;

  
  
  virtual void InitOnBackgroundThread() = 0;

  
  
  virtual base::Time LastModificationTime();

  
  
  
  scoped_ptr<PolicyBundle> InitialLoad(const scoped_refptr<SchemaMap>& schemas);

  
  
  
  
  
  
  // currently being written to, and whose contents are incomplete.
  
  
  void Reload(bool force);

  const scoped_refptr<SchemaMap>& schema_map() const { return schema_map_; }

 private:
  
  friend class AsyncPolicyProvider;

  typedef base::Callback<void(scoped_ptr<PolicyBundle>)> UpdateCallback;

  
  
  void Init(const UpdateCallback& update_callback);

  
  void RefreshPolicies(scoped_refptr<SchemaMap> schema_map);

  
  
  void ScheduleNextReload(base::TimeDelta delay);

  
  
  
  bool IsSafeToReload(const base::Time& now, base::TimeDelta* delay);

  
  scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  UpdateCallback update_callback_;

  
  base::WeakPtrFactory<AsyncPolicyLoader> weak_factory_;

  
  base::Time last_modification_time_;

  
  
  
  
  base::Time last_modification_clock_;

  
  scoped_refptr<SchemaMap> schema_map_;

  DISALLOW_COPY_AND_ASSIGN(AsyncPolicyLoader);
};

}  

#endif  
