// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_CALLBACK_WORK_ITEM_H_
#define CHROME_INSTALLER_UTIL_CALLBACK_WORK_ITEM_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "chrome/installer/util/work_item.h"

class CallbackWorkItem : public WorkItem {
 public:
  virtual ~CallbackWorkItem();

  virtual bool Do() OVERRIDE;
  virtual void Rollback() OVERRIDE;

  bool IsRollback() const;

 private:
  friend class WorkItem;

  enum RollState {
    RS_UNDEFINED,
    RS_FORWARD,
    RS_BACKWARD,
  };

  CallbackWorkItem(base::Callback<bool(const CallbackWorkItem&)> callback);

  base::Callback<bool(const CallbackWorkItem&)> callback_;
  RollState roll_state_;

  FRIEND_TEST_ALL_PREFIXES(CallbackWorkItemTest, TestFailure);
  FRIEND_TEST_ALL_PREFIXES(CallbackWorkItemTest, TestForwardBackward);
};

#endif  
