// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_PUBLIC_PROVIDER_WEB_WEB_STATE_H_
#define IOS_PUBLIC_PROVIDER_WEB_WEB_STATE_H_

#include "ios/public/consumer/base/supports_user_data.h"

namespace ios {

class WebState : public ios::SupportsUserData {
 public:
  virtual ~WebState() {}

 protected:
  WebState() {}
};

}  

#endif  
