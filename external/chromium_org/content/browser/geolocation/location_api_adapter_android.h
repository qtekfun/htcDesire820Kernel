// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_LOCATION_API_ADAPTER_ANDROID_H_
#define CONTENT_BROWSER_GEOLOCATION_LOCATION_API_ADAPTER_ANDROID_H_

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"

namespace content {
class LocationProviderAndroid;
struct Geoposition;

class AndroidLocationApiAdapter {
 public:
  
  
  bool Start(LocationProviderAndroid* location_provider, bool high_accuracy);
  
  
  void Stop();

  
  static AndroidLocationApiAdapter* GetInstance();

  
  static bool RegisterGeolocationService(JNIEnv* env);

  
  static void OnNewLocationAvailable(double latitude,
                                     double longitude,
                                     double time_stamp,
                                     bool has_altitude, double altitude,
                                     bool has_accuracy, double accuracy,
                                     bool has_heading, double heading,
                                     bool has_speed, double speed);
  static void OnNewErrorAvailable(JNIEnv* env, jstring message);

 private:
  friend struct DefaultSingletonTraits<AndroidLocationApiAdapter>;
  AndroidLocationApiAdapter();
  ~AndroidLocationApiAdapter();

  void CreateJavaObject(JNIEnv* env);

  
  void OnNewGeopositionInternal(const Geoposition& geoposition);

  
  static void NotifyProviderNewGeoposition(const Geoposition& geoposition);

  base::android::ScopedJavaGlobalRef<jobject>
      java_location_provider_android_object_;
  LocationProviderAndroid* location_provider_;

  
  
  base::Lock lock_;
  scoped_refptr<base::MessageLoopProxy> message_loop_;
};

}  

#endif  
