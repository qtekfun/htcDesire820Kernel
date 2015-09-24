// Copyright (c) 2011,2012 Code Aurora Forum. All rights reserved.

//     * Redistributions of source code must retain the above copyright
//       copyright notice, this list of conditions and the following
//       from this software without specific prior written permission.

// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS

#ifndef TCP_FIN_AGGREGATION_H_
#define TCP_FIN_AGGREGATION_H_

#include "base/basictypes.h"
#include "client_socket_pool_base.h"

namespace net {

class ITCPFinAggregation {

public:
  ITCPFinAggregation() {};
  virtual ~ITCPFinAggregation() {};

  virtual bool IsEnabled() = 0;
  virtual void ReaperCleanup(bool close_unused_socket) = 0;
  virtual int GetCleanupInterval(int current_interval) = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(ITCPFinAggregation);
};

} 

#endif 
