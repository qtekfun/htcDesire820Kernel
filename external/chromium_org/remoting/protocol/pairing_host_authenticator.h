// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_PAIRING_HOST_AUTHENTICATOR_H_
#define REMOTING_PROTOCOL_PAIRING_HOST_AUTHENTICATOR_H_

#include "base/memory/weak_ptr.h"
#include "remoting/protocol/pairing_authenticator_base.h"
#include "remoting/protocol/pairing_registry.h"

namespace remoting {

class RsaKeyPair;

namespace protocol {

class PairingRegistry;

class PairingHostAuthenticator : public PairingAuthenticatorBase {
 public:
  PairingHostAuthenticator(
      scoped_refptr<PairingRegistry> pairing_registry,
      const std::string& local_cert,
      scoped_refptr<RsaKeyPair> key_pair,
      const std::string& pin);
  virtual ~PairingHostAuthenticator();

  
  virtual State state() const OVERRIDE;
  virtual RejectionReason rejection_reason() const OVERRIDE;
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;

 private:
  
  virtual void CreateV2AuthenticatorWithPIN(
      State initial_state,
      const SetAuthenticatorCallback& callback) OVERRIDE;
  virtual void AddPairingElements(buzz::XmlElement* message) OVERRIDE;

  
  
  void ProcessMessageWithPairing(const buzz::XmlElement* message,
                                 const base::Closure& resume_callback,
                                 PairingRegistry::Pairing pairing);

  
  scoped_refptr<PairingRegistry> pairing_registry_;
  std::string local_cert_;
  scoped_refptr<RsaKeyPair> key_pair_;
  const std::string& pin_;
  bool protocol_error_;
  bool waiting_for_paired_secret_;

  base::WeakPtrFactory<PairingHostAuthenticator> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PairingHostAuthenticator);
};

}  
}  

#endif  
