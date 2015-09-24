// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_LOAD_TIMING_INFO_TEST_UTIL_H_
#define NET_BASE_LOAD_TIMING_INFO_TEST_UTIL_H_

#include "net/base/load_timing_info.h"

namespace net {

enum ConnectTimeFlags {
  CONNECT_TIMING_HAS_CONNECT_TIMES_ONLY = 0,
  CONNECT_TIMING_HAS_DNS_TIMES          = 1 << 0,
  CONNECT_TIMING_HAS_SSL_TIMES          = 1 << 1,
};

void ExpectConnectTimingHasNoTimes(
    const LoadTimingInfo::ConnectTiming& connect_timing);

void ExpectConnectTimingHasTimes(
    const LoadTimingInfo::ConnectTiming& connect_timing,
    int connect_timing_flags);

void ExpectLoadTimingHasOnlyConnectionTimes(
    const LoadTimingInfo& load_timing_info);

}  

#endif  
