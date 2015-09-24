// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_DEVTOOLS_CLIENT_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_DEVTOOLS_CLIENT_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class TimeDelta;
}

class DevToolsEventListener;
class Status;

class DevToolsClient {
 public:
  typedef base::Callback<Status(bool* is_condition_met)> ConditionalFunc;

  virtual ~DevToolsClient() {}

  virtual const std::string& GetId() = 0;

  virtual bool WasCrashed() = 0;

  
  virtual Status ConnectIfNecessary() = 0;

  virtual Status SendCommand(const std::string& method,
                             const base::DictionaryValue& params) = 0;
  virtual Status SendCommandAndGetResult(
      const std::string& method,
      const base::DictionaryValue& params,
      scoped_ptr<base::DictionaryValue>* result) = 0;

  
  virtual void AddListener(DevToolsEventListener* listener) = 0;

  
  
  
  
  
  virtual Status HandleEventsUntil(const ConditionalFunc& conditional_func,
                                   const base::TimeDelta& timeout) = 0;

  
  virtual Status HandleReceivedEvents() = 0;
};

#endif  
