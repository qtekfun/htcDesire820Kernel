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

#ifndef TALK_P2P_BASE_RAWTRANSPORTCHANNEL_H_
#define TALK_P2P_BASE_RAWTRANSPORTCHANNEL_H_

#include <string>
#include <vector>
#include "talk/base/messagequeue.h"
#include "talk/p2p/base/transportchannelimpl.h"
#include "talk/p2p/base/rawtransport.h"
#include "talk/p2p/base/candidate.h"

#if defined(FEATURE_ENABLE_PSTN)

namespace talk_base {
class Thread;
}

namespace cricket {

class Port;
class Connection;
class StunPort;
class RelayPort;
class PortAllocator;
class PortAllocatorSession;

class RawTransportChannel : public TransportChannelImpl,
    public talk_base::MessageHandler {
 public:
  RawTransportChannel(const std::string &name,
                      const std::string &content_type,
                      RawTransport* transport,
                      talk_base::Thread *worker_thread,
                      PortAllocator *allocator);
  virtual ~RawTransportChannel();

  
  virtual int SendPacket(const char *data, size_t len);
  virtual int SetOption(talk_base::Socket::Option opt, int value);
  virtual int GetError();

  
  virtual Transport* GetTransport() { return raw_transport_; }

  
  
  
  virtual void Connect();

  
  virtual void Reset();

  
  virtual void OnSignalingReady() {}

  
  
  virtual void OnCandidate(const Candidate& candidate);

  void OnRemoteAddress(const talk_base::SocketAddress& remote_address);


 private:
  RawTransport* raw_transport_;
  talk_base::Thread *worker_thread_;
  PortAllocator* allocator_;
  PortAllocatorSession* allocator_session_;
  StunPort* stun_port_;
  RelayPort* relay_port_;
  Port* port_;
  bool use_relay_;
  talk_base::SocketAddress remote_address_;

  
  void OnPortReady(PortAllocatorSession* session, Port* port);

  
  void OnCandidatesReady(PortAllocatorSession *session,
                         const std::vector<Candidate>& candidates);

  
  
  void SetPort(Port* port);

  
  
  void SetWritable();

  
  void OnReadPacket(Port* port, const char* data, size_t size,
                    const talk_base::SocketAddress& addr);

  
  virtual void OnMessage(talk_base::Message *msg);

  DISALLOW_EVIL_CONSTRUCTORS(RawTransportChannel);
};

}  

#endif  
#endif  
