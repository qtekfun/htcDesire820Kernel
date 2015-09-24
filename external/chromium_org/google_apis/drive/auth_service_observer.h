// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_AUTH_SERVICE_OBSERVER_H_
#define GOOGLE_APIS_DRIVE_AUTH_SERVICE_OBSERVER_H_

namespace google_apis {

class AuthServiceObserver {
 public:
  
  virtual void OnOAuth2RefreshTokenChanged() = 0;

 protected:
  virtual ~AuthServiceObserver() {}
};

}  

#endif  
