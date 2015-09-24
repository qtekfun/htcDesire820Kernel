// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_AUTHENTICATOR_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"

namespace buzz {
class XmlElement;
}  

namespace remoting {
namespace protocol {

class ChannelAuthenticator;

typedef base::Callback<void(const std::string& secret)> SecretFetchedCallback;
typedef base::Callback<void(
    bool pairing_supported,
    const SecretFetchedCallback& secret_fetched_callback)> FetchSecretCallback;

class Authenticator {
 public:
  
  
  
  
  
  
  
  
  
  
  
  enum State {
    
    WAITING_MESSAGE,

    
    MESSAGE_READY,

    
    ACCEPTED,

    
    REJECTED,

    
    PROCESSING_MESSAGE,
  };

  enum RejectionReason {
    INVALID_CREDENTIALS,
    PROTOCOL_ERROR,
  };

  
  static bool IsAuthenticatorMessage(const buzz::XmlElement* message);

  
  static scoped_ptr<buzz::XmlElement> CreateEmptyAuthenticatorMessage();

  
  
  static const buzz::XmlElement* FindAuthenticatorMessage(
      const buzz::XmlElement* message);

  Authenticator() {}
  virtual ~Authenticator() {}

  
  virtual State state() const = 0;

  
  virtual RejectionReason rejection_reason() const = 0;

  
  
  
  
  
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) = 0;

  
  
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() = 0;

  
  
  virtual scoped_ptr<ChannelAuthenticator>
      CreateChannelAuthenticator() const = 0;
};

class AuthenticatorFactory {
 public:
  AuthenticatorFactory() {}
  virtual ~AuthenticatorFactory() {}

  
  
  
  
  
  
  
  
  virtual scoped_ptr<Authenticator> CreateAuthenticator(
      const std::string& local_jid,
      const std::string& remote_jid,
      const buzz::XmlElement* first_message) = 0;
};

}  
}  

#endif  
