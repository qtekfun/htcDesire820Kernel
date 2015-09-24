// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_NET_SYNC_WEBSOCKET_H_
#define CHROME_TEST_CHROMEDRIVER_NET_SYNC_WEBSOCKET_H_

#include <string>

#include "base/time/time.h"

class GURL;

class SyncWebSocket {
 public:
  enum StatusCode {
    kOk = 0,
    kTimeout,
    kDisconnected
  };

  virtual ~SyncWebSocket() {}

  
  virtual bool IsConnected() = 0;

  
  virtual bool Connect(const GURL& url) = 0;

  
  virtual bool Send(const std::string& message) = 0;

  
  
  
  virtual StatusCode ReceiveNextMessage(
      std::string* message,
      const base::TimeDelta& timeout) = 0;

  
  
  virtual bool HasNextMessage() = 0;
};

#endif  
