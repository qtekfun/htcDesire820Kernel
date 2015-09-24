// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_AUTHENTICATION_METHOD_H_
#define REMOTING_PROTOCOL_AUTHENTICATION_METHOD_H_

#include <string>

namespace remoting {
namespace protocol {

class Authenticator;

class AuthenticationMethod {
 public:
  enum MethodType {
    INVALID,
    SPAKE2,
    SPAKE2_PAIR,
    THIRD_PARTY
  };

  enum HashFunction {
    NONE,
    HMAC_SHA256,
  };

  
  static AuthenticationMethod Invalid();
  static AuthenticationMethod Spake2(HashFunction hash_function);
  static AuthenticationMethod Spake2Pair();
  static AuthenticationMethod ThirdParty();

  
  
  static AuthenticationMethod FromString(const std::string& value);

  
  
  static std::string ApplyHashFunction(HashFunction hash_function,
                                       const std::string& tag,
                                       const std::string& shared_secret);

  bool is_valid() const { return type_ != INVALID; }

  MethodType type() const { return type_; }

  

  
  HashFunction hash_function() const;

  
  const std::string ToString() const;

  
  
  bool operator ==(const AuthenticationMethod& other) const;
  bool operator !=(const AuthenticationMethod& other) const {
    return !(*this == other);
  }

 protected:
  AuthenticationMethod();
  AuthenticationMethod(MethodType type, HashFunction hash_function);

  MethodType type_;
  HashFunction hash_function_;
};

struct SharedSecretHash {
  AuthenticationMethod::HashFunction hash_function;
  std::string value;

  
  
  bool Parse(const std::string& as_string);
};

}  
}  

#endif  
