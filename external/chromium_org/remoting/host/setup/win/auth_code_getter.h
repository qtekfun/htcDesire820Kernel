// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_SETUP_WIN_AUTH_CODE_GETTER_H
#define REMOTING_HOST_SETUP_WIN_AUTH_CODE_GETTER_H

#include <ole2.h>
#include <exdisp.h>

#include <string>

#include "base/callback.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "base/win/scoped_comptr.h"

namespace remoting {

class AuthCodeGetter : public base::NonThreadSafe {
 public:
  AuthCodeGetter();
  ~AuthCodeGetter();

  
  
  
  void GetAuthCode(base::Callback<void(const std::string&)> on_auth_code);

 private:
  
  void StartTimer();
  
  void OnTimer();
  
  
  bool TestBrowserUrl(std::string* auth_code);
  
  void KillBrowser();

  
  base::Callback<void(const std::string&)> on_auth_code_;
  
  base::win::ScopedComPtr<IWebBrowser2, &IID_IWebBrowser2> browser_;
  
  base::OneShotTimer<AuthCodeGetter> timer_;
  
  base::TimeDelta timer_interval_;

  DISALLOW_COPY_AND_ASSIGN(AuthCodeGetter);
};

}  

#endif  
