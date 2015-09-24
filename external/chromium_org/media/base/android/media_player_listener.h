// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_ANDROID_MEDIA_PLAYER_LISTENER_H_
#define MEDIA_BASE_ANDROID_MEDIA_PLAYER_LISTENER_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"

namespace base {
class MessageLoopProxy;
}

namespace media {

class MediaPlayerBridge;

class MediaPlayerListener {
 public:
  
  
  
  MediaPlayerListener(
      const scoped_refptr<base::MessageLoopProxy>& message_loop,
      base::WeakPtr<MediaPlayerBridge> media_player);
 virtual ~MediaPlayerListener();

  
  
  void OnMediaError(JNIEnv* , jobject , jint error_type);
  void OnVideoSizeChanged(JNIEnv* , jobject ,
                          jint width, jint height);
  void OnBufferingUpdate(JNIEnv* , jobject , jint percent);
  void OnPlaybackComplete(JNIEnv* , jobject );
  void OnSeekComplete(JNIEnv* , jobject );
  void OnMediaPrepared(JNIEnv* , jobject );
  void OnMediaInterrupted(JNIEnv* , jobject );

  
  void CreateMediaPlayerListener(jobject context, jobject media_player_bridge);
  void ReleaseMediaPlayerListenerResources();

  
  static bool RegisterMediaPlayerListener(JNIEnv* env);

 private:
  
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  
  base::WeakPtr<MediaPlayerBridge> media_player_;

  base::android::ScopedJavaGlobalRef<jobject> j_media_player_listener_;

  DISALLOW_COPY_AND_ASSIGN(MediaPlayerListener);
};

}  

#endif  
