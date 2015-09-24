// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_CONFIG_SERVICE_ANDROID_H_
#define NET_PROXY_PROXY_CONFIG_SERVICE_ANDROID_H_

#include <string>

#include "base/android/jni_android.h"
#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_config_service.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {

class ProxyConfig;

class NET_EXPORT ProxyConfigServiceAndroid : public ProxyConfigService {
 public:
  
  
  
  
  typedef base::Callback<std::string (const std::string& property)>
      GetPropertyCallback;

  
  
  class JNIDelegate {
   public:
    virtual ~JNIDelegate() {}

    
    
    
    virtual void ProxySettingsChangedTo(JNIEnv*, jobject, jstring, jint) = 0;

    
    
    virtual void ProxySettingsChanged(JNIEnv*, jobject) = 0;
  };

  ProxyConfigServiceAndroid(base::SequencedTaskRunner* network_task_runner,
                            base::SequencedTaskRunner* jni_task_runner);

  virtual ~ProxyConfigServiceAndroid();

  
  static bool Register(JNIEnv* env);

  
  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual ConfigAvailability GetLatestProxyConfig(ProxyConfig* config) OVERRIDE;

 private:
  friend class ProxyConfigServiceAndroidTestBase;
  class Delegate;

  
  ProxyConfigServiceAndroid(base::SequencedTaskRunner* network_task_runner,
                            base::SequencedTaskRunner* jni_task_runner,
                            GetPropertyCallback get_property_callback);

  
  void ProxySettingsChanged();

  scoped_refptr<Delegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(ProxyConfigServiceAndroid);
};

} 

#endif 
