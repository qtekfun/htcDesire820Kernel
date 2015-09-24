// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_LOAD_TIMING_INFO_H_
#define NET_BASE_LOAD_TIMING_INFO_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

struct NET_EXPORT LoadTimingInfo {
  
  
  struct NET_EXPORT_PRIVATE ConnectTiming {
    ConnectTiming();
    ~ConnectTiming();

    
    
    
    
    
    
    base::TimeTicks dns_start;
    base::TimeTicks dns_end;

    
    
    
    
    
    
    
    
    
    
    
    base::TimeTicks connect_start;
    base::TimeTicks connect_end;

    
    
    
    base::TimeTicks ssl_start;
    base::TimeTicks ssl_end;
  };

  LoadTimingInfo();
  ~LoadTimingInfo();

  
  
  
  
  
  
  
  
  
  bool socket_reused;

  
  
  
  
  
  
  
  
  uint32 socket_log_id;

  
  
  
  base::Time request_start_time;

  base::TimeTicks request_start;

  
  base::TimeTicks proxy_resolve_start;
  base::TimeTicks proxy_resolve_end;

  ConnectTiming connect_timing;

  
  base::TimeTicks send_start;
  base::TimeTicks send_end;

  
  base::TimeTicks receive_headers_end;
};

}  

#endif  
