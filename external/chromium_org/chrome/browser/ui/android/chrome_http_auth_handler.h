// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ANDROID_CHROME_HTTP_AUTH_HANDLER_H_
#define CHROME_BROWSER_UI_ANDROID_CHROME_HTTP_AUTH_HANDLER_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/login/login_prompt.h"

class ChromeHttpAuthHandler {
 public:
  ChromeHttpAuthHandler(const base::string16& explanation);
  ~ChromeHttpAuthHandler();

  
  
  void Init();

  
  
  void SetObserver(LoginHandler* observer);

  
  
  
  jobject GetJavaObject();

  
  void OnAutofillDataAvailable(
      const base::string16& username,
      const base::string16& password);

  
  
  

  
  void SetAuth(JNIEnv* env, jobject, jstring username, jstring password);

  
  void CancelAuth(JNIEnv* env, jobject);

  
  base::android::ScopedJavaLocalRef<jstring> GetMessageTitle(
      JNIEnv* env, jobject);
  base::android::ScopedJavaLocalRef<jstring> GetMessageBody(
      JNIEnv* env, jobject);
  base::android::ScopedJavaLocalRef<jstring> GetUsernameLabelText(
      JNIEnv* env, jobject);
  base::android::ScopedJavaLocalRef<jstring> GetPasswordLabelText(
      JNIEnv* env, jobject);
  base::android::ScopedJavaLocalRef<jstring> GetOkButtonText(
      JNIEnv* env, jobject);
  base::android::ScopedJavaLocalRef<jstring> GetCancelButtonText(
      JNIEnv* env, jobject);
  
  static bool RegisterChromeHttpAuthHandler(JNIEnv* env);
 private:
  LoginHandler* observer_;
  base::android::ScopedJavaGlobalRef<jobject> java_chrome_http_auth_handler_;
  
  base::string16 explanation_;

  DISALLOW_COPY_AND_ASSIGN(ChromeHttpAuthHandler);
};

#endif  
