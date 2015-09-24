// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_SCT_STATUS_FLAGS_H_
#define NET_CERT_SCT_STATUS_FLAGS_H_

namespace net {

namespace ct {

enum SCTVerifyStatus {
  
  
  SCT_STATUS_NONE = 0,

  
  SCT_STATUS_LOG_UNKNOWN = 1,

  
  SCT_STATUS_INVALID = 2,

  
  SCT_STATUS_OK = 3,
};

}  

}  

#endif  
