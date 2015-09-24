// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_DRM_BRIDGE_H_
#define MEDIA_BASE_ANDROID_MEDIA_DRM_BRIDGE_H_

#include <jni.h>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"
#include "media/base/media_keys.h"
#include "url/gurl.h"

class GURL;

namespace media {

class MediaPlayerManager;

class MEDIA_EXPORT MediaDrmBridge : public MediaKeys {
 public:
  enum SecurityLevel {
    SECURITY_LEVEL_NONE = 0,
    SECURITY_LEVEL_1 = 1,
    SECURITY_LEVEL_3 = 3,
  };

  typedef base::Callback<void(bool)> ResetCredentialsCB;

  virtual ~MediaDrmBridge();

  
  
  static scoped_ptr<MediaDrmBridge> Create(
      int media_keys_id,
      const std::vector<uint8>& scheme_uuid,
      const GURL& frame_url,
      const std::string& security_level,
      MediaPlayerManager* manager);

  
  static bool IsAvailable();

  static bool IsSecurityLevelSupported(const std::vector<uint8>& scheme_uuid,
                                       const std::string& security_level);

  static bool IsCryptoSchemeSupported(const std::vector<uint8>& scheme_uuid,
                                      const std::string& container_mime_type);

  static bool IsSecureDecoderRequired(const std::string& security_level_str);

  static bool RegisterMediaDrmBridge(JNIEnv* env);

  
  virtual bool CreateSession(uint32 session_id,
                             const std::string& type,
                             const uint8* init_data,
                             int init_data_length) OVERRIDE;
  virtual void UpdateSession(uint32 session_id,
                             const uint8* response,
                             int response_length) OVERRIDE;
  virtual void ReleaseSession(uint32 session_id) OVERRIDE;

  
  
  base::android::ScopedJavaLocalRef<jobject> GetMediaCrypto();

  
  
  void SetMediaCryptoReadyCB(const base::Closure& closure);

  
  void OnMediaCryptoReady(JNIEnv* env, jobject j_media_drm);

  
  void OnSessionCreated(JNIEnv* env,
                        jobject j_media_drm,
                        jint j_session_id,
                        jstring j_web_session_id);
  void OnSessionMessage(JNIEnv* env,
                        jobject j_media_drm,
                        jint j_session_id,
                        jbyteArray j_message,
                        jstring j_destination_url);
  void OnSessionReady(JNIEnv* env, jobject j_media_drm, jint j_session_id);
  void OnSessionClosed(JNIEnv* env, jobject j_media_drm, jint j_session_id);
  void OnSessionError(JNIEnv* env, jobject j_media_drm, jint j_session_id);

  
  void ResetDeviceCredentials(const ResetCredentialsCB& callback);

  
  void OnResetDeviceCredentialsCompleted(JNIEnv* env, jobject, bool success);

  
  
  bool IsProtectedSurfaceRequired();

  int media_keys_id() const { return media_keys_id_; }

  GURL frame_url() const { return frame_url_; }

 private:
  static bool IsSecureDecoderRequired(SecurityLevel security_level);

  MediaDrmBridge(int media_keys_id,
                 const std::vector<uint8>& scheme_uuid,
                 const GURL& frame_url,
                 const std::string& security_level,
                 MediaPlayerManager* manager);

  
  SecurityLevel GetSecurityLevel();

  
  int media_keys_id_;

  
  std::vector<uint8> scheme_uuid_;

  
  const GURL frame_url_;

  
  base::android::ScopedJavaGlobalRef<jobject> j_media_drm_;

  
  MediaPlayerManager* manager_;

  base::Closure media_crypto_ready_cb_;

  ResetCredentialsCB reset_credentials_cb_;

  DISALLOW_COPY_AND_ASSIGN(MediaDrmBridge);
};

}  

#endif  
