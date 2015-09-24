// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_INTERFACE_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_INTERFACE_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time.h"

namespace net {

class URLRequestThrottlerHeaderInterface;

class URLRequestThrottlerEntryInterface
    : public base::RefCountedThreadSafe<URLRequestThrottlerEntryInterface> {
 public:
  URLRequestThrottlerEntryInterface() {}

  
  
  
  
  virtual bool IsDuringExponentialBackoff() const = 0;

  
  
  
  
  
  
  
  
  
  virtual int64 ReserveSendingTimeForNextRequest(
      const base::TimeTicks& earliest_time) = 0;

  
  virtual base::TimeTicks GetExponentialBackoffReleaseTime() const = 0;

  
  virtual void UpdateWithResponse(
      const std::string& host,
      const URLRequestThrottlerHeaderInterface* response) = 0;

  
  
  
  
  virtual void ReceivedContentWasMalformed() = 0;

 protected:
  friend class base::RefCountedThreadSafe<URLRequestThrottlerEntryInterface>;
  virtual ~URLRequestThrottlerEntryInterface() {}

 private:
  friend class base::RefCounted<URLRequestThrottlerEntryInterface>;
  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerEntryInterface);
};

}  

#endif  
