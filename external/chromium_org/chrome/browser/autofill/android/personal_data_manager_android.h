// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOFILL_ANDROID_PERSONAL_DATA_MANAGER_ANDROID_H_
#define CHROME_BROWSER_AUTOFILL_ANDROID_PERSONAL_DATA_MANAGER_ANDROID_H_

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "components/autofill/core/browser/personal_data_manager.h"
#include "components/autofill/core/browser/personal_data_manager_observer.h"

namespace autofill {

class PersonalDataManagerAndroid : public PersonalDataManagerObserver {
 public:
  PersonalDataManagerAndroid(JNIEnv* env, jobject obj);

  
  

  
  jint GetProfileCount(JNIEnv* unused_env, jobject unused_obj);

  
  
  base::android::ScopedJavaLocalRef<jobject> GetProfileByIndex(
      JNIEnv* env,
      jobject unused_obj,
      jint index);

  
  
  
  base::android::ScopedJavaLocalRef<jobject> GetProfileByGUID(
      JNIEnv* env,
      jobject unused_obj,
      jstring jguid);

  
  
  
  base::android::ScopedJavaLocalRef<jstring> SetProfile(JNIEnv* env,
                                                        jobject unused_obj,
                                                        jobject jprofile);

  
  

  
  jint GetCreditCardCount(JNIEnv* unused_env, jobject unused_obj);

  
  
  base::android::ScopedJavaLocalRef<jobject> GetCreditCardByIndex(
      JNIEnv* env,
      jobject unused_obj,
      jint index);

  
  
  base::android::ScopedJavaLocalRef<jobject> GetCreditCardByGUID(
      JNIEnv* env,
      jobject unused_obj,
      jstring jguid);

  
  
  
  base::android::ScopedJavaLocalRef<jstring> SetCreditCard(
      JNIEnv* env,
      jobject unused_obj,
      jobject jcard);

  
  void RemoveByGUID(JNIEnv* env, jobject unused_obj, jstring jguid);

  
  virtual void OnPersonalDataChanged() OVERRIDE;

  
  static bool Register(JNIEnv* env);

 private:
  virtual ~PersonalDataManagerAndroid();

  
  JavaObjectWeakGlobalRef weak_java_obj_;

  
  PersonalDataManager* personal_data_manager_;

  DISALLOW_COPY_AND_ASSIGN(PersonalDataManagerAndroid);
};

}  

#endif  
