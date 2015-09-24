// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_PASSWORD_UI_VIEW_ANDROID_H_
#define CHROME_BROWSER_ANDROID_PASSWORD_UI_VIEW_ANDROID_H_

#include <vector>

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "chrome/browser/password_manager/password_store.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "chrome/browser/password_manager/password_store_factory.h"
#include "chrome/browser/ui/passwords/password_manager_presenter.h"
#include "chrome/browser/ui/passwords/password_ui_view.h"

class PasswordUIViewAndroid : public PasswordUIView {
 public:
  PasswordUIViewAndroid(JNIEnv* env, jobject);
  virtual ~PasswordUIViewAndroid();

  
  virtual Profile* GetProfile() OVERRIDE;
  virtual void ShowPassword(size_t index, const base::string16& password_value)
      OVERRIDE;
  virtual void SetPasswordList(
      const ScopedVector<autofill::PasswordForm>& password_list,
      bool show_passwords) OVERRIDE;
  virtual void SetPasswordExceptionList(
      const ScopedVector<autofill::PasswordForm>& password_exception_list)
      OVERRIDE;

  
  base::android::ScopedJavaLocalRef<jobject> GetSavedPasswordEntry(
      JNIEnv* env, jobject, int index);
  base::android::ScopedJavaLocalRef<jstring>
      GetSavedPasswordException(JNIEnv* env, jobject, int index);
  void UpdatePasswordLists(JNIEnv* env, jobject);
  void HandleRemoveSavedPasswordEntry(JNIEnv* env, jobject, int index);
  void HandleRemoveSavedPasswordException(JNIEnv* env, jobject, int index);
  
  void Destroy(JNIEnv*, jobject);

  
  static bool RegisterPasswordUIViewAndroid(JNIEnv* env);

 private:
  PasswordManagerPresenter password_manager_presenter_;
  
  JavaObjectWeakGlobalRef weak_java_ui_controller_;

  DISALLOW_COPY_AND_ASSIGN(PasswordUIViewAndroid);
};

#endif  
