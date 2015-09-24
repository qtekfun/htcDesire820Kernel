// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_PERFORMANCE_SYNC_TIMING_HELPER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_PERFORMANCE_SYNC_TIMING_HELPER_H_

#include "base/basictypes.h"

#include <string>
#include <vector>

namespace base {
class TimeDelta;
}

class ProfileSyncServiceHarness;

class SyncTimingHelper {
 public:
  SyncTimingHelper();
  ~SyncTimingHelper();

  
  static base::TimeDelta TimeSyncCycle(ProfileSyncServiceHarness* client);

  
  
  static base::TimeDelta TimeMutualSyncCycle(
      ProfileSyncServiceHarness* client, ProfileSyncServiceHarness* partner);

  
  
  static base::TimeDelta TimeUntilQuiescence(
      std::vector<ProfileSyncServiceHarness*>& clients);

  
  
  
  static void PrintResult(const std::string& measurement,
                          const std::string& trace,
                          const base::TimeDelta& dt);

 private:
  DISALLOW_COPY_AND_ASSIGN(SyncTimingHelper);
};

#endif  
