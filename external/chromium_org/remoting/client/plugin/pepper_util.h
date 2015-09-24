// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_PLUGIN_PLUGIN_UTIL_H_
#define REMOTING_CLIENT_PLUGIN_PLUGIN_UTIL_H_

#include <stdint.h>

#include "base/callback_forward.h"

namespace pp {
class InstanceHandle;
class NetAddress;
}

namespace talk_base {
class SocketAddress;
}

namespace remoting {

bool SocketAddressToPpNetAddressWithPort(
    const pp::InstanceHandle& instance,
    const talk_base::SocketAddress& address,
    pp::NetAddress* pp_net_address,
    uint16_t port);
bool SocketAddressToPpNetAddress(const pp::InstanceHandle& instance,
                                 const talk_base::SocketAddress& address,
                                 pp::NetAddress* pp_net_address);
void PpNetAddressToSocketAddress(const pp::NetAddress& pp_net_address,
                                 talk_base::SocketAddress* address);

}  

#endif  
