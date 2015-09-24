// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_WEB_CONTENTS_DELEGATE_ANDROID_WEB_CONTENTS_DELEGATE_ANDROID_H_
#define COMPONENTS_WEB_CONTENTS_DELEGATE_ANDROID_WEB_CONTENTS_DELEGATE_ANDROID_H_

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_delegate.h"

class GURL;

namespace content {
class WebContents;
class WebContentsDelegate;
struct NativeWebKeyboardEvent;
struct OpenURLParams;
}

namespace web_contents_delegate_android {

class ValidationMessageBubbleAndroid;

enum WebContentsDelegateLogLevel {
  
  WEB_CONTENTS_DELEGATE_LOG_LEVEL_DEBUG = 0,
  
  WEB_CONTENTS_DELEGATE_LOG_LEVEL_LOG = 1,
  
  WEB_CONTENTS_DELEGATE_LOG_LEVEL_WARNING = 2,
  
  WEB_CONTENTS_DELEGATE_LOG_LEVEL_ERROR = 3,
};


class WebContentsDelegateAndroid : public content::WebContentsDelegate {
 public:
  WebContentsDelegateAndroid(JNIEnv* env, jobject obj);
  virtual ~WebContentsDelegateAndroid();

  
  
  
  void SetOwnerWebContents(content::WebContents* contents);

  
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE;
  virtual content::ColorChooser* OpenColorChooser(
      content::WebContents* source,
      SkColor color,
      const std::vector<content::ColorSuggestion>& suggestions) OVERRIDE;
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void ActivateContents(content::WebContents* contents) OVERRIDE;
  virtual void DeactivateContents(content::WebContents* contents) OVERRIDE;
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;
  virtual void LoadProgressChanged(content::WebContents* source,
                                   double load_progress) OVERRIDE;
  virtual void RendererUnresponsive(content::WebContents* source) OVERRIDE;
  virtual void RendererResponsive(content::WebContents* source) OVERRIDE;
  virtual void CloseContents(content::WebContents* source) OVERRIDE;
  virtual void MoveContents(content::WebContents* source,
                            const gfx::Rect& pos) OVERRIDE;
  virtual bool AddMessageToConsole(content::WebContents* source,
                                   int32 level,
                                   const base::string16& message,
                                   int32 line_no,
                                   const base::string16& source_id) OVERRIDE;
  virtual void UpdateTargetURL(content::WebContents* source,
                               int32 page_id,
                               const GURL& url) OVERRIDE;
  virtual void HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) OVERRIDE;
  virtual bool TakeFocus(content::WebContents* source, bool reverse) OVERRIDE;
  virtual void ShowRepostFormWarningDialog(
      content::WebContents* source) OVERRIDE;
  virtual void ToggleFullscreenModeForTab(content::WebContents* web_contents,
                                          bool enter_fullscreen) OVERRIDE;
  virtual bool IsFullscreenForTabOrPending(
      const content::WebContents* web_contents) const OVERRIDE;
  virtual void ShowValidationMessage(content::WebContents* web_contents,
                                     const gfx::Rect& anchor_in_root_view,
                                     const string16& main_text,
                                     const string16& sub_text) OVERRIDE;
  virtual void HideValidationMessage(
      content::WebContents* web_contents) OVERRIDE;
  virtual void MoveValidationMessage(
      content::WebContents* web_contents,
      const gfx::Rect& anchor_in_root_view) OVERRIDE;

 protected:
  base::android::ScopedJavaLocalRef<jobject> GetJavaDelegate(JNIEnv* env) const;

 private:
  
  
  
  JavaObjectWeakGlobalRef weak_java_delegate_;

  scoped_ptr<ValidationMessageBubbleAndroid> validation_message_bubble_;
};

bool RegisterWebContentsDelegateAndroid(JNIEnv* env);

}  

#endif  
