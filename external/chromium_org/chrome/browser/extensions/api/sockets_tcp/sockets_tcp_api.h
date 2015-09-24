// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_SOCKETS_TCP_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_SOCKETS_TCP_SOCKETS_TCP_API_H_

#include "chrome/browser/extensions/api/socket/socket_api.h"
#include "chrome/common/extensions/api/sockets_tcp.h"

namespace extensions {
class ResumableTCPSocket;
}

namespace extensions {
namespace api {

class TCPSocketAsyncApiFunction : public SocketAsyncApiFunction {
 protected:
  virtual ~TCPSocketAsyncApiFunction();

  virtual scoped_ptr<SocketResourceManagerInterface>
      CreateSocketResourceManager() OVERRIDE;

  ResumableTCPSocket* GetTcpSocket(int socket_id);
};

class TCPSocketExtensionWithDnsLookupFunction
    : public SocketExtensionWithDnsLookupFunction {
 protected:
  virtual ~TCPSocketExtensionWithDnsLookupFunction();

  virtual scoped_ptr<SocketResourceManagerInterface>
      CreateSocketResourceManager() OVERRIDE;

  ResumableTCPSocket* GetTcpSocket(int socket_id);
};

class SocketsTcpCreateFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.create", SOCKETS_TCP_CREATE)

  SocketsTcpCreateFunction();

 protected:
  virtual ~SocketsTcpCreateFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(SocketsTcpUnitTest, Create);
  scoped_ptr<sockets_tcp::Create::Params> params_;
};

class SocketsTcpUpdateFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.update", SOCKETS_TCP_UPDATE)

  SocketsTcpUpdateFunction();

 protected:
  virtual ~SocketsTcpUpdateFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::Update::Params> params_;
};

class SocketsTcpSetPausedFunction
    : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.setPaused", SOCKETS_TCP_SETPAUSED)

  SocketsTcpSetPausedFunction();

 protected:
  virtual ~SocketsTcpSetPausedFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::SetPaused::Params> params_;
  TCPSocketEventDispatcher* socket_event_dispatcher_;
};

class SocketsTcpSetKeepAliveFunction
    : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.setKeepAlive",
                             SOCKETS_TCP_SETKEEPALIVE)

  SocketsTcpSetKeepAliveFunction();

 protected:
  virtual ~SocketsTcpSetKeepAliveFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::SetKeepAlive::Params> params_;
};

class SocketsTcpSetNoDelayFunction
    : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.setNoDelay", SOCKETS_TCP_SETNODELAY)

  SocketsTcpSetNoDelayFunction();

 protected:
  virtual ~SocketsTcpSetNoDelayFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::SetNoDelay::Params> params_;
};

class SocketsTcpConnectFunction
    : public TCPSocketExtensionWithDnsLookupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.connect", SOCKETS_TCP_CONNECT)

  SocketsTcpConnectFunction();

 protected:
  virtual ~SocketsTcpConnectFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void AsyncWorkStart() OVERRIDE;

  
  virtual void AfterDnsLookup(int lookup_result) OVERRIDE;

 private:
  void StartConnect();
  void OnCompleted(int net_result);

  scoped_ptr<sockets_tcp::Connect::Params> params_;
  TCPSocketEventDispatcher* socket_event_dispatcher_;
};

class SocketsTcpDisconnectFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.disconnect", SOCKETS_TCP_DISCONNECT)

  SocketsTcpDisconnectFunction();

 protected:
  virtual ~SocketsTcpDisconnectFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::Disconnect::Params> params_;
};

class SocketsTcpSendFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.send", SOCKETS_TCP_SEND)

  SocketsTcpSendFunction();

 protected:
  virtual ~SocketsTcpSendFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void AsyncWorkStart() OVERRIDE;

 private:
  void OnCompleted(int net_result);
  void SetSendResult(int net_result, int bytes_sent);

  scoped_ptr<sockets_tcp::Send::Params> params_;
  scoped_refptr<net::IOBuffer> io_buffer_;
  size_t io_buffer_size_;
};

class SocketsTcpCloseFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.close", SOCKETS_TCP_CLOSE)

  SocketsTcpCloseFunction();

 protected:
  virtual ~SocketsTcpCloseFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::Close::Params> params_;
};

class SocketsTcpGetInfoFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.getInfo", SOCKETS_TCP_GETINFO)

  SocketsTcpGetInfoFunction();

 protected:
  virtual ~SocketsTcpGetInfoFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;

 private:
  scoped_ptr<sockets_tcp::GetInfo::Params> params_;
};

class SocketsTcpGetSocketsFunction : public TCPSocketAsyncApiFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("sockets.tcp.getSockets", SOCKETS_TCP_GETSOCKETS)

  SocketsTcpGetSocketsFunction();

 protected:
  virtual ~SocketsTcpGetSocketsFunction();

  
  virtual bool Prepare() OVERRIDE;
  virtual void Work() OVERRIDE;
};

}  
}  

#endif  
