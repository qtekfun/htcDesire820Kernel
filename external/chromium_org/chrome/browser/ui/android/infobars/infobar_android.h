// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ANDROID_INFOBARS_INFOBAR_ANDROID_H_
#define CHROME_BROWSER_UI_ANDROID_INFOBARS_INFOBAR_ANDROID_H_

#include <string>

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/infobars/infobar.h"

class InfoBarDelegate;
class InfoBarService;

class InfoBarAndroid : public InfoBar {
 public:

  
  
  enum ActionType {
    ACTION_NONE = 0,
    
    ACTION_OK = 1,
    ACTION_CANCEL = 2,
    
    ACTION_TRANSLATE = 3,
    ACTION_TRANSLATE_SHOW_ORIGINAL = 4,
  };

  explicit InfoBarAndroid(scoped_ptr<InfoBarDelegate> delegate);
  virtual ~InfoBarAndroid();

  
  virtual base::android::ScopedJavaLocalRef<jobject> CreateRenderInfoBar(
      JNIEnv* env) = 0;

  void set_java_infobar(const base::android::JavaRef<jobject>& java_info_bar);
  bool HasSetJavaInfoBar() const;

  
  
  void ReassignJavaInfoBar(InfoBarAndroid* replacement);

  virtual void OnLinkClicked(JNIEnv* env, jobject obj) {}
  void OnButtonClicked(JNIEnv* env,
                       jobject obj,
                       jint action,
                       jstring action_value);
  void OnCloseButtonClicked(JNIEnv* env, jobject obj);

  void CloseJavaInfoBar();

  
  
  int GetEnumeratedIconId();

  
  
  virtual void PassJavaInfoBar(InfoBarAndroid* source) {}

 protected:
  
  
  virtual void ProcessButton(int action,
                             const std::string& action_value) = 0;
  void CloseInfoBar();

 private:
  base::android::ScopedJavaGlobalRef<jobject> java_info_bar_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarAndroid);
};

bool RegisterNativeInfoBar(JNIEnv* env);

#endif  
