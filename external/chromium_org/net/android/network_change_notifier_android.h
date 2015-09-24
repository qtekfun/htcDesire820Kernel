// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_NETWORK_CHANGE_NOTIFIER_ANDROID_H_
#define NET_ANDROID_NETWORK_CHANGE_NOTIFIER_ANDROID_H_

#include "base/android/jni_android.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/android/network_change_notifier_delegate_android.h"
#include "net/base/network_change_notifier.h"

namespace net {

class NetworkChangeNotifierAndroidTest;
class NetworkChangeNotifierFactoryAndroid;

class NET_EXPORT_PRIVATE NetworkChangeNotifierAndroid
    : public NetworkChangeNotifier,
      public NetworkChangeNotifierDelegateAndroid::Observer {
 public:
  virtual ~NetworkChangeNotifierAndroid();

  
  virtual ConnectionType GetCurrentConnectionType() const OVERRIDE;

  
  virtual void OnConnectionTypeChanged() OVERRIDE;

  static bool Register(JNIEnv* env);

 private:
  friend class NetworkChangeNotifierAndroidTest;
  friend class NetworkChangeNotifierFactoryAndroid;

  explicit NetworkChangeNotifierAndroid(
      NetworkChangeNotifierDelegateAndroid* delegate);

  static NetworkChangeCalculatorParams NetworkChangeCalculatorParamsAndroid();

  NetworkChangeNotifierDelegateAndroid* const delegate_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierAndroid);
};

}  

#endif  
