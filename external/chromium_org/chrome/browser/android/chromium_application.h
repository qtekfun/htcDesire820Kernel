// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_CHROMIUM_APPLICATION_H_
#define CHROME_BROWSER_ANDROID_CHROMIUM_APPLICATION_H_

#include <jni.h>

#include "base/basictypes.h"

namespace chrome {
namespace android {

class ChromiumApplication {
 public:
  static bool RegisterBindings(JNIEnv* env);

  
  static void OpenProtectedContentSettings();

  
  static void ShowSyncSettings();

  
  static void ShowTermsOfServiceDialog();

  
  static bool AreParentalControlsEnabled();

 private:
  ChromiumApplication() {}
  ~ChromiumApplication() {}

  DISALLOW_COPY_AND_ASSIGN(ChromiumApplication);
};

}  
}  

#endif  
