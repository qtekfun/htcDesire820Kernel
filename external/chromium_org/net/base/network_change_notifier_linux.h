// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_CHANGE_NOTIFIER_LINUX_H_
#define NET_BASE_NETWORK_CHANGE_NOTIFIER_LINUX_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier.h"

namespace net {

class NET_EXPORT_PRIVATE NetworkChangeNotifierLinux
    : public NetworkChangeNotifier {
 public:
  static NetworkChangeNotifierLinux* Create();

 private:
  class Thread;

  NetworkChangeNotifierLinux();
  virtual ~NetworkChangeNotifierLinux();
  static NetworkChangeCalculatorParams NetworkChangeCalculatorParamsLinux();

  
  virtual ConnectionType GetCurrentConnectionType() const OVERRIDE;

  virtual const internal::AddressTrackerLinux*
      GetAddressTrackerInternal() const OVERRIDE;

  
  
  
  
  scoped_ptr<Thread> notifier_thread_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierLinux);
};

}  

#endif  
