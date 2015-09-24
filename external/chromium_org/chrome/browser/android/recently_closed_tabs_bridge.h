// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_RECENTLY_CLOSED_TABS_BRIDGE_H_
#define CHROME_BROWSER_ANDROID_RECENTLY_CLOSED_TABS_BRIDGE_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "chrome/browser/sessions/tab_restore_service_observer.h"

class Profile;
class TabRestoreService;

class RecentlyClosedTabsBridge : public TabRestoreServiceObserver {
 public:
  explicit RecentlyClosedTabsBridge(Profile* profile);

  void Destroy(JNIEnv* env, jobject obj);
  void SetRecentlyClosedCallback(JNIEnv* env, jobject obj, jobject jcallback);
  jboolean GetRecentlyClosedTabs(JNIEnv* env,
                                 jobject obj,
                                 jobject jtabs,
                                 jint max_tab_count);
  jboolean OpenRecentlyClosedTab(JNIEnv* env,
                                 jobject obj,
                                 jobject jtab,
                                 jint tab_id);
  void ClearRecentlyClosedTabs(JNIEnv* env, jobject obj);

  
  
  virtual void TabRestoreServiceChanged(TabRestoreService* service) OVERRIDE;

  
  virtual void TabRestoreServiceDestroyed(TabRestoreService* service) OVERRIDE;

  
  static bool Register(JNIEnv* env);

 private:
  virtual ~RecentlyClosedTabsBridge();

  
  
  void EnsureTabRestoreService();

  
  base::android::ScopedJavaGlobalRef<jobject> callback_;

  
  Profile* profile_;

  
  TabRestoreService* tab_restore_service_;

  DISALLOW_COPY_AND_ASSIGN(RecentlyClosedTabsBridge);
};

#endif  
