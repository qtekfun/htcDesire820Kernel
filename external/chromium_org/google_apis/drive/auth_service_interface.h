// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_AUTH_SERVICE_INTERFACE_H_
#define GOOGLE_APIS_DRIVE_AUTH_SERVICE_INTERFACE_H_

#include <string>

#include "base/callback_forward.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace google_apis {

class AuthServiceObserver;

typedef base::Callback<void(GDataErrorCode error,
                            const std::string& access_token)>
    AuthStatusCallback;

class AuthServiceInterface {
 public:
  virtual ~AuthServiceInterface() {}

  
  virtual void AddObserver(AuthServiceObserver* observer) = 0;
  virtual void RemoveObserver(AuthServiceObserver* observer) = 0;

  
  
  virtual void StartAuthentication(const AuthStatusCallback& callback) = 0;

  
  
  virtual bool HasAccessToken() const = 0;

  
  
  
  virtual bool HasRefreshToken() const = 0;

  
  virtual const std::string& access_token() const = 0;

  
  virtual void ClearAccessToken() = 0;

  
  virtual void ClearRefreshToken() = 0;
};

}  

#endif  
