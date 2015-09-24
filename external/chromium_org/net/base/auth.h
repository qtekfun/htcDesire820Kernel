// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_AUTH_H__
#define NET_BASE_AUTH_H__

#include <string>

#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT AuthChallengeInfo :
    public base::RefCountedThreadSafe<AuthChallengeInfo> {
 public:
  AuthChallengeInfo();

  
  bool Equals(const AuthChallengeInfo& other) const;

  
  bool is_proxy;

  
  HostPortPair challenger;

  
  
  std::string scheme;

  
  std::string realm;

 private:
  friend class base::RefCountedThreadSafe<AuthChallengeInfo>;
  ~AuthChallengeInfo();
};

class NET_EXPORT AuthCredentials {
 public:
  AuthCredentials();
  AuthCredentials(const base::string16& username,
                  const base::string16& password);
  ~AuthCredentials();

  
  void Set(const base::string16& username, const base::string16& password);

  
  bool Equals(const AuthCredentials& other) const;

  
  bool Empty() const;

  
  
  void Zap();

  const base::string16& username() const { return username_; }
  const base::string16& password() const { return password_; }

 private:
  
  
  
  base::string16 username_;

  
  
  
  base::string16 password_;

  
  
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
  AuthCredentials credentials; 

  
  AuthData();

 private:
  friend class base::RefCountedThreadSafe<AuthData>;
  ~AuthData();
};

}  

#endif  
