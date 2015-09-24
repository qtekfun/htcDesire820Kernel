// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_ANDROID_ACTIVITY_STATUS_H_
#define BASE_ANDROID_ACTIVITY_STATUS_H_

#include <jni.h>

#include "base/android/jni_android.h"
#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/observer_list_threadsafe.h"

namespace base {
namespace android {

enum ActivityState {
#define DEFINE_ACTIVITY_STATE(x, y) ACTIVITY_STATE_##x = y,
#include "base/android/activity_state_list.h"
#undef DEFINE_ACTIVITY_STATE
};

class BASE_EXPORT ActivityStatus {
 public:
  typedef base::Callback<void(ActivityState)> StateChangeCallback;

  class Listener {
   public:
    explicit Listener(const StateChangeCallback& callback);
    ~Listener();

   private:
    friend class ActivityStatus;

    void Notify(ActivityState state);

    StateChangeCallback callback_;

    DISALLOW_COPY_AND_ASSIGN(Listener);
  };

  
  static ActivityStatus* GetInstance();

  
  static bool RegisterBindings(JNIEnv* env);

  
  void OnActivityStateChange(ActivityState new_state);

 private:
  friend struct DefaultSingletonTraits<ActivityStatus>;

  ActivityStatus();
  ~ActivityStatus();

  void RegisterListener(Listener* listener);
  void UnregisterListener(Listener* listener);

  scoped_refptr<ObserverListThreadSafe<Listener> > observers_;

  DISALLOW_COPY_AND_ASSIGN(ActivityStatus);
};

}  
}  

#endif  
