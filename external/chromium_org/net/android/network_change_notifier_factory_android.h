// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_NETWORK_CHANGE_NOTIFIER_FACTORY_ANDROID_H_
#define NET_ANDROID_NETWORK_CHANGE_NOTIFIER_FACTORY_ANDROID_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/android/network_change_notifier_delegate_android.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier_factory.h"

namespace net {

class NetworkChangeNotifier;
class NetworkChangeNotifierDelegateAndroid;

class NET_EXPORT NetworkChangeNotifierFactoryAndroid :
    public NetworkChangeNotifierFactory {
 public:
  
  NetworkChangeNotifierFactoryAndroid();

  
  virtual ~NetworkChangeNotifierFactoryAndroid();

  
  virtual NetworkChangeNotifier* CreateInstance() OVERRIDE;

 private:
  
  NetworkChangeNotifierDelegateAndroid delegate_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierFactoryAndroid);
};

}  

#endif  
