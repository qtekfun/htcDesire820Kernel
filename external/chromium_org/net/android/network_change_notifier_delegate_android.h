// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_NETWORK_CHANGE_NOTIFIER_DELEGATE_ANDROID_H_
#define NET_ANDROID_NETWORK_CHANGE_NOTIFIER_DELEGATE_ANDROID_H_

#include "base/android/jni_android.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker.h"
#include "net/base/network_change_notifier.h"

namespace net {

class NET_EXPORT_PRIVATE NetworkChangeNotifierDelegateAndroid {
 public:
  typedef NetworkChangeNotifier::ConnectionType ConnectionType;

  
  
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    virtual void OnConnectionTypeChanged() = 0;
  };

  NetworkChangeNotifierDelegateAndroid();
  ~NetworkChangeNotifierDelegateAndroid();

  
  
  
  
  void NotifyConnectionTypeChanged(JNIEnv* env,
                                   jobject obj,
                                   jint new_connection_type);
  jint GetConnectionType(JNIEnv* env, jobject obj) const;

  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  ConnectionType GetCurrentConnectionType() const;

  
  static bool Register(JNIEnv* env);

 private:
  friend class BaseNetworkChangeNotifierAndroidTest;

  void SetCurrentConnectionType(ConnectionType connection_type);

  
  void SetOnline();
  void SetOffline();

  base::ThreadChecker thread_checker_;
  scoped_refptr<ObserverListThreadSafe<Observer> > observers_;
  scoped_refptr<base::SingleThreadTaskRunner> jni_task_runner_;
  base::android::ScopedJavaGlobalRef<jobject> java_network_change_notifier_;
  mutable base::Lock connection_type_lock_;  
  ConnectionType connection_type_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifierDelegateAndroid);
};

}  

#endif  
