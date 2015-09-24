// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_FIELD_TRIAL_SYNCHRONIZER_H_
#define CHROME_BROWSER_METRICS_FIELD_TRIAL_SYNCHRONIZER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/metrics/field_trial.h"


class FieldTrialSynchronizer
    : public base::RefCountedThreadSafe<FieldTrialSynchronizer>,
      public base::FieldTrialList::Observer {
 public:
  
  
  
  
  
  
  FieldTrialSynchronizer();

  
  
  void NotifyAllRenderers(const std::string& field_trial_name,
                          const std::string& group_name);

  

  
  
  
  
  virtual void OnFieldTrialGroupFinalized(
      const std::string& name,
      const std::string& group_name) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<FieldTrialSynchronizer>;
  virtual ~FieldTrialSynchronizer();

  DISALLOW_COPY_AND_ASSIGN(FieldTrialSynchronizer);
};

#endif  
