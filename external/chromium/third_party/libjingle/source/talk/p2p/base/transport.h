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


#ifndef TALK_P2P_BASE_TRANSPORT_H_
#define TALK_P2P_BASE_TRANSPORT_H_

#include <string>
#include <map>
#include <vector>
#include "talk/base/criticalsection.h"
#include "talk/base/messagequeue.h"
#include "talk/base/sigslot.h"
#include "talk/p2p/base/candidate.h"
#include "talk/p2p/base/constants.h"

namespace talk_base {
class Thread;
}

namespace buzz {
class QName;
class XmlElement;
}

namespace cricket {

struct ParseError;
struct WriteError;
class PortAllocator;
class SessionManager;
class Session;
class TransportChannel;
class TransportChannelImpl;

typedef std::vector<buzz::XmlElement*> XmlElements;
typedef std::vector<Candidate> Candidates;

class TransportParser {
 public:
  virtual bool ParseCandidates(SignalingProtocol protocol,
                               const buzz::XmlElement* elem,
                               Candidates* candidates,
                               ParseError* error) = 0;
  virtual bool WriteCandidates(SignalingProtocol protocol,
                               const Candidates& candidates,
                               XmlElements* candidate_elems,
                               WriteError* error) = 0;

  
  
  
  bool ParseAddress(const buzz::XmlElement* elem,
                    const buzz::QName& address_name,
                    const buzz::QName& port_name,
                    talk_base::SocketAddress* address,
                    ParseError* error);

  virtual ~TransportParser() {}
};

class Transport : public talk_base::MessageHandler,
                  public sigslot::has_slots<> {
 public:
  Transport(talk_base::Thread* signaling_thread,
            talk_base::Thread* worker_thread,
            const std::string& type,
            PortAllocator* allocator);
  virtual ~Transport();

  
  talk_base::Thread* signaling_thread() { return signaling_thread_; }
  
  talk_base::Thread* worker_thread() { return worker_thread_; }

  
  const std::string& type() const { return type_; }

  
  PortAllocator* port_allocator() { return allocator_; }

  
  
  
  bool readable() const { return readable_; }
  bool writable() const { return writable_; }
  sigslot::signal1<Transport*> SignalReadableState;
  sigslot::signal1<Transport*> SignalWritableState;

  
  bool connect_requested() const { return connect_requested_; }

  
  TransportChannelImpl* CreateChannel(const std::string& name,
                                      const std::string& content_type);
  
  
  TransportChannelImpl* GetChannel(const std::string& name);
  
  bool HasChannel(const std::string& name) {
    return (NULL != GetChannel(name));
  }
  bool HasChannels();
  void DestroyChannel(const std::string& name);

  
  
  void ConnectChannels();
  sigslot::signal1<Transport*> SignalConnecting;

  
  
  void ResetChannels();

  
  void DestroyAllChannels();

  
  
  
  
  
  sigslot::signal1<Transport*> SignalRequestSignaling;
  void OnSignalingReady();

  
  sigslot::signal2<Transport*,
                   const std::vector<Candidate>&> SignalCandidatesReady;
  void OnRemoteCandidates(const std::vector<Candidate>& candidates);

  
  
  virtual bool VerifyCandidate(const Candidate& candidate,
                               ParseError* error);

  
  
  
  
  
  
  
  virtual void OnTransportError(const buzz::XmlElement* error) {}
  sigslot::signal6<Transport*, const buzz::XmlElement*, const buzz::QName&,
                   const std::string&, const std::string&,
                   const buzz::XmlElement*>
      SignalTransportError;

  sigslot::signal2<Transport*, const std::string&> SignalChannelGone;

  
  
  bool allow_local_ips() const { return allow_local_ips_; }
  void set_allow_local_ips(bool value) { allow_local_ips_ = value; }

 protected:
  
  
  virtual TransportChannelImpl* CreateTransportChannel(
      const std::string& name, const std::string &content_type) = 0;
  virtual void DestroyTransportChannel(TransportChannelImpl* channel) = 0;

  
  virtual void OnTransportSignalingReady() {}

 private:
  typedef std::map<std::string, TransportChannelImpl*> ChannelMap;

  
  void OnChannelReadableState(TransportChannel* channel);
  void OnChannelWritableState(TransportChannel* channel);

  
  void OnChannelRequestSignaling();

  
  void OnRemoteCandidate(const Candidate& candidate);
  
  void OnChannelCandidateReady(TransportChannelImpl* channel,
                               const Candidate& candidate);

  
  void OnMessage(talk_base::Message* msg);

  
  
  
  TransportChannelImpl* CreateChannel_w(const std::string& name,
                                        const std::string& content_type);
  void DestroyChannel_w(const std::string& name);
  void ConnectChannels_w();
  void ResetChannels_w();
  void DestroyAllChannels_w();
  void OnRemoteCandidate_w(const Candidate& candidate);
  void OnChannelReadableState_s();
  void OnChannelWritableState_s();
  void OnChannelRequestSignaling_s();
  void OnConnecting_s();

  
  typedef void (TransportChannelImpl::* TransportChannelFunc)();
  void CallChannels_w(TransportChannelFunc func);

  
  bool GetTransportState_s(bool read);

  void OnChannelCandidateReady_s();

  talk_base::Thread* signaling_thread_;
  talk_base::Thread* worker_thread_;
  std::string type_;
  PortAllocator* allocator_;
  bool destroyed_;
  bool readable_;
  bool writable_;
  bool connect_requested_;
  ChannelMap channels_;
  
  
  std::vector<Candidate> ready_candidates_;
  
  talk_base::CriticalSection crit_;
  bool allow_local_ips_;

  DISALLOW_EVIL_CONSTRUCTORS(Transport);
};

}  

#endif  
