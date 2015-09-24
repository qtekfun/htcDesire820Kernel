// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_HEADER_BLOCK_H_
#define NET_SPDY_SPDY_HEADER_BLOCK_H_

#include <map>
#include <string>

#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace net {

typedef std::map<std::string, std::string> SpdyHeaderBlock;

NET_EXPORT base::Value* SpdyHeaderBlockNetLogCallback(
    const SpdyHeaderBlock* headers,
    NetLog::LogLevel log_level);

NET_EXPORT bool SpdyHeaderBlockFromNetLogParam(
    const base::Value* event_param,
    SpdyHeaderBlock* headers);

}  

#endif  
