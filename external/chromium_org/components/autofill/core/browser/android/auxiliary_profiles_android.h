// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_ANDROID_AUXILIARY_PROFILES_ANDROID_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_ANDROID_AUXILIARY_PROFILES_ANDROID_H_

#include <jni.h>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "components/autofill/core/browser/android/auxiliary_profile_loader_android.h"

namespace autofill {

class AutofillProfile;
class AuxiliaryProfileLoaderAndroid;

 
 
 
 
class AuxiliaryProfilesAndroid {
 public:
  
  
  AuxiliaryProfilesAndroid(
     const autofill::AuxiliaryProfileLoaderAndroid& profile_loader,
     const std::string& app_locale);
  ~AuxiliaryProfilesAndroid();

  
  scoped_ptr<AutofillProfile> LoadContactsProfile();

 private:
  
  void LoadAddress(AutofillProfile* profile);
  
  void LoadName(AutofillProfile* profile);
  
  void LoadEmailAddress(AutofillProfile* profile);
  
  void LoadPhoneNumbers(AutofillProfile* profile);

  const AuxiliaryProfileLoaderAndroid& profile_loader_;
  std::string app_locale_;

  DISALLOW_COPY_AND_ASSIGN(AuxiliaryProfilesAndroid);
};

} 

#endif  
