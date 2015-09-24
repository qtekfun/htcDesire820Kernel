// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_SERVER_TCP_SERVER_SOCKET_EVENT_DISPATCHER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_SERVER_TCP_SERVER_SOCKET_EVENT_DISPATCHER_H_

#include "chrome/browser/extensions/api/api_resource_manager.h"
#include "chrome/browser/extensions/api/sockets_tcp/sockets_tcp_api.h"
#include "chrome/browser/extensions/api/sockets_tcp_server/sockets_tcp_server_api.h"

namespace extensions {
struct Event;
class ResumableTCPSocket;
}

namespace extensions {
namespace api {

class TCPServerSocketEventDispatcher
    : public ProfileKeyedAPI,
      public base::SupportsWeakPtr<TCPServerSocketEventDispatcher> {
 public:
  explicit TCPServerSocketEventDispatcher(Profile* profile);
  virtual ~TCPServerSocketEventDispatcher();

  
  void OnServerSocketListen(const std::string& extension_id, int socket_id);

  
  void OnServerSocketResume(const std::string& extension_id, int socket_id);

  
  static ProfileKeyedAPIFactory<TCPServerSocketEventDispatcher>*
      GetFactoryInstance();

  
  static TCPServerSocketEventDispatcher* Get(Profile* profile);

 private:
  typedef ApiResourceManager<ResumableTCPServerSocket>::ApiResourceData
      ServerSocketData;
  typedef ApiResourceManager<ResumableTCPSocket>::ApiResourceData
      ClientSocketData;
  friend class ProfileKeyedAPIFactory<TCPServerSocketEventDispatcher>;
  
  static const char* service_name() {
    return "TCPServerSocketEventDispatcher";
  }
  static const bool kServiceHasOwnInstanceInIncognito = true;
  static const bool kServiceIsNULLWhileTesting = true;

  
  
  struct AcceptParams {
    AcceptParams();
    ~AcceptParams();

    content::BrowserThread::ID thread_id;
    void* profile_id;
    std::string extension_id;
    scoped_refptr<ServerSocketData> server_sockets;
    scoped_refptr<ClientSocketData> client_sockets;
    int socket_id;
  };

  
  void StartSocketAccept(const std::string& extension_id, int socket_id);

  
  static void StartAccept(const AcceptParams& params);

  
  static void AcceptCallback(const AcceptParams& params,
                             int result_code,
                             net::TCPClientSocket *socket);

  
  static void PostEvent(const AcceptParams& params,
                        scoped_ptr<Event> event);

  
  static void DispatchEvent(void* profile_id,
                            const std::string& extension_id,
                            scoped_ptr<Event> event);

  
  content::BrowserThread::ID thread_id_;
  Profile* const profile_;
  scoped_refptr<ServerSocketData> server_sockets_;
  scoped_refptr<ClientSocketData> client_sockets_;
};

}  
}  

#endif  
