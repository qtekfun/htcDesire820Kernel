// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_BACKOFF_DELAY_PROVIDER_H_
#define SYNC_ENGINE_BACKOFF_DELAY_PROVIDER_H_

#include "base/time/time.h"
#include "sync/base/sync_export.h"

namespace syncer {

namespace sessions {
struct ModelNeutralState;
}

class SYNC_EXPORT_PRIVATE BackoffDelayProvider {
 public:
  
  static BackoffDelayProvider* FromDefaults();

  
  
  
  
  
  
  static BackoffDelayProvider* WithShortInitialRetryOverride();

  virtual ~BackoffDelayProvider();

  
  
  
  virtual base::TimeDelta GetDelay(const base::TimeDelta& last_delay);

  
  
  virtual base::TimeDelta GetInitialDelay(
      const sessions::ModelNeutralState& state) const;

 protected:
  BackoffDelayProvider(const base::TimeDelta& default_initial_backoff,
                       const base::TimeDelta& short_initial_backoff);

 private:
  const base::TimeDelta default_initial_backoff_;
  const base::TimeDelta short_initial_backoff_;

  DISALLOW_COPY_AND_ASSIGN(BackoffDelayProvider);
};

}  

#endif  
