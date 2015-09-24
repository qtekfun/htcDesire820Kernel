// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_TCP_SOCKET_EVENT_DISPATCHER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_TCP_SOCKET_EVENT_DISPATCHER_H_

#include "chrome/browser/extensions/api/api_resource_manager.h"
#include "chrome/browser/extensions/api/sockets_tcp/sockets_tcp_api.h"

namespace extensions {
struct Event;
class ResumableTCPSocket;
}

namespace extensions {
namespace api {

class TCPSocketEventDispatcher
    : public ProfileKeyedAPI,
      public base::SupportsWeakPtr<TCPSocketEventDispatcher> {
 public:
  explicit TCPSocketEventDispatcher(Profile* profile);
  virtual ~TCPSocketEventDispatcher();

  
  void OnSocketConnect(const std::string& extension_id, int socket_id);

  
  void OnSocketResume(const std::string& extension_id, int socket_id);

  
  static ProfileKeyedAPIFactory<TCPSocketEventDispatcher>* GetFactoryInstance();

  
  static TCPSocketEventDispatcher* Get(Profile* profile);

 private:
  typedef ApiResourceManager<ResumableTCPSocket>::ApiResourceData SocketData;
  friend class ProfileKeyedAPIFactory<TCPSocketEventDispatcher>;
  
  static const char* service_name() {
    return "TCPSocketEventDispatcher";
  }
  static const bool kServiceHasOwnInstanceInIncognito = true;
  static const bool kServiceIsNULLWhileTesting = true;

  
  
  struct ReadParams {
    ReadParams();
    ~ReadParams();

    content::BrowserThread::ID thread_id;
    void* profile_id;
    std::string extension_id;
    scoped_refptr<SocketData> sockets;
    int socket_id;
  };

  
  void StartSocketRead(const std::string& extension_id, int socket_id);

  
  static void StartRead(const ReadParams& params);

  
  static void ReadCallback(const ReadParams& params,
                           int bytes_read,
                           scoped_refptr<net::IOBuffer> io_buffer);

  
  static void PostEvent(const ReadParams& params, scoped_ptr<Event> event);

  
  static void DispatchEvent(void* profile_id,
                            const std::string& extension_id,
                            scoped_ptr<Event> event);

  
  content::BrowserThread::ID thread_id_;
  Profile* const profile_;
  scoped_refptr<SocketData> sockets_;
};

}  
}  

#endif  
