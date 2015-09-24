// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_ANDROID_H_
#define CONTENT_BROWSER_ACCESSIBILITY_BROWSER_ACCESSIBILITY_MANAGER_ANDROID_H_

#include "base/android/scoped_java_ref.h"
#include "content/browser/accessibility/browser_accessibility_manager.h"
#include "content/browser/android/content_view_core_impl.h"

namespace content {

namespace aria_strings {
  extern const char kAriaLivePolite[];
  extern const char kAriaLiveAssertive[];
}

class CONTENT_EXPORT BrowserAccessibilityManagerAndroid
    : public BrowserAccessibilityManager {
 public:
  BrowserAccessibilityManagerAndroid(
      base::android::ScopedJavaLocalRef<jobject> content_view_core,
      const AccessibilityNodeData& src,
      BrowserAccessibilityDelegate* delegate,
      BrowserAccessibilityFactory* factory = new BrowserAccessibilityFactory());

  virtual ~BrowserAccessibilityManagerAndroid();

  static AccessibilityNodeData GetEmptyDocument();

  void SetContentViewCore(
      base::android::ScopedJavaLocalRef<jobject> content_view_core);

  
  virtual void NotifyAccessibilityEvent(
      blink::WebAXEvent event_type, BrowserAccessibility* node) OVERRIDE;

  
  
  

  
  jint GetRootId(JNIEnv* env, jobject obj);
  jboolean IsNodeValid(JNIEnv* env, jobject obj, jint id);
  jint HitTest(JNIEnv* env, jobject obj, jint x, jint y);

  
  jboolean PopulateAccessibilityNodeInfo(
      JNIEnv* env, jobject obj, jobject info, jint id);
  jboolean PopulateAccessibilityEvent(
      JNIEnv* env, jobject obj, jobject event, jint id, jint event_type);

  
  void Click(JNIEnv* env, jobject obj, jint id);
  void Focus(JNIEnv* env, jobject obj, jint id);
  void Blur(JNIEnv* env, jobject obj);

 protected:
  virtual void NotifyRootChanged() OVERRIDE;

  virtual bool UseRootScrollOffsetsWhenComputingBounds() OVERRIDE;

 private:
  
  
  friend class BrowserAccessibilityManager;

  
  
  
  
  
  JavaObjectWeakGlobalRef java_ref_;

  
  
  
  BrowserAccessibility* FuzzyHitTest(
      int x, int y, BrowserAccessibility* start_node);

  static void FuzzyHitTestImpl(int x, int y, BrowserAccessibility* start_node,
      BrowserAccessibility** nearest_candidate, int* min_distance);

  
  
  static int CalculateDistanceSquared(int x, int y, BrowserAccessibility* node);

  DISALLOW_COPY_AND_ASSIGN(BrowserAccessibilityManagerAndroid);
};

bool RegisterBrowserAccessibilityManager(JNIEnv* env);

}

#endif  
