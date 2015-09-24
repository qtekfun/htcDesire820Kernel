// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_FAKE_NETWORK_MANAGER_H_
#define JINGLE_GLUE_FAKE_NETWORK_MANAGER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/net_util.h"
#include "third_party/libjingle/source/talk/base/network.h"

namespace jingle_glue {

class FakeNetworkManager : public talk_base::NetworkManager {
 public:
  FakeNetworkManager(const net::IPAddressNumber& address);
  virtual ~FakeNetworkManager();

  
  virtual void StartUpdating() OVERRIDE;
  virtual void StopUpdating() OVERRIDE;
  virtual void GetNetworks(NetworkList* networks) const OVERRIDE;

 protected:
  void SendNetworksChangedSignal();

  bool started_;
  scoped_ptr<talk_base::Network> network_;

  base::WeakPtrFactory<FakeNetworkManager> weak_factory_;
};

}  

#endif  
