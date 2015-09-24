// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_AUTH_H__
#define NET_BASE_AUTH_H__
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT AuthChallengeInfo :
    public base::RefCountedThreadSafe<AuthChallengeInfo> {
 public:
  AuthChallengeInfo();

  bool operator==(const AuthChallengeInfo& that) const;

  bool operator!=(const AuthChallengeInfo& that) const {
    return !(*this == that);
  }

  bool is_proxy;  
  std::wstring host_and_port;  
                               
  std::wstring scheme;  
  std::wstring realm;  

 private:
  friend class base::RefCountedThreadSafe<AuthChallengeInfo>;
  ~AuthChallengeInfo();
};

enum AuthState {
  AUTH_STATE_DONT_NEED_AUTH,
  AUTH_STATE_NEED_AUTH,
  AUTH_STATE_HAVE_AUTH,
  AUTH_STATE_CANCELED
};

class AuthData : public base::RefCountedThreadSafe<AuthData> {
 public:
  AuthState state;  
  std::wstring scheme;  
  string16 username;  
  string16 password;  

  
  AuthData();

 private:
  friend class base::RefCountedThreadSafe<AuthData>;
  ~AuthData();
};

}  

#endif  
