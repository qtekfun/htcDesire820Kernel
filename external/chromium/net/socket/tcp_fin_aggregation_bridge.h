// Copyright (c) 2011, Code Aurora Forum. All rights reserved.

//     * Redistributions of source code must retain the above copyright
//       copyright notice, this list of conditions and the following
//       from this software without specific prior written permission.

// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS

#ifndef TCP_FIN_AGGREGATION_BRIDGE_H_
#define TCP_FIN_AGGREGATION_BRIDGE_H_

#include "client_socket_pool_base.h"
#include "time.h"

namespace net {
namespace internal {
  class ClientSocketPoolBaseHelper;
  class IdleSocket;
}
};

extern void DecrementIdleCount(net::internal::ClientSocketPoolBaseHelper* pool_base_helper) __attribute__((visibility("default"), used));
extern void RemoveGroup(net::internal::ClientSocketPoolBaseHelper* pool_base_helper, const std::string& group_name) __attribute__((visibility("default"), used));
extern bool ShouldCleanup(net::internal::IdleSocket* idle_socket, base::Time now, base::TimeDelta timeout) __attribute__((visibility("default"), used));
extern base::Time GetCurrentTime() __attribute__((visibility("default"), used));

#endif 
