// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_DELEGATE_H_
#define CHROME_BROWSER_SIGNIN_SIGNIN_MANAGER_DELEGATE_H_

class SigninManagerDelegate {
 public:
  virtual ~SigninManagerDelegate() {}

  
  
  virtual bool AreSigninCookiesAllowed() = 0;
};

#endif  
