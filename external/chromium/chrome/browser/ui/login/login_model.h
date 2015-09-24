// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_LOGIN_LOGIN_MODEL_H_
#define CHROME_BROWSER_UI_LOGIN_LOGIN_MODEL_H_
#pragma once

#include <string>

class LoginModelObserver {
 public:
  
  
  virtual void OnAutofillDataAvailable(const std::wstring& username,
                                       const std::wstring& password) = 0;

 protected:
  virtual ~LoginModelObserver() {}
};

class LoginModel {
 public:
  
  
  
  virtual void SetObserver(LoginModelObserver* observer) = 0;

 protected:
  virtual ~LoginModel() {}
};

#endif  
