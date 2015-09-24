// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_HOST_CAPABILITY_H_
#define NET_HTTP_HTTP_PIPELINED_HOST_CAPABILITY_H_

namespace net {

enum HttpPipelinedHostCapability {
  PIPELINE_UNKNOWN = 0,
  PIPELINE_INCAPABLE = 1,
  PIPELINE_CAPABLE = 2,
  PIPELINE_PROBABLY_CAPABLE = 3,  
                                  
                                  
};

}  

#endif  
