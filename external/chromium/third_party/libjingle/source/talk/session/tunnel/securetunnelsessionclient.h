/*
 * libjingle
 * Copyright 2004--2008, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef TALK_SESSION_TUNNEL_SECURETUNNELSESSIONCLIENT_H_
#define TALK_SESSION_TUNNEL_SECURETUNNELSESSIONCLIENT_H_

#include <string>

#include "talk/base/sslidentity.h"
#include "talk/base/sslstreamadapter.h"
#include "talk/session/tunnel/tunnelsessionclient.h"

namespace cricket {

class SecureTunnelSession;  



class SecureTunnelSessionClient : public TunnelSessionClient {
 public:
  
  
  
  SecureTunnelSessionClient(const buzz::Jid& jid, SessionManager* manager);

  
  
  
  void SetIdentity(talk_base::SSLIdentity* identity);

  
  
  
  bool GenerateIdentity();

  
  
  
  
  talk_base::SSLIdentity& GetIdentity() const;

  
  virtual void OnIncomingTunnel(const buzz::Jid& jid, Session *session);
  virtual bool ParseContent(SignalingProtocol protocol,
                            const buzz::XmlElement* elem,
                            const ContentDescription** content,
                            ParseError* error);
  virtual bool WriteContent(SignalingProtocol protocol,
                            const ContentDescription* content,
                            buzz::XmlElement** elem,
                            WriteError* error);
  virtual SessionDescription* CreateOffer(
      const buzz::Jid &jid, const std::string &description);
  virtual SessionDescription* CreateAnswer(
      const SessionDescription* offer);

 protected:
  virtual TunnelSession* MakeTunnelSession(
      Session* session, talk_base::Thread* stream_thread,
      TunnelSessionRole role);

 private:
  
  
  
  
  talk_base::scoped_ptr<talk_base::SSLIdentity> identity_;

  DISALLOW_EVIL_CONSTRUCTORS(SecureTunnelSessionClient);
};


class SecureTunnelSession : public TunnelSession {
 public:
  
  
  
  
  
  SecureTunnelSession(SecureTunnelSessionClient* client, Session* session,
                      talk_base::Thread* stream_thread,
                      TunnelSessionRole role);

  
  
  
  virtual talk_base::StreamInterface* GetStream();

 protected:
  
  virtual void OnAccept();

  
  
  
  talk_base::StreamInterface* MakeSecureStream(
      talk_base::StreamInterface* stream);

  
  
  
  
  
  TunnelSessionRole role_;

  
  
  
  
  
  
  
  talk_base::scoped_ptr<talk_base::StreamReference> ssl_stream_reference_;

  DISALLOW_EVIL_CONSTRUCTORS(SecureTunnelSession);
};

}  

#endif  
