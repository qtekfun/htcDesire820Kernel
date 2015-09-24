// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_NET_LOG_PARAMETERS_H_
#define NET_DISK_CACHE_NET_LOG_PARAMETERS_H_

#include <string>

#include "net/base/net_log.h"

namespace disk_cache {

class Entry;

net::NetLog::ParametersCallback CreateNetLogEntryCreationCallback(
    const Entry* entry,
    bool created);

net::NetLog::ParametersCallback CreateNetLogReadWriteDataCallback(
    int index,
    int offset,
    int buf_len,
    bool truncate);

net::NetLog::ParametersCallback CreateNetLogReadWriteCompleteCallback(
    int bytes_copied);

net::NetLog::ParametersCallback CreateNetLogSparseOperationCallback(
    int64 offset,
    int buff_len);

net::NetLog::ParametersCallback CreateNetLogSparseReadWriteCallback(
    const net::NetLog::Source& source,
    int child_len);

net::NetLog::ParametersCallback CreateNetLogGetAvailableRangeResultCallback(
    int64 start,
    int result);

}  

#endif  
