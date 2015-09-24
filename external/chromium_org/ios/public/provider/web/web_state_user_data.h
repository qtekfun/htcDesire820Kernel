// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_PUBLIC_PROVIDER_WEB_WEB_STATE_USER_DATA_H_
#define IOS_PUBLIC_PROVIDER_WEB_WEB_STATE_USER_DATA_H_

#include "ios/public/consumer/base/supports_user_data.h"
#include "ios/public/provider/web/web_state.h"

namespace ios {

template <typename T>
class WebStateUserData : public ios::SupportsUserData::Data {
 public:
  
  
  static void CreateForWebState(WebState* web_state) {
    if (!FromWebState(web_state))
      web_state->SetUserData(UserDataKey(), new T(web_state));
  }

  
  
  
  static T* FromWebState(WebState* web_state) {
    return static_cast<T*>(web_state->GetUserData(UserDataKey()));
  }
  static const T* FromWebState(const WebState* web_state) {
    return static_cast<const T*>(web_state->GetUserData(UserDataKey()));
  }

 protected:
  static inline void* UserDataKey() {
    return &kLocatorKey;
  }

 private:
  
  static int kLocatorKey;
};

#define DEFINE_WEB_STATE_USER_DATA_KEY(TYPE) \
template<>                                      \
int ios::WebStateUserData<TYPE>::kLocatorKey = 0

}  

#endif  
