// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_CONSUMER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_CONSUMER_H_
#pragma once

#include "content/browser/cancelable_request.h"

namespace webkit_glue {
struct PasswordForm;
};

class PasswordStoreConsumer {
 public:
  PasswordStoreConsumer();

  
  
  virtual void OnPasswordStoreRequestDone(
      CancelableRequestProvider::Handle handle,
      const std::vector<webkit_glue::PasswordForm*>& result) = 0;

  
  
  
  
  CancelableRequestConsumerBase* cancelable_consumer() {
    return &cancelable_consumer_;
  }

 protected:
  virtual ~PasswordStoreConsumer();

 private:
  CancelableRequestConsumer cancelable_consumer_;
};

#endif  
