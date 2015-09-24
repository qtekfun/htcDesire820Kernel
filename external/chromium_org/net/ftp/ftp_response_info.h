// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_RESPONSE_INFO_H_
#define NET_FTP_FTP_RESPONSE_INFO_H_

#include "base/time/time.h"
#include "net/base/host_port_pair.h"

namespace net {

class FtpResponseInfo {
 public:
  FtpResponseInfo();
  ~FtpResponseInfo();

  
  
  bool needs_auth;

  
  
  base::Time request_time;

  
  
  base::Time response_time;

  
  
  int64 expected_content_size;

  
  bool is_directory_listing;

  
  HostPortPair socket_address;
};

}  

#endif  
