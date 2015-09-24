/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
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

#ifndef TALK_P2P_BASE_SESSION_H_
#define TALK_P2P_BASE_SESSION_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "talk/p2p/base/sessionmessages.h"
#include "talk/p2p/base/sessionmanager.h"
#include "talk/base/socketaddress.h"
#include "talk/p2p/base/sessionclient.h"
#include "talk/p2p/base/parsing.h"
#include "talk/p2p/base/port.h"
#include "talk/xmllite/xmlelement.h"
#include "talk/xmpp/constants.h"

namespace cricket {

class P2PTransportChannel;
class Transport;
class TransportChannel;
class TransportChannelProxy;
class TransportChannelImpl;

struct MessageError : ParseError {
  buzz::QName type;

  
  MessageError() : ParseError(), type(buzz::QN_STANZA_BAD_REQUEST) {}

  void SetType(const buzz::QName type) {
    this->type = type;
  }
};

struct SessionError : WriteError {
};

class TransportProxy {
 public:
  TransportProxy(const std::string& content_name, Transport* transport)
      : content_name_(content_name),
        transport_(transport),
        state_(STATE_INIT),
        sent_candidates_(false) {}
  ~TransportProxy();

  std::string content_name() const { return content_name_; }
  Transport* impl() const { return transport_; }
  std::string type() const;
  bool negotiated() const { return state_ == STATE_NEGOTIATED; }
  const Candidates& sent_candidates() const { return sent_candidates_; }

  TransportChannel* GetChannel(const std::string& name);
  TransportChannel* CreateChannel(const std::string& name,
                                  const std::string& content_type);
  void DestroyChannel(const std::string& name);
  void AddSentCandidates(const Candidates& candidates);
  void ClearSentCandidates() { sent_candidates_.clear(); }
  void SpeculativelyConnectChannels();
  void CompleteNegotiation();

 private:
  enum TransportState {
    STATE_INIT,
    STATE_CONNECTING,
    STATE_NEGOTIATED
  };

  typedef std::map<std::string, TransportChannelProxy*> ChannelMap;

  TransportChannelProxy* GetProxy(const std::string& name);
  TransportChannelImpl* GetOrCreateImpl(const std::string& name,
                                        const std::string& content_type);
  void SetProxyImpl(const std::string& name, TransportChannelProxy* proxy);

  std::string content_name_;
  Transport* transport_;
  TransportState state_;
  ChannelMap channels_;
  Candidates sent_candidates_;
};

typedef std::map<std::string, TransportProxy*> TransportMap;


class BaseSession : public sigslot::has_slots<>,
                    public talk_base::MessageHandler {
 public:
  enum State {
    STATE_INIT = 0,
    STATE_SENTINITIATE,       
    STATE_RECEIVEDINITIATE,   
    STATE_SENTACCEPT,         
    STATE_RECEIVEDACCEPT,     
    STATE_SENTMODIFY,         
    STATE_RECEIVEDMODIFY,     
    STATE_SENTREJECT,         
    STATE_RECEIVEDREJECT,     
    STATE_SENTREDIRECT,       
    STATE_SENTTERMINATE,      
    STATE_RECEIVEDTERMINATE,  
    STATE_INPROGRESS,         
    STATE_DEINIT,             
  };

  enum Error {
    ERROR_NONE = 0,      
    ERROR_TIME = 1,      
    ERROR_RESPONSE = 2,  
    ERROR_NETWORK = 3,   
    ERROR_CONTENT = 4,   
  };

  explicit BaseSession(talk_base::Thread *signaling_thread);
  virtual ~BaseSession();

  
  void SetState(State state);

  
  virtual void SetError(Error error);

  
  virtual void OnMessage(talk_base::Message *pmsg);

  
  
  State state() const { return state_; }
  sigslot::signal2<BaseSession *, State> SignalState;

  
  
  Error error() const { return error_; }
  sigslot::signal2<BaseSession *, Error> SignalError;

  
  
  
  
  
  
  virtual TransportChannel* CreateChannel(const std::string& content_name,
                                          const std::string& channel_name) = 0;

  
  virtual TransportChannel* GetChannel(const std::string& content_name,
                                       const std::string& channel_name) = 0;

  
  
  
  
  virtual void DestroyChannel(const std::string& content_name,
                              const std::string& channel_name) = 0;

  
  sigslot::signal2<Session*, const std::string&> SignalChannelGone;

  
  
  const SessionDescription* local_description() const {
    return local_description_;
  }
  
  bool set_local_description(const SessionDescription* sdesc) {
    if (sdesc != local_description_) {
      delete local_description_;
      local_description_ = sdesc;
    }
    return true;
  }

  
  
  const SessionDescription* remote_description() const {
    return remote_description_;
  }
  
  bool set_remote_description(const SessionDescription* sdesc) {
    if (sdesc != remote_description_) {
      delete remote_description_;
      remote_description_ = sdesc;
    }
    return true;
  }

  
  
  
  virtual bool Accept(const SessionDescription* sdesc) = 0;
  virtual bool Reject(const std::string& reason) = 0;
  bool Terminate() {
    return TerminateWithReason(STR_TERMINATE_SUCCESS);
  }
  virtual bool TerminateWithReason(const std::string& reason) = 0;

  
  virtual talk_base::Thread *worker_thread() = 0;

  talk_base::Thread *signaling_thread() {
    return signaling_thread_;
  }

  
  const std::string& local_name() const { return local_name_; }

  
  const std::string& remote_name() const { return remote_name_; }

  
  
  
  
  
  void set_remote_name(const std::string& name) { remote_name_ = name; }

  const std::string& id() const { return sid_; }

 protected:
  State state_;
  Error error_;
  const SessionDescription* local_description_;
  const SessionDescription* remote_description_;
  std::string sid_;
  
  
  
  std::string local_name_;
  std::string remote_name_;
  talk_base::Thread *signaling_thread_;
};

class Session : public BaseSession {
 public:
  
