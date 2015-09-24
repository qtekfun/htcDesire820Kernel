// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_ANDROID_AUXILIARY_PROFILE_LOADER_ANDROID_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_ANDROID_AUXILIARY_PROFILE_LOADER_ANDROID_H_

#include <vector>

#include "base/android/jni_android.h"
#include "base/strings/string16.h"

namespace autofill {

bool RegisterAuxiliaryProfileLoader(JNIEnv* env);

class AuxiliaryProfileLoaderAndroid {
 public:
  AuxiliaryProfileLoaderAndroid();
  virtual ~AuxiliaryProfileLoaderAndroid();

  
  
  void Init(JNIEnv* env, const jobject& context);

  
  
  virtual bool GetHasPermissions() const;
  
  virtual base::string16 GetStreet() const;
  
  virtual base::string16 GetPostOfficeBox() const;
  
  virtual base::string16 GetNeighborhood() const;
  
  
  virtual base::string16 GetRegion() const;
  
  virtual base::string16 GetCity() const;
  
  virtual base::string16 GetPostalCode() const;
  
  virtual base::string16 GetCountry() const;

  
  virtual base::string16 GetFirstName() const;
  
  virtual base::string16 GetMiddleName() const;
  
  virtual base::string16 GetLastName() const;
  
  virtual base::string16 GetSuffix() const;

  
  virtual void GetEmailAddresses(
      std::vector<base::string16>* email_addresses) const;

  
  virtual void GetPhoneNumbers(
      std::vector<base::string16>* phone_numbers) const;

 private:
  JNIEnv* env_;
  
  
  base::android::ScopedJavaLocalRef<jobject> populator_;
  DISALLOW_COPY_AND_ASSIGN(AuxiliaryProfileLoaderAndroid);
};

} 

#endif  
