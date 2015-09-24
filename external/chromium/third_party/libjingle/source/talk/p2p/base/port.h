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

#ifndef TALK_P2P_BASE_PORT_H_
#define TALK_P2P_BASE_PORT_H_

#include <string>
#include <vector>
#include <map>

#include "talk/base/network.h"
#include "talk/base/packetsocketfactory.h"
#include "talk/base/proxyinfo.h"
#include "talk/base/ratetracker.h"
#include "talk/base/sigslot.h"
#include "talk/base/socketaddress.h"
#include "talk/base/thread.h"
#include "talk/p2p/base/candidate.h"
#include "talk/p2p/base/stun.h"
#include "talk/p2p/base/stunrequest.h"

namespace talk_base {
class AsyncPacketSocket;
}

namespace cricket {

class Connection;
class ConnectionRequest;

enum ProtocolType {
  PROTO_UDP,
  PROTO_TCP,
  PROTO_SSLTCP,
  PROTO_LAST = PROTO_SSLTCP
};

const char* ProtoToString(ProtocolType proto);
bool StringToProto(const char* value, ProtocolType* proto);

struct ProtocolAddress {
  talk_base::SocketAddress address;
  ProtocolType proto;

  ProtocolAddress(const talk_base::SocketAddress& a, ProtocolType p)
    : address(a), proto(p) { }
};

class Port : public talk_base::MessageHandler, public sigslot::has_slots<> {
 public:
  Port(talk_base::Thread* thread, const std::string& type,
       talk_base::PacketSocketFactory* factory, talk_base::Network* network,
       uint32 ip, int min_port, int max_port);
  virtual ~Port();

  
  talk_base::Thread* thread() { return thread_; }

  
  talk_base::PacketSocketFactory* socket_factory() const { return factory_; }
  void set_socket_factory(talk_base::PacketSocketFactory* factory) {
    factory_ = factory;
  }

  
  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }

  
  
  
  
  
  
  const std::string& username_fragment() const { return username_frag_; }
  void set_username_fragment(const std::string& username) {
    username_frag_ = username;
  }

  const std::string& password() const { return password_; }
  void set_password(const std::string& password) { password_ = password; }


  
  
  float preference() const { return preference_; }
  void set_preference(float preference) { preference_ = preference; }

  
  const std::string& type() const { return type_; }

  
  talk_base::Network* network() { return network_; }

  
  uint32 generation() { return generation_; }
  void set_generation(uint32 generation) { generation_ = generation; }

  
  
  
  
  
  virtual void PrepareAddress() = 0;
  sigslot::signal1<Port*> SignalAddressReady;
  sigslot::signal1<Port*> SignalAddressError;

  
  const std::vector<Candidate>& candidates() const { return candidates_; }

  
  
  typedef std::map<talk_base::SocketAddress, Connection*> AddressMap;
  const AddressMap& connections() { return connections_; }

  
  Connection* GetConnection(const talk_base::SocketAddress& remote_addr);

  
  enum CandidateOrigin { ORIGIN_THIS_PORT, ORIGIN_OTHER_PORT, ORIGIN_MESSAGE };
  virtual Connection* CreateConnection(const Candidate& remote_candidate,
    CandidateOrigin origin) = 0;

  
  sigslot::signal2<Port*, Connection*> SignalConnectionCreated;

  
  
  virtual int SendTo(
      const void* data, size_t size, const talk_base::SocketAddress& addr,
      bool payload) = 0;

  
  
  
  sigslot::signal4<Port*, const talk_base::SocketAddress&, StunMessage*,
                   const std::string&> SignalUnknownAddress;

  
  
  
  void SendBindingResponse(StunMessage* request,
                           const talk_base::SocketAddress& addr);
  void SendBindingErrorResponse(
      StunMessage* request, const talk_base::SocketAddress& addr,
      int error_code, const std::string& reason);

  
  sigslot::signal2<Port*, int> SignalReadError;
  sigslot::signal2<Port*, int> SignalWriteError;

  
  virtual int SetOption(talk_base::Socket::Option opt, int value) = 0;
  virtual int GetError() = 0;

  void set_proxy(const std::string& user_agent,
                 const talk_base::ProxyInfo& proxy) {
    user_agent_ = user_agent;
    proxy_ = proxy;
  }
  const std::string& user_agent() { return user_agent_; }
  const talk_base::ProxyInfo& proxy() { return proxy_; }

  
  
  
  
  void EnablePortPackets();
  sigslot::signal4<Port*, const char*, size_t, const talk_base::SocketAddress&>
      SignalReadPacket;

  
  
  void Start();

  
  void Destroy();

  
  
  sigslot::signal1<Port*> SignalDestroyed;

