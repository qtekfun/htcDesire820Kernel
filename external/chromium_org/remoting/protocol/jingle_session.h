// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_JINGLE_SESSION_H_
#define REMOTING_PROTOCOL_JINGLE_SESSION_H_

#include <list>
#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/timer/timer.h"
#include "crypto/rsa_private_key.h"
#include "net/base/completion_callback.h"
#include "remoting/jingle_glue/iq_sender.h"
#include "remoting/protocol/authenticator.h"
#include "remoting/protocol/channel_factory.h"
#include "remoting/protocol/jingle_messages.h"
#include "remoting/protocol/session.h"
#include "remoting/protocol/session_config.h"
#include "remoting/protocol/transport.h"

namespace net {
class Socket;
class StreamSocket;
}  

namespace remoting {
namespace protocol {

class ChannelMultiplexer;
class JingleSessionManager;

class JingleSession : public Session,
                      public ChannelFactory,
                      public Transport::EventHandler {
 public:
  virtual ~JingleSession();

  
  virtual void SetEventHandler(Session::EventHandler* event_handler) OVERRIDE;
  virtual ErrorCode error() OVERRIDE;
  virtual const std::string& jid() OVERRIDE;
  virtual const CandidateSessionConfig* candidate_config() OVERRIDE;
  virtual const SessionConfig& config() OVERRIDE;
  virtual void set_config(const SessionConfig& config) OVERRIDE;
  virtual ChannelFactory* GetTransportChannelFactory() OVERRIDE;
  virtual ChannelFactory* GetMultiplexedChannelFactory() OVERRIDE;
  virtual void Close() OVERRIDE;

  
  virtual void CreateStreamChannel(
      const std::string& name,
      const StreamChannelCallback& callback) OVERRIDE;
  virtual void CreateDatagramChannel(
      const std::string& name,
      const DatagramChannelCallback& callback) OVERRIDE;
  virtual void CancelChannelCreation(const std::string& name) OVERRIDE;

  
  virtual void OnTransportCandidate(
      Transport* transport,
      const cricket::Candidate& candidate) OVERRIDE;
  virtual void OnTransportRouteChange(Transport* transport,
                                      const TransportRoute& route) OVERRIDE;
  virtual void OnTransportReady(Transport* transport,
                                bool ready) OVERRIDE;
  virtual void OnTransportFailed(Transport* transport) OVERRIDE;
  virtual void OnTransportDeleted(Transport* transport) OVERRIDE;

 private:
  friend class JingleSessionManager;

  typedef std::map<std::string, Transport*> ChannelsMap;
  typedef base::Callback<void(JingleMessageReply::ErrorType)> ReplyCallback;

  explicit JingleSession(JingleSessionManager* session_manager);

  
  void StartConnection(const std::string& peer_jid,
                       scoped_ptr<Authenticator> authenticator,
                       scoped_ptr<CandidateSessionConfig> config);

  
  void AddPendingRemoteCandidates(Transport* channel, const std::string& name);

  
  void InitializeIncomingConnection(const JingleMessage& initiate_message,
                                    scoped_ptr<Authenticator> authenticator);
  void AcceptIncomingConnection(const JingleMessage& initiate_message);

  
  
  
  void SendMessage(const JingleMessage& message);

  
  void OnMessageResponse(JingleMessage::ActionType request_type,
                         IqRequest* request,
                         const buzz::XmlElement* response);

  
  void SendTransportInfo();

  
  
  void OnTransportInfoResponse(IqRequest* request,
                               const buzz::XmlElement* response);

  
  
  
  void OnIncomingMessage(const JingleMessage& message,
                         const ReplyCallback& reply_callback);

  
  void OnAccept(const JingleMessage& message,
                const ReplyCallback& reply_callback);
  void OnSessionInfo(const JingleMessage& message,
                     const ReplyCallback& reply_callback);
  void OnTerminate(const JingleMessage& message,
                   const ReplyCallback& reply_callback);
  void ProcessTransportInfo(const JingleMessage& message);

  
  bool InitializeConfigFromDescription(const ContentDescription* description);

  
  void ContinueAcceptIncomingConnection();
  
  void ProcessAuthenticationStep();

  
  
  
  void CloseInternal(ErrorCode error);

  
  void SetState(State new_state);

  JingleSessionManager* session_manager_;
  std::string peer_jid_;
  scoped_ptr<CandidateSessionConfig> candidate_config_;
  Session::EventHandler* event_handler_;

  std::string session_id_;
  State state_;
  ErrorCode error_;

  SessionConfig config_;
  bool config_is_set_;

  scoped_ptr<Authenticator> authenticator_;

  
  std::set<IqRequest*> pending_requests_;

  
  std::list<IqRequest*> transport_info_requests_;

  ChannelsMap channels_;
  scoped_ptr<ChannelMultiplexer> channel_multiplexer_;

  base::OneShotTimer<JingleSession> transport_infos_timer_;
  std::list<JingleMessage::NamedCandidate> pending_candidates_;

  
  std::list<JingleMessage::NamedCandidate> pending_remote_candidates_;

  DISALLOW_COPY_AND_ASSIGN(JingleSession);
};

}  
}  

#endif  
