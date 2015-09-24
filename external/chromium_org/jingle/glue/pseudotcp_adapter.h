// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_PSEUDOTCP_ADAPTER_H_
#define JINGLE_GLUE_PSEUDOTCP_ADAPTER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/timer/timer.h"
#include "net/base/net_log.h"
#include "net/socket/stream_socket.h"
#include "third_party/libjingle/source/talk/p2p/base/pseudotcp.h"

namespace jingle_glue {

class PseudoTcpAdapter : public net::StreamSocket, base::NonThreadSafe {
 public:
  
  
  PseudoTcpAdapter(net::Socket* socket);
  virtual ~PseudoTcpAdapter();

  
  virtual int Read(net::IOBuffer* buffer, int buffer_size,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buffer, int buffer_size,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

  
  virtual int Connect(const net::CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual const net::BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual net::NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(net::SSLInfo* ssl_info) OVERRIDE;

  
  void SetAckDelay(int delay_ms);

  
  void SetNoDelay(bool no_delay);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void SetWriteWaitsForSend(bool write_waits_for_send);

 private:
  class Core;

  scoped_refptr<Core> core_;

  net::BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(PseudoTcpAdapter);
};

}  

#endif  
