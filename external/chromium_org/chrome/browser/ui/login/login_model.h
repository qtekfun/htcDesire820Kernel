// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LOGIN_LOGIN_MODEL_H_
#define CHROME_BROWSER_UI_LOGIN_LOGIN_MODEL_H_

#include "base/strings/string16.h"

class LoginModelObserver {
 public:
  
  
  virtual void OnAutofillDataAvailable(const base::string16& username,
                                       const base::string16& password) = 0;

  virtual void OnLoginModelDestroying() = 0;

 protected:
  virtual ~LoginModelObserver() {}
};

class LoginModel {
 public:
  
  virtual void AddObserver(LoginModelObserver* observer) = 0;
  
  virtual void RemoveObserver(LoginModelObserver* observer) = 0;

 protected:
  virtual ~LoginModel() {}
};

#endif  
