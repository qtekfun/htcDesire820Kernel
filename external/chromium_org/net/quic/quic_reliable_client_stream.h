// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_RELIABLE_CLIENT_STREAM_H_
#define NET_QUIC_QUIC_RELIABLE_CLIENT_STREAM_H_

#include "net/base/ip_endpoint.h"
#include "net/base/upload_data_stream.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/http/http_stream.h"
#include "net/quic/quic_data_stream.h"

namespace net {

class QuicClientSession;

class NET_EXPORT_PRIVATE QuicReliableClientStream : public QuicDataStream {
 public:
  
  class NET_EXPORT_PRIVATE Delegate {
   public:
    Delegate() {}

    
    
    virtual int OnDataReceived(const char* data, int length) = 0;

    
    virtual void OnClose(QuicErrorCode error) = 0;

    
    virtual void OnError(int error) = 0;

    
    virtual bool HasSendHeadersComplete() = 0;

   protected:
    virtual ~Delegate() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  QuicReliableClientStream(QuicStreamId id,
                           QuicSession* session,
                           const BoundNetLog& net_log);

  virtual ~QuicReliableClientStream();

  
  virtual uint32 ProcessData(const char* data, uint32 data_len) OVERRIDE;
  virtual void OnFinRead() OVERRIDE;
  virtual void OnCanWrite() OVERRIDE;
  virtual QuicPriority EffectivePriority() const OVERRIDE;

  
  
  using QuicDataStream::set_priority;

  int WriteStreamData(base::StringPiece data,
                      bool fin,
                      const CompletionCallback& callback);
  
  
  
  void SetDelegate(Delegate* delegate);
  Delegate* GetDelegate() { return delegate_; }
  void OnError(int error);

  
  
  
  
  bool CanWrite(const CompletionCallback& callback);

  const BoundNetLog& net_log() const { return net_log_; }

  using QuicDataStream::HasBufferedData;

 private:
  BoundNetLog net_log_;
  Delegate* delegate_;

  CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(QuicReliableClientStream);
};

}  

#endif  
