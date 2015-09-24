// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CONTENT_VIDEO_VIEW_H_
#define CONTENT_BROWSER_ANDROID_CONTENT_VIDEO_VIEW_H_

#include <jni.h>

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "ui/gfx/native_widget_types.h"

namespace content {

class BrowserMediaPlayerManager;
class PowerSaveBlocker;

class ContentVideoView {
 public:
  
  
  ContentVideoView(
      const base::android::ScopedJavaLocalRef<jobject>& context,
      const base::android::ScopedJavaLocalRef<jobject>& client,
      BrowserMediaPlayerManager* manager);

  ~ContentVideoView();

  
  void OpenVideo();

  static bool RegisterContentVideoView(JNIEnv* env);
  static void KeepScreenOn(bool screen_on);

  
  static bool HasContentVideoView();

  
  int GetVideoWidth(JNIEnv*, jobject obj) const;
  int GetVideoHeight(JNIEnv*, jobject obj) const;
  int GetDurationInMilliSeconds(JNIEnv*, jobject obj) const;
  int GetCurrentPosition(JNIEnv*, jobject obj) const;
  bool IsPlaying(JNIEnv*, jobject obj);
  void UpdateMediaMetadata(JNIEnv*, jobject obj);

  
  
  
  void ExitFullscreen(JNIEnv*, jobject, jboolean release_media_player);

  
  void SeekTo(JNIEnv*, jobject obj, jint msec);
  void Play(JNIEnv*, jobject obj);
  void Pause(JNIEnv*, jobject obj);

  
  void SetSurface(JNIEnv*, jobject obj, jobject surface);

  
  
  void UpdateMediaMetadata();
  void OnMediaPlayerError(int errorType);
  void OnVideoSizeChanged(int width, int height);
  void OnBufferingUpdate(int percent);
  void OnPlaybackComplete();
  void OnExitFullscreen();

  
  base::android::ScopedJavaLocalRef<jobject> GetJavaObject(JNIEnv* env);

 private:
  
  
  void DestroyContentVideoView(bool native_view_destroyed);

  
  gfx::NativeView GetNativeView();

  void CreatePowerSaveBlocker();

  
  
  BrowserMediaPlayerManager* manager_;

  
  
  
  
  
  
  scoped_ptr<PowerSaveBlocker> power_save_blocker_;

  
  JavaObjectWeakGlobalRef j_content_video_view_;

  DISALLOW_COPY_AND_ASSIGN(ContentVideoView);
};

} 

#endif  
