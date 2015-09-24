// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_DEVTOOLS_EVENT_LISTENER_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_DEVTOOLS_EVENT_LISTENER_H_

#include <string>

namespace base {
class DictionaryValue;
}

class DevToolsClient;
class Status;

class DevToolsEventListener {
 public:
  virtual ~DevToolsEventListener();

  
  virtual Status OnConnected(DevToolsClient* client);

  
  virtual Status OnEvent(DevToolsClient* client,
                         const std::string& method,
                         const base::DictionaryValue& params);

  
  virtual Status OnCommandSuccess(DevToolsClient* client,
                                  const std::string& method);
};

#endif  
