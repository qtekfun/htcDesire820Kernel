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

#ifndef TALK_P2P_BASE_RELAYSERVER_H_
#define TALK_P2P_BASE_RELAYSERVER_H_

#include <string>
#include <vector>
#include <map>

#include "talk/base/asyncudpsocket.h"
#include "talk/base/socketaddresspair.h"
#include "talk/base/thread.h"
#include "talk/base/time.h"
#include "talk/p2p/base/port.h"
#include "talk/p2p/base/stun.h"

namespace cricket {

class RelayServerBinding;
class RelayServerConnection;

class RelayServer : public talk_base::MessageHandler,
                    public sigslot::has_slots<> {
 public:
  
  explicit RelayServer(talk_base::Thread* thread);
  ~RelayServer();

  talk_base::Thread* thread() { return thread_; }

  
  bool log_bindings() const { return log_bindings_; }
  void set_log_bindings(bool log_bindings) { log_bindings_ = log_bindings; }

  
  
  void AddInternalSocket(talk_base::AsyncPacketSocket* socket);
  void RemoveInternalSocket(talk_base::AsyncPacketSocket* socket);

  
  
  
  void AddExternalSocket(talk_base::AsyncPacketSocket* socket);
  void RemoveExternalSocket(talk_base::AsyncPacketSocket* socket);

  
  
  
  void AddInternalServerSocket(talk_base::AsyncSocket* socket,
                               cricket::ProtocolType proto);

  
  void RemoveInternalServerSocket(talk_base::AsyncSocket* socket);

  
  int GetConnectionCount() const;
  talk_base::SocketAddressPair GetConnection(int connection) const;
  bool HasConnection(const talk_base::SocketAddress& address) const;

 private:
  typedef std::vector<talk_base::AsyncPacketSocket*> SocketList;
  typedef std::map<talk_base::AsyncSocket*,
                   cricket::ProtocolType> ServerSocketMap;
  typedef std::map<std::string, RelayServerBinding*> BindingMap;
  typedef std::map<talk_base::SocketAddressPair,
                   RelayServerConnection*> ConnectionMap;

  talk_base::Thread* thread_;
  bool log_bindings_;
  SocketList internal_sockets_;
  SocketList external_sockets_;
  ServerSocketMap server_sockets_;
  BindingMap bindings_;
  ConnectionMap connections_;

  
  void OnInternalPacket(talk_base::AsyncPacketSocket* socket,
                        const char* bytes, size_t size,
                        const talk_base::SocketAddress& remote_addr);
  void OnExternalPacket(talk_base::AsyncPacketSocket* socket,
                        const char* bytes, size_t size,
                        const talk_base::SocketAddress& remote_addr);

  void OnReadEvent(talk_base::AsyncSocket* socket);

  
  bool HandleStun(const char* bytes, size_t size,
                  const talk_base::SocketAddress& remote_addr,
                  talk_base::AsyncPacketSocket* socket,
                  std::string* username, StunMessage* msg);
  void HandleStunAllocate(const char* bytes, size_t size,
                          const talk_base::SocketAddressPair& ap,
                          talk_base::AsyncPacketSocket* socket);
  void HandleStun(RelayServerConnection* int_conn, const char* bytes,
                  size_t size);
  void HandleStunAllocate(RelayServerConnection* int_conn,
                          const StunMessage& msg);
  void HandleStunSend(RelayServerConnection* int_conn, const StunMessage& msg);

  
  void AddConnection(RelayServerConnection* conn);
  void RemoveConnection(RelayServerConnection* conn);
  void RemoveBinding(RelayServerBinding* binding);

  
  void OnMessage(talk_base::Message *pmsg);

  
  void OnTimeout(RelayServerBinding* binding);

  
  void AcceptConnection(talk_base::AsyncSocket* server_socket);

  friend class RelayServerConnection;
  friend class RelayServerBinding;
};

class RelayServerConnection {
 public:
  RelayServerConnection(RelayServerBinding* binding,
                        const talk_base::SocketAddressPair& addrs,
                        talk_base::AsyncPacketSocket* socket);
  ~RelayServerConnection();

  RelayServerBinding* binding() { return binding_; }
  talk_base::AsyncPacketSocket* socket() { return socket_; }

  
  
  const talk_base::SocketAddressPair& addr_pair() { return addr_pair_; }

  
  
  void Send(const char* data, size_t size);
  void Send(const char* data, size_t size,
            const talk_base::SocketAddress& ext_addr);

  
  void SendStun(const StunMessage& msg);
  void SendStunError(const StunMessage& request, int code, const char* desc);

  
  
  bool locked() const { return locked_; }
  void Lock();
  void Unlock();

  
  
  const talk_base::SocketAddress& default_destination() const {
    return default_dest_;
  }
  void set_default_destination(const talk_base::SocketAddress& addr) {
    default_dest_ = addr;
  }

 private:
  RelayServerBinding* binding_;
  talk_base::SocketAddressPair addr_pair_;
  talk_base::AsyncPacketSocket* socket_;
  bool locked_;
  talk_base::SocketAddress default_dest_;
};

class RelayServerBinding : public talk_base::MessageHandler {
 public:
  RelayServerBinding(
      RelayServer* server, const std::string& username,
      const std::string& password, uint32 lifetime);
  virtual ~RelayServerBinding();

  RelayServer* server() { return server_; }
  uint32 lifetime() { return lifetime_; }
  const std::string& username() { return username_; }
  const std::string& password() { return password_; }
  const std::string& magic_cookie() { return magic_cookie_; }

  
  void AddInternalConnection(RelayServerConnection* conn);
  void AddExternalConnection(RelayServerConnection* conn);

  
  
  void NoteUsed();
  sigslot::signal1<RelayServerBinding*> SignalTimeout;

  
  
  bool HasMagicCookie(const char* bytes, size_t size) const;

  
  
  RelayServerConnection* GetInternalConnection(
      const talk_base::SocketAddress& ext_addr);
  RelayServerConnection* GetExternalConnection(
      const talk_base::SocketAddress& ext_addr);

  
  void OnMessage(talk_base::Message *pmsg);

 private:
  RelayServer* server_;

  std::string username_;
  std::string password_;
  std::string magic_cookie_;

  std::vector<RelayServerConnection*> internal_connections_;
  std::vector<RelayServerConnection*> external_connections_;

  uint32 lifetime_;
  uint32 last_used_;
  
};

}  

#endif  