  SessionManager* session_manager() const { return session_manager_; }

  
  talk_base::Thread *worker_thread() {
    return session_manager_->worker_thread();
  }

  
  const std::string& content_type() const { return content_type_; }

  
  SessionClient* client() const { return client_; }

  SignalingProtocol current_protocol() const { return current_protocol_; }

  void set_current_protocol(SignalingProtocol protocol) {
    current_protocol_ = protocol;
  }

  
  bool initiator() const { return initiator_; }

  const SessionDescription* initiator_description() const {
    if (initiator_) {
      return local_description_;
    } else {
      return remote_description_;
    }
  }

  
  sigslot::signal2<Session*, const std::string&> SignalReceivedTerminateReason;

  void set_allow_local_ips(bool allow);

  
  
  Transport* GetTransport(const std::string& content_name);

  
  
  bool Initiate(const std::string& to,
                const SessionDescription* sdesc);

  
  
  
  
  virtual bool Accept(const SessionDescription* sdesc);
  virtual bool Reject(const std::string& reason);
  virtual bool TerminateWithReason(const std::string& reason);

  
  
  
  bool SendInfoMessage(const XmlElements& elems);
  sigslot::signal2<Session*, const XmlElements&> SignalInfoMessage;

  
  TransportParserMap GetTransportParsers();
  ContentParserMap GetContentParsers();

  
  
  
  virtual TransportChannel* CreateChannel(const std::string& content_name,
                                          const std::string& channel_name);

  
  virtual TransportChannel* GetChannel(const std::string& content_name,
                                       const std::string& channel_name);

  
  virtual void DestroyChannel(const std::string& content_name,
                              const std::string& channel_name);

  
  virtual void SetError(Error error);

  
  virtual void OnMessage(talk_base::Message *pmsg);

  
  
  
  sigslot::signal1<const StringToMediaSourcesMap&> SignalMediaSources;

  
  bool SetVideoView(const VideoViewRequestVector& view_requests);

 private:
  
  Session(SessionManager *session_manager,
          const std::string& local_name, const std::string& initiator_name,
          const std::string& sid, const std::string& content_type,
          SessionClient* client);
  ~Session();

  
  TransportProxy* GetTransportProxy(const std::string& content_name);
  TransportProxy* GetTransportProxy(const Transport* transport);
  TransportProxy* GetFirstTransportProxy();
  
