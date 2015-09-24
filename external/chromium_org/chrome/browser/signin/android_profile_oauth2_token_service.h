// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_ANDROID_PROFILE_OAUTH2_TOKEN_SERVICE_H_
#define CHROME_BROWSER_SIGNIN_ANDROID_PROFILE_OAUTH2_TOKEN_SERVICE_H_

#include <jni.h>
#include <string>

#include "base/android/jni_helper.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/signin/profile_oauth2_token_service.h"
#include "google_apis/gaia/google_service_auth_error.h"

class AndroidProfileOAuth2TokenService : public ProfileOAuth2TokenService {
 public:
  
  
  static bool Register(JNIEnv* env);

  
  static AndroidProfileOAuth2TokenService* Create();

  
  static jobject GetForProfile(
      JNIEnv* env, jclass clazz, jobject j_profile_android);

  virtual bool RefreshTokenIsAvailable(
      const std::string& account_id) OVERRIDE;

  
  virtual std::vector<std::string> GetAccounts() OVERRIDE;

  void ValidateAccounts(JNIEnv* env,
                        jobject obj,
                        jobjectArray accounts,
                        jstring current_account);

  
  
  void ValidateAccounts(const std::string& signed_in_account,
                        const std::vector<std::string>& account_ids);

  
  
  
  virtual void FireRefreshTokenAvailableFromJava(JNIEnv* env,
                                                 jobject obj,
                                                 const jstring account_name);
  
  
  virtual void FireRefreshTokenRevokedFromJava(JNIEnv* env,
                                               jobject obj,
                                               const jstring account_name);
  
  
  virtual void FireRefreshTokensLoadedFromJava(JNIEnv* env, jobject obj);

 protected:
  friend class ProfileOAuth2TokenServiceFactory;
  AndroidProfileOAuth2TokenService();
  virtual ~AndroidProfileOAuth2TokenService();

  
  
  virtual void FetchOAuth2Token(RequestImpl* request,
                                const std::string& account_id,
                                net::URLRequestContextGetter* getter,
                                const std::string& client_id,
                                const std::string& client_secret,
                                const ScopeSet& scopes) OVERRIDE;

  
  
  virtual void InvalidateOAuth2Token(const std::string& account_id,
                                     const std::string& client_id,
                                     const ScopeSet& scopes,
                                     const std::string& access_token) OVERRIDE;

  
  virtual void FireRefreshTokenAvailable(
      const std::string& account_id) OVERRIDE;
  
  virtual void FireRefreshTokenRevoked(const std::string& account_id) OVERRIDE;
  
  virtual void FireRefreshTokensLoaded() OVERRIDE;

 private:
  base::android::ScopedJavaGlobalRef<jobject> java_ref_;

  DISALLOW_COPY_AND_ASSIGN(AndroidProfileOAuth2TokenService);
};

#endif  
