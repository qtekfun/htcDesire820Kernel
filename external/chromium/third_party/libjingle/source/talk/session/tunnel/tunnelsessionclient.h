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

#ifndef __TUNNELSESSIONCLIENT_H__
#define __TUNNELSESSIONCLIENT_H__

#include <vector>

#include "talk/base/criticalsection.h"
#include "talk/base/stream.h"
#include "talk/p2p/base/constants.h"
#include "talk/p2p/base/pseudotcp.h"
#include "talk/p2p/base/session.h"
#include "talk/p2p/base/sessiondescription.h"
#include "talk/p2p/base/sessionmanager.h"
#include "talk/p2p/base/sessionclient.h"
#include "talk/xmllite/qname.h"
#include "talk/xmpp/constants.h"

namespace cricket {

class TunnelSession;
class TunnelStream;

enum TunnelSessionRole { INITIATOR, RESPONDER };


class TunnelSessionClientBase
  : public SessionClient, public talk_base::MessageHandler {
public:
  TunnelSessionClientBase(const buzz::Jid& jid, SessionManager* manager,
                          const std::string &ns);
  virtual ~TunnelSessionClientBase();

  const buzz::Jid& jid() const { return jid_; }
  SessionManager* session_manager() const { return session_manager_; }

  void OnSessionCreate(Session* session, bool received);
  void OnSessionDestroy(Session* session);

  
  
  
  talk_base::StreamInterface* CreateTunnel(const buzz::Jid& to,
                                           const std::string& description);

  talk_base::StreamInterface* AcceptTunnel(Session* session);
  void DeclineTunnel(Session* session);

  
  virtual void OnIncomingTunnel(const buzz::Jid &jid, Session *session) = 0;

  
  virtual SessionDescription* CreateOffer(
      const buzz::Jid &jid, const std::string &description) = 0;
  
  
  virtual SessionDescription* CreateAnswer(
      const SessionDescription* offer) = 0;

protected:

  void OnMessage(talk_base::Message* pmsg);

  
  
  
  virtual TunnelSession* MakeTunnelSession(Session* session,
                                           talk_base::Thread* stream_thread,
                                           TunnelSessionRole role);

  buzz::Jid jid_;
  SessionManager* session_manager_;
  std::vector<TunnelSession*> sessions_;
  std::string namespace_;
  bool shutdown_;
};

class TunnelSessionClient
  : public TunnelSessionClientBase, public sigslot::has_slots<>  {
public:
  TunnelSessionClient(const buzz::Jid& jid, SessionManager* manager);
  TunnelSessionClient(const buzz::Jid& jid, SessionManager* manager,
                      const std::string &ns);
  virtual ~TunnelSessionClient();

  virtual bool ParseContent(SignalingProtocol protocol,
                            const buzz::XmlElement* elem,
                            const ContentDescription** content,
                            ParseError* error);
  virtual bool WriteContent(SignalingProtocol protocol,
                            const ContentDescription* content,
                            buzz::XmlElement** elem,
                            WriteError* error);

  
  sigslot::signal4<TunnelSessionClient*, buzz::Jid, std::string, Session*>
    SignalIncomingTunnel;

  virtual void OnIncomingTunnel(const buzz::Jid &jid,
                                Session *session);
  virtual SessionDescription* CreateOffer(
      const buzz::Jid &jid, const std::string &description);
  virtual SessionDescription* CreateAnswer(
      const SessionDescription* offer);
};


class PseudoTcpChannel;

class TunnelSession : public sigslot::has_slots<> {
 public:
  
  TunnelSession(TunnelSessionClientBase* client, Session* session,
                talk_base::Thread* stream_thread);

  virtual talk_base::StreamInterface* GetStream();
  bool HasSession(Session* session);
  Session* ReleaseSession(bool channel_exists);

 protected:
  virtual ~TunnelSession();

  virtual void OnSessionState(BaseSession* session, BaseSession::State state);
  virtual void OnInitiate();
  virtual void OnAccept();
  virtual void OnTerminate();
  virtual void OnChannelClosed(PseudoTcpChannel* channel);

  TunnelSessionClientBase* client_;
  Session* session_;
  PseudoTcpChannel* channel_;
};


} 

#endif 
