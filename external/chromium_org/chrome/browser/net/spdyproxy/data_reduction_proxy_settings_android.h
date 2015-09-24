// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_SPDYPROXY_DATA_REDUCTION_PROXY_SETTINGS_ANDROID_H_
#define CHROME_BROWSER_NET_SPDYPROXY_DATA_REDUCTION_PROXY_SETTINGS_ANDROID_H_

#include "base/android/jni_helper.h"
#include "base/android/jni_string.h"
#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_member.h"
#include "chrome/browser/net/spdyproxy/data_reduction_proxy_settings.h"


using base::android::ScopedJavaLocalRef;


class DataReductionProxySettingsAndroid : public DataReductionProxySettings {
 public:
  DataReductionProxySettingsAndroid(JNIEnv* env, jobject obj);
  
  DataReductionProxySettingsAndroid();

  virtual ~DataReductionProxySettingsAndroid();

  void InitDataReductionProxySettings(JNIEnv* env, jobject obj);

  void BypassHostPattern(JNIEnv* env, jobject obj, jstring pattern);
  
  
  
  
  void BypassURLPattern(JNIEnv* env, jobject obj, jstring pattern);

  virtual void AddURLPatternToBypass(const std::string& pattern) OVERRIDE;

  
  jboolean IsDataReductionProxyAllowed(JNIEnv* env, jobject obj);
  jboolean IsDataReductionProxyPromoAllowed(JNIEnv* env, jobject obj);
  ScopedJavaLocalRef<jstring> GetDataReductionProxyOrigin(JNIEnv* env,
                                                          jobject obj);
  jboolean IsDataReductionProxyEnabled(JNIEnv* env, jobject obj);
  jboolean IsDataReductionProxyManaged(JNIEnv* env, jobject obj);
  void SetDataReductionProxyEnabled(JNIEnv* env, jobject obj, jboolean enabled);

  jlong GetDataReductionLastUpdateTime(JNIEnv* env, jobject obj);
  ScopedJavaLocalRef<jlongArray> GetDailyOriginalContentLengths(JNIEnv* env,
                                                                jobject obj);
  ScopedJavaLocalRef<jlongArray> GetDailyReceivedContentLengths(JNIEnv* env,
                                                                jobject obj);

  
  
  base::android::ScopedJavaLocalRef<jobject> GetContentLengths(JNIEnv* env,
                                                               jobject obj);

  
  
  
  jboolean IsAcceptableAuthChallenge(JNIEnv* env,
                                     jobject obj,
                                     jstring host,
                                     jstring realm);

  ScopedJavaLocalRef<jstring> GetTokenForAuthChallenge(JNIEnv* env,
                                                       jobject obj,
                                                       jstring host,
                                                       jstring realm);

  
  static bool Register(JNIEnv* env);

 protected:
  
  virtual void AddDefaultProxyBypassRules() OVERRIDE;

  
  
  virtual void SetProxyConfigs(
      bool enabled, bool restricted, bool at_startup) OVERRIDE;

 private:
  friend class DataReductionProxySettingsAndroidTest;
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsAndroidTest,
                           TestBypassPACRules);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsAndroidTest,
                           TestSetProxyPac);
  FRIEND_TEST_ALL_PREFIXES(DataReductionProxySettingsAndroidTest,
                           TestGetDailyContentLengths);


  ScopedJavaLocalRef<jlongArray> GetDailyContentLengths(JNIEnv* env,
                                                        const char* pref_name);
  std::string GetProxyPacScript(bool restricted);

  std::vector<std::string> pac_bypass_rules_;

  DISALLOW_COPY_AND_ASSIGN(DataReductionProxySettingsAndroid);
};

#endif  
