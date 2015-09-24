// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_CONSUMER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_CONSUMER_H_

#include "chrome/browser/common/cancelable_request.h"
#include "chrome/common/cancelable_task_tracker.h"

namespace autofill {
struct PasswordForm;
}

class PasswordStoreConsumer {
 public:
  PasswordStoreConsumer();

  
  
  virtual void OnPasswordStoreRequestDone(
      CancelableRequestProvider::Handle handle,
      const std::vector<autofill::PasswordForm*>& result) = 0;

  
  
  
  
  CancelableRequestConsumerBase* cancelable_consumer() {
    return &cancelable_consumer_;
  }

  
  
  
  virtual void OnGetPasswordStoreResults(
      const std::vector<autofill::PasswordForm*>& results) = 0;

  
  CancelableTaskTracker* cancelable_task_tracker() {
    return &cancelable_task_tracker_;
  }

 protected:
  virtual ~PasswordStoreConsumer();

 private:
  CancelableRequestConsumer cancelable_consumer_;
  CancelableTaskTracker cancelable_task_tracker_;
};

#endif  