  virtual void OnMessage(talk_base::Message *pmsg);

  
  std::string ToString() const;

 protected:
  
  void AddAddress(const talk_base::SocketAddress& address,
                  const std::string& protocol, bool final);

  
  void AddConnection(Connection* conn);

  
  
  
  void OnReadPacket(const char* data, size_t size,
                    const talk_base::SocketAddress& addr);


  
  
  
  
  
  bool GetStunMessage(const char* data, size_t size,
                      const talk_base::SocketAddress& addr,
                      StunMessage** out_msg, std::string* out_username);

  
  talk_base::Thread* thread_;
  talk_base::PacketSocketFactory* factory_;
  std::string type_;
  talk_base::Network* network_;
  uint32 ip_;
  int min_port_;
  int max_port_;
  uint32 generation_;
  std::string name_;
  std::string username_frag_;
  std::string password_;
  float preference_;
  std::vector<Candidate> candidates_;
  AddressMap connections_;
  enum Lifetime { LT_PRESTART, LT_PRETIMEOUT, LT_POSTTIMEOUT } lifetime_;
  bool enable_port_packets_;

 private:
  
  void OnConnectionDestroyed(Connection* conn);

  
  void CheckTimeout();

  
  std::string user_agent_;
  talk_base::ProxyInfo proxy_;

  friend class Connection;
};

class Connection : public talk_base::MessageHandler,
    public sigslot::has_slots<> {
 public:
  virtual ~Connection();

  
  Port* port() { return port_; }
  const Port* port() const { return port_; }

  
  virtual const Candidate& local_candidate() const;

  
  const Candidate& remote_candidate() const { return remote_candidate_; }

  enum ReadState {
    STATE_READABLE     = 0,  
    STATE_READ_TIMEOUT = 1   
  };

  ReadState read_state() const { return read_state_; }

  enum WriteState {
    STATE_WRITABLE      = 0,  
    STATE_WRITE_CONNECT = 1,  
    STATE_WRITE_TIMEOUT = 2   
  };

  WriteState write_state() const { return write_state_; }

  
  
  bool connected() const { return connected_; }

  
  uint32 rtt() const { return rtt_; }

  size_t sent_total_bytes();
  size_t sent_bytes_second();
  size_t recv_total_bytes();
  size_t recv_bytes_second();
  sigslot::signal1<Connection*> SignalStateChange;

  
  
  sigslot::signal1<Connection*> SignalDestroyed;

  
  
  
  virtual int Send(const void* data, size_t size) = 0;

  
  virtual int GetError() = 0;

  sigslot::signal3<Connection*, const char*, size_t> SignalReadPacket;

  
  void OnReadPacket(const char* data, size_t size);

  
  
  
  
  bool pruned() const { return pruned_; }
  void Prune();

  
  void Destroy();

  
  
  void UpdateState(uint32 now);

  
  uint32 last_ping_sent() const { return last_ping_sent_; }
  void Ping(uint32 now);

  
  
  void ReceivedPing();

  
  std::string ToString() const;

  bool reported() const { return reported_; }
  void set_reported(bool reported) { reported_ = reported;}

 protected:
  
  Connection(Port* port, size_t index, const Candidate& candidate);

  
  void OnSendStunPacket(const void* data, size_t size, StunRequest* req);

  
  void OnConnectionRequestResponse(ConnectionRequest* req,
                                   StunMessage* response);
  void OnConnectionRequestErrorResponse(ConnectionRequest* req,
                                        StunMessage* response);
  void OnConnectionRequestTimeout(ConnectionRequest* req);

  
  void set_read_state(ReadState value);
  void set_write_state(WriteState value);
  void set_connected(bool value);

  
  void CheckTimeout();

  void OnMessage(talk_base::Message *pmsg);

  Port* port_;
  size_t local_candidate_index_;
  Candidate remote_candidate_;
  ReadState read_state_;
  WriteState write_state_;
  bool connected_;
  bool pruned_;
  StunRequestManager requests_;
  uint32 rtt_;
  uint32 last_ping_sent_;      
  uint32 last_ping_received_;  
                               
  uint32 last_data_received_;
  std::vector<uint32> pings_since_last_response_;

  talk_base::RateTracker recv_rate_tracker_;
  talk_base::RateTracker send_rate_tracker_;

 private:
  bool reported_;

  friend class Port;
  friend class ConnectionRequest;
};

class ProxyConnection : public Connection {
 public:
  ProxyConnection(Port* port, size_t index, const Candidate& candidate);

  virtual int Send(const void* data, size_t size);
  virtual int GetError() { return error_; }

 private:
  int error_;
};

}  

#endif  
