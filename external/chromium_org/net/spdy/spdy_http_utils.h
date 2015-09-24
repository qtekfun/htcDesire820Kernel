// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_HTTP_UTILS_H_
#define NET_SPDY_SPDY_HTTP_UTILS_H_

#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"
#include "url/gurl.h"

namespace net {

class HttpResponseInfo;
struct HttpRequestInfo;
class HttpRequestHeaders;

bool SpdyHeadersToHttpResponse(const SpdyHeaderBlock& headers,
                               SpdyMajorVersion protocol_version,
                               HttpResponseInfo* response);

void NET_EXPORT_PRIVATE CreateSpdyHeadersFromHttpRequest(
    const HttpRequestInfo& info,
    const HttpRequestHeaders& request_headers,
    SpdyHeaderBlock* headers,
    SpdyMajorVersion protocol_version,
    bool direct);

GURL GetUrlFromHeaderBlock(const SpdyHeaderBlock& headers,
                           SpdyMajorVersion protocol_version,
                           bool pushed);

NET_EXPORT_PRIVATE bool ShouldShowHttpHeaderValue(
    const std::string& header_name);

NET_EXPORT_PRIVATE SpdyPriority ConvertRequestPriorityToSpdyPriority(
    RequestPriority priority,
    SpdyMajorVersion protocol_version);

NET_EXPORT_PRIVATE RequestPriority ConvertSpdyPriorityToRequestPriority(
    SpdyPriority priority,
    SpdyMajorVersion protocol_version);

}  

#endif  