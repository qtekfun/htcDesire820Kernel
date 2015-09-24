// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_INTERFACE_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_INTERFACE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

class URLRequest;
class URLRequestThrottlerHeaderInterface;

class NET_EXPORT URLRequestThrottlerEntryInterface
    : public base::RefCountedThreadSafe<URLRequestThrottlerEntryInterface> {
 public:
  URLRequestThrottlerEntryInterface() {}

  
  
  
  
  
  
  
  virtual bool ShouldRejectRequest(const URLRequest& request) const = 0;

  
  
  
  
  
  
  
  
  
  virtual int64 ReserveSendingTimeForNextRequest(
      const base::TimeTicks& earliest_time) = 0;

  
  virtual base::TimeTicks GetExponentialBackoffReleaseTime() const = 0;

  
  virtual void UpdateWithResponse(
      const std::string& host,
      const URLRequestThrottlerHeaderInterface* response) = 0;

  
  
  
  
  
  virtual void ReceivedContentWasMalformed(int response_code) = 0;

 protected:
  friend class base::RefCountedThreadSafe<URLRequestThrottlerEntryInterface>;
  virtual ~URLRequestThrottlerEntryInterface() {}

 private:
  friend class base::RefCounted<URLRequestThrottlerEntryInterface>;
  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerEntryInterface);
};

}  

#endif  