  TransportProxy* GetOrCreateTransportProxy(const std::string& content_name);
  
  
  bool CreateTransportProxies(const TransportInfos& tinfos,
                              SessionError* error);
  void SpeculativelyConnectAllTransportChannels();
  bool OnRemoteCandidates(const TransportInfos& tinfos,
                          ParseError* error);
  
  
  TransportInfos GetEmptyTransportInfos(const ContentInfos& contents) const;

  
  
  
  void OnTransportConnecting(Transport* transport);

  
  
  
  void OnTransportWritable(Transport* transport);

  
  void OnTransportRequestSignaling(Transport* transport);

  
  
  
  void OnTransportCandidatesReady(Transport* transport,
                                  const Candidates& candidates);

  
  
  void OnTransportSendError(Transport* transport,
                            const buzz::XmlElement* stanza,
                            const buzz::QName& name,
                            const std::string& type,
                            const std::string& text,
                            const buzz::XmlElement* extra_info);

  
  
  void OnTransportChannelGone(Transport* transport, const std::string& name);

  
  
  
  
  sigslot::signal1<Session*> SignalRequestSignaling;
  void OnSignalingReady();

  
  bool SendInitiateMessage(const SessionDescription* sdesc,
                           SessionError* error);
  bool SendAcceptMessage(const SessionDescription* sdesc, SessionError* error);
  bool SendRejectMessage(const std::string& reason, SessionError* error);
  bool SendTerminateMessage(const std::string& reason, SessionError* error);
  bool SendTransportInfoMessage(const TransportInfo& tinfo,
                                SessionError* error);
  bool SendViewMessage(const SessionView& view, SessionError* error);
  bool ResendAllTransportInfoMessages(SessionError* error);

  
  
  
  
  

  
  
  bool SendMessage(ActionType type, const XmlElements& action_elems,
                   SessionError* error);
  
  template <typename Action>
  bool SendMessage(ActionType type, const Action& action,
                   SessionError* error);

  
  template <typename Action>
  bool WriteActionMessage(ActionType type, const Action& action,
                          buzz::XmlElement* stanza, WriteError* error);
  template <typename Action>
  bool WriteActionMessage(SignalingProtocol protocol,
                          ActionType type, const Action& action,
                          buzz::XmlElement* stanza, WriteError* error);

  
  
  
  bool WriteSessionAction(SignalingProtocol protocol,
                          const SessionInitiate& init,
                          XmlElements* elems, WriteError* error);
  bool WriteSessionAction(SignalingProtocol protocol,
                          const TransportInfo& tinfo,
                          XmlElements* elems, WriteError* error);
  bool WriteSessionAction(SignalingProtocol protocol,
                          const SessionTerminate& term,
                          XmlElements* elems, WriteError* error);

  
  
  void SendAcknowledgementMessage(const buzz::XmlElement* stanza);

  
  
  
  
  sigslot::signal2<Session *, const buzz::XmlElement*> SignalOutgoingMessage;
  void OnIncomingMessage(const SessionMessage& msg);

  void OnFailedSend(const buzz::XmlElement* orig_stanza,
                    const buzz::XmlElement* error_stanza);

  
  
  sigslot::signal6<BaseSession*,
                   const buzz::XmlElement*,
                   const buzz::QName&,
                   const std::string&,
                   const std::string&,
                   const buzz::XmlElement*> SignalErrorMessage;

  
  
  bool OnInitiateMessage(const SessionMessage& msg, MessageError* error);
  bool OnAcceptMessage(const SessionMessage& msg, MessageError* error);
  bool OnRejectMessage(const SessionMessage& msg, MessageError* error);
  bool OnInfoMessage(const SessionMessage& msg);
  bool OnTerminateMessage(const SessionMessage& msg, MessageError* error);
  bool OnTransportInfoMessage(const SessionMessage& msg, MessageError* error);
  bool OnTransportAcceptMessage(const SessionMessage& msg, MessageError* error);
  bool OnNotifyMessage(const SessionMessage& msg, MessageError* error);
  bool OnUpdateMessage(const SessionMessage& msg, MessageError* error);
  bool OnRedirectError(const SessionRedirect& redirect, SessionError* error);

  
  bool CheckState(State state, MessageError* error);

  SessionManager *session_manager_;
  bool initiator_;
  std::string initiator_name_;
  std::string content_type_;
  SessionClient* client_;
  std::string transport_type_;
  TransportParser* transport_parser_;
  
  
  bool allow_local_ips_;
  TransportMap transports_;
  
  SignalingProtocol current_protocol_;

  friend class SessionManager;  
                                
};

}  

#endif  
