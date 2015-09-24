// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_H_
#define CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_H_

#include "base/time/time.h"

class NetworkTime {
 public:
  NetworkTime() {}
  explicit NetworkTime(base::Time t) : time_internal_(t) {}
  ~NetworkTime() {}

  base::Time GetTime() const {
    return time_internal_;
  }

 private:
  
  base::Time time_internal_;
};

#endif  
