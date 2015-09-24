// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_CHROME_ASYNC_SOCKET_H_
#define JINGLE_GLUE_CHROME_ASYNC_SOCKET_H_

#if !defined(FEATURE_ENABLE_SSL)
#error ChromeAsyncSocket expects FEATURE_ENABLE_SSL to be defined
#endif

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"
#include "third_party/libjingle/source/talk/xmpp/asyncsocket.h"

namespace net {
class IOBufferWithSize;
class StreamSocket;
}  

namespace jingle_glue {

class ResolvingClientSocketFactory;

class ChromeAsyncSocket : public buzz::AsyncSocket {
 public:
  
  ChromeAsyncSocket(
      ResolvingClientSocketFactory* resolving_client_socket_factory,
      size_t read_buf_size,
      size_t write_buf_size);

  
  virtual ~ChromeAsyncSocket();

  

  
  
  virtual State state() OVERRIDE;

  
  
  
  virtual Error error() OVERRIDE;

  
  
  virtual int GetError() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool Connect(const talk_base::SocketAddress& address) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  virtual bool Read(char* data, size_t len, size_t* len_read) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool Write(const char* data, size_t len) OVERRIDE;

  
  
  virtual bool Close() OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  virtual bool StartTls(const std::string& domain_name) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

 private:
  enum AsyncIOState {
    
    IDLE,
    
    POSTED,
    
    PENDING,
  };

  bool IsOpen() const;

  
  void DoNonNetError(Error error);
  void DoNetError(net::Error net_error);
  void DoNetErrorFromStatus(int status);

  
  void ProcessConnectDone(int status);

  
  void PostDoRead();
  void DoRead();
  void ProcessReadDone(int status);

  
  void PostDoWrite();
  void DoWrite();
  void ProcessWriteDone(int status);

  
  void ProcessSSLConnectDone(int status);

  
  void DoClose();

  scoped_ptr<ResolvingClientSocketFactory> resolving_client_socket_factory_;

  
  buzz::AsyncSocket::State state_;
  buzz::AsyncSocket::Error error_;
  net::Error net_error_;

  
  scoped_ptr<net::StreamSocket> transport_socket_;

  
  
  
  AsyncIOState read_state_;
  scoped_refptr<net::IOBufferWithSize> read_buf_;
  size_t read_start_, read_end_;

  
  
  
  AsyncIOState write_state_;
  scoped_refptr<net::IOBufferWithSize> write_buf_;
  size_t write_end_;

  base::WeakPtrFactory<ChromeAsyncSocket> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromeAsyncSocket);
};

}  

#endif  
