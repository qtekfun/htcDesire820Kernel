// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_COMMUNICATOR_SINGLE_LOGIN_ATTEMPT_H_
#define JINGLE_NOTIFIER_COMMUNICATOR_SINGLE_LOGIN_ATTEMPT_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "jingle/notifier/base/xmpp_connection.h"
#include "jingle/notifier/communicator/connection_settings.h"
#include "jingle/notifier/communicator/login_settings.h"
#include "talk/xmpp/xmppengine.h"

namespace buzz {
class XmppTaskParentInterface;
}  

namespace notifier {

struct ServerInformation;

class SingleLoginAttempt : public XmppConnection::Delegate {
 public:
  
  
  
  
  class Delegate {
   public:
    
    virtual void OnConnect(
        base::WeakPtr<buzz::XmppTaskParentInterface> base_task) = 0;

    
    
    virtual void OnRedirect(const ServerInformation& redirect_server) = 0;

    
    
    
    virtual void OnCredentialsRejected() = 0;

    
    
    
    virtual void OnSettingsExhausted() = 0;

   protected:
    virtual ~Delegate();
  };

  
  SingleLoginAttempt(const LoginSettings& login_settings, Delegate* delegate);

  virtual ~SingleLoginAttempt();

  
  virtual void OnConnect(
      base::WeakPtr<buzz::XmppTaskParentInterface> parent) OVERRIDE;
  virtual void OnError(buzz::XmppEngine::Error error,
                       int error_subcode,
                       const buzz::XmlElement* stream_error) OVERRIDE;

 private:
  void TryConnect(const ConnectionSettings& new_settings);

  const LoginSettings login_settings_;
  Delegate* const delegate_;
  const ConnectionSettingsList settings_list_;
  ConnectionSettingsList::const_iterator current_settings_;
  scoped_ptr<XmppConnection> xmpp_connection_;

  DISALLOW_COPY_AND_ASSIGN(SingleLoginAttempt);
};

}  

#endif  
