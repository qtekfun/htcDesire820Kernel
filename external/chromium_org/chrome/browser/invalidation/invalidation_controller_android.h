// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_CONTROLLER_ANDROID_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_CONTROLLER_ANDROID_H_

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_helper.h"
#include "sync/notifier/invalidation_util.h"

namespace invalidation {

class InvalidationControllerAndroid {
 public:
  InvalidationControllerAndroid();
  virtual ~InvalidationControllerAndroid();

  
  
  virtual void SetRegisteredObjectIds(const syncer::ObjectIdSet& ids);

  
  std::string GetInvalidatorClientId();

 private:
  
  base::android::ScopedJavaGlobalRef<jobject> invalidation_controller_;

  DISALLOW_COPY_AND_ASSIGN(InvalidationControllerAndroid);
};

bool RegisterInvalidationController(JNIEnv* env);

}  

#endif  
