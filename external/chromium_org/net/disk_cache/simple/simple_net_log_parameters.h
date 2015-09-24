// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_NET_LOG_PARAMETERS_H_
#define NET_DISK_CACHE_SIMPLE_NET_LOG_PARAMETERS_H_

#include "net/base/net_log.h"

namespace disk_cache {

class SimpleEntryImpl;

net::NetLog::ParametersCallback CreateNetLogSimpleEntryConstructionCallback(
    const SimpleEntryImpl* entry);

net::NetLog::ParametersCallback CreateNetLogSimpleEntryCreationCallback(
    const SimpleEntryImpl* entry,
    int net_error);

}  

#endif  
