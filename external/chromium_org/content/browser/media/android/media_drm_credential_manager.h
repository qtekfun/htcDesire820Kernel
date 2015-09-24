// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_ANDROID_MEDIA_DRM_CREDENTIAL_MANAGER_H_
#define CONTENT_BROWSER_MEDIA_ANDROID_MEDIA_DRM_CREDENTIAL_MANAGER_H_

#include <jni.h>
#include <string>

#include "base/callback.h"
#include "base/memory/singleton.h"
#include "media/base/android/media_drm_bridge.h"

namespace content {

class MediaDrmCredentialManager {
 public:
  static MediaDrmCredentialManager* GetInstance();

  typedef base::Callback<void(bool)> ResetCredentialsCB;

  
  static void ResetCredentials(JNIEnv* env, jclass clazz, jobject callback);

  
  void ResetCredentials(const ResetCredentialsCB& callback);

  static bool RegisterMediaDrmCredentialManager(JNIEnv* env);

 private:
  friend struct DefaultSingletonTraits<MediaDrmCredentialManager>;
  friend class Singleton<MediaDrmCredentialManager>;

  MediaDrmCredentialManager();
  ~MediaDrmCredentialManager();


  
  
  void OnResetCredentialsCompleted(const std::string& security_level,
                                   bool success);

  
  
  bool ResetCredentialsInternal(const std::string& security_level);

  
  scoped_ptr<media::MediaDrmBridge> media_drm_bridge_;

  
  ResetCredentialsCB reset_credentials_cb_;

  DISALLOW_COPY_AND_ASSIGN(MediaDrmCredentialManager);
};

}  

#endif  
