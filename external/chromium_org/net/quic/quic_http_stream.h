// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_HTTP_STREAM_H_
#define NET_QUIC_QUIC_HTTP_STREAM_H_

#include <list>

#include "base/memory/weak_ptr.h"
#include "net/base/io_buffer.h"
#include "net/http/http_stream.h"
#include "net/quic/quic_client_session.h"
#include "net/quic/quic_reliable_client_stream.h"

namespace net {

namespace test {
class QuicHttpStreamPeer;
}  

class NET_EXPORT_PRIVATE QuicHttpStream :
      public QuicClientSession::Observer,
      public QuicReliableClientStream::Delegate,
      public HttpStream {
 public:
  explicit QuicHttpStream(const base::WeakPtr<QuicClientSession>& session);

  virtual ~QuicHttpStream();

  
  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               RequestPriority priority,
                               const BoundNetLog& net_log,
                               const CompletionCallback& callback) OVERRIDE;
  virtual int SendRequest(const HttpRequestHeaders& request_headers,
                          HttpResponseInfo* response,
                          const CompletionCallback& callback) OVERRIDE;
  virtual UploadProgress GetUploadProgress() const OVERRIDE;
  virtual int ReadResponseHeaders(const CompletionCallback& callback) OVERRIDE;
  virtual const HttpResponseInfo* GetResponseInfo() const OVERRIDE;
  virtual int ReadResponseBody(IOBuffer* buf,
                               int buf_len,
                               const CompletionCallback& callback) OVERRIDE;
  virtual void Close(bool not_reusable) OVERRIDE;
  virtual HttpStream* RenewStreamForAuth() OVERRIDE;
  virtual bool IsResponseBodyComplete() const OVERRIDE;
  virtual bool CanFindEndOfResponse() const OVERRIDE;
  virtual bool IsConnectionReused() const OVERRIDE;
  virtual void SetConnectionReused() OVERRIDE;
  virtual bool IsConnectionReusable() const OVERRIDE;
  virtual int64 GetTotalReceivedBytes() const OVERRIDE;
  virtual bool GetLoadTimingInfo(
      LoadTimingInfo* load_timing_info) const OVERRIDE;
  virtual void GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) OVERRIDE;
  virtual bool IsSpdyHttpStream() const OVERRIDE;
  virtual void Drain(HttpNetworkSession* session) OVERRIDE;
  virtual void SetPriority(RequestPriority priority) OVERRIDE;

  
  virtual int OnDataReceived(const char* data, int length) OVERRIDE;
  virtual void OnClose(QuicErrorCode error) OVERRIDE;
  virtual void OnError(int error) OVERRIDE;
  virtual bool HasSendHeadersComplete() OVERRIDE;

  
  virtual void OnCryptoHandshakeConfirmed() OVERRIDE;
  virtual void OnSessionClosed(int error) OVERRIDE;

 private:
  friend class test::QuicHttpStreamPeer;

  enum State {
    STATE_NONE,
    STATE_SEND_HEADERS,
    STATE_SEND_HEADERS_COMPLETE,
    STATE_READ_REQUEST_BODY,
    STATE_READ_REQUEST_BODY_COMPLETE,
    STATE_SEND_BODY,
    STATE_SEND_BODY_COMPLETE,
    STATE_OPEN,
  };

  void OnStreamReady(int rv);
  void OnIOComplete(int rv);
  void DoCallback(int rv);

  int DoLoop(int);
  int DoSendHeaders();
  int DoSendHeadersComplete(int rv);
  int DoReadRequestBody();
  int DoReadRequestBodyComplete(int rv);
  int DoSendBody();
  int DoSendBodyComplete(int rv);
  int DoReadResponseHeaders();
  int DoReadResponseHeadersComplete(int rv);

  int ParseResponseHeaders();

  void BufferResponseBody(const char* data, int length);

  State next_state_;

  base::WeakPtr<QuicClientSession> session_;
  int session_error_;  
  bool was_handshake_confirmed_;  
  QuicClientSession::StreamRequest stream_request_;
  QuicReliableClientStream* stream_;  

  
  

  
  const HttpRequestInfo* request_info_;
  
  UploadDataStream* request_body_stream_;
  
  RequestPriority priority_;
  
  
  HttpResponseInfo* response_info_;
  
  
  
  
  int response_status_;

  
  SpdyHeaderBlock request_headers_;

  bool response_headers_received_;

  
  std::string request_;

  
  scoped_refptr<GrowableIOBuffer> read_buf_;

  
  
  std::list<scoped_refptr<IOBufferWithSize> > response_body_;

  
  CompletionCallback callback_;

  
  scoped_refptr<IOBuffer> user_buffer_;
  int user_buffer_len_;

  
  scoped_refptr<IOBufferWithSize> raw_request_body_buf_;
  
  scoped_refptr<DrainableIOBuffer> request_body_buf_;

  BoundNetLog stream_net_log_;

  base::WeakPtrFactory<QuicHttpStream> weak_factory_;
};

}  

#endif  
