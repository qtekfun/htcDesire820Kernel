// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_RESOURCE_RESPONSE_INFO_H_
#define WEBKIT_COMMON_RESOURCE_RESPONSE_INFO_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "net/base/host_port_pair.h"
#include "net/base/load_timing_info.h"
#include "net/http/http_response_info.h"
#include "url/gurl.h"
#include "webkit/common/resource_devtools_info.h"

namespace webkit_glue {

struct ResourceResponseInfo {
  WEBKIT_COMMON_EXPORT ResourceResponseInfo();
  WEBKIT_COMMON_EXPORT ~ResourceResponseInfo();

  
  
  base::Time request_time;

  
  
  base::Time response_time;

  
  scoped_refptr<net::HttpResponseHeaders> headers;

  
  std::string mime_type;

  
  
  std::string charset;

  
  
  std::string security_info;

  
  int64 content_length;

  
  
  int64 encoded_data_length;

  
  int64 appcache_id;

  
  
  GURL appcache_manifest_url;

  
  
  net::LoadTimingInfo load_timing;

  
  
  
  scoped_refptr<ResourceDevToolsInfo> devtools_info;

  
  
  
  base::FilePath download_file_path;

  
  bool was_fetched_via_spdy;

  
  bool was_npn_negotiated;

  
  
  bool was_alternate_protocol_available;

  
  net::HttpResponseInfo::ConnectionInfo connection_info;

  
  
  
  bool was_fetched_via_proxy;

  
  std::string npn_negotiated_protocol;

  
  net::HostPortPair socket_address;
};

}  

#endif  
