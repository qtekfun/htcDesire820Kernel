// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_PAIRING_AUTHENTICATOR_BASE_H_
#define REMOTING_PROTOCOL_PAIRING_AUTHENTICATOR_BASE_H_

#include "base/memory/weak_ptr.h"
#include "remoting/protocol/authenticator.h"
#include "third_party/libjingle/source/talk/xmllite/xmlelement.h"

namespace remoting {
namespace protocol {

class PairingAuthenticatorBase : public Authenticator {
 public:
  PairingAuthenticatorBase();
  virtual ~PairingAuthenticatorBase();

  
  virtual State state() const OVERRIDE;
  virtual RejectionReason rejection_reason() const OVERRIDE;
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() OVERRIDE;
  virtual scoped_ptr<ChannelAuthenticator>
      CreateChannelAuthenticator() const OVERRIDE;

 protected:
  typedef base::Callback<void(scoped_ptr<Authenticator> authenticator)>
      SetAuthenticatorCallback;

  static const buzz::StaticQName kPairingInfoTag;
  static const buzz::StaticQName kClientIdAttribute;

  
  
  virtual void CreateV2AuthenticatorWithPIN(
      State initial_state,
      const SetAuthenticatorCallback& callback) = 0;

  
  
  virtual void AddPairingElements(buzz::XmlElement* message) = 0;

  
  
  
  
  std::string error_message_;

  
  
  scoped_ptr<Authenticator> v2_authenticator_;

  
  
  bool using_paired_secret_;

 private:
  
  void MaybeAddErrorMessage(buzz::XmlElement* message);
  bool HasErrorMessage(const buzz::XmlElement* message) const;
  void CheckForFailedSpakeExchange(const base::Closure& resume_callback);
  void SetAuthenticatorAndProcessMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback,
      scoped_ptr<Authenticator> authenticator);

  
  
  bool waiting_for_authenticator_;

  base::WeakPtrFactory<PairingAuthenticatorBase> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PairingAuthenticatorBase);
};

}  
}  

#endif  
