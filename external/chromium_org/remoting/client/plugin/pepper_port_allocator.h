// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_PLUGIN_PEPPER_PORT_ALLOCATOR_H_
#define REMOTING_CLIENT_PLUGIN_PEPPER_PORT_ALLOCATOR_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ppapi/cpp/instance_handle.h"
#include "third_party/libjingle/source/talk/p2p/client/httpportallocator.h"

namespace remoting {

class PepperPortAllocator : public cricket::HttpPortAllocatorBase {
 public:
  static scoped_ptr<PepperPortAllocator> Create(
      const pp::InstanceHandle& instance);
  virtual ~PepperPortAllocator();

  
  virtual cricket::PortAllocatorSession* CreateSessionInternal(
      const std::string& content_name,
      int component,
      const std::string& ice_username_fragment,
      const std::string& ice_password) OVERRIDE;

 private:
  PepperPortAllocator(
      const pp::InstanceHandle& instance,
      scoped_ptr<talk_base::NetworkManager> network_manager,
      scoped_ptr<talk_base::PacketSocketFactory> socket_factory);

  pp::InstanceHandle instance_;
  scoped_ptr<talk_base::NetworkManager> network_manager_;
  scoped_ptr<talk_base::PacketSocketFactory> socket_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperPortAllocator);
};

}  

#endif  
