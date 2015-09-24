// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_STATUS_CHANGE_CHECKER_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_STATUS_CHANGE_CHECKER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sync/test/integration/profile_sync_service_harness.h"

class StatusChangeChecker {
 public:
  typedef base::Callback<bool()> StatusChangeCallback;

  StatusChangeChecker(StatusChangeCallback callback,
                      const std::string& callback_name);
  ~StatusChangeChecker();
  bool IsExitConditionSatisfied();
  std::string callback_name() const { return callback_name_; }

 private:
  
  StatusChangeCallback callback_;

  
  std::string callback_name_;

  DISALLOW_COPY_AND_ASSIGN(StatusChangeChecker);
};

#endif  
