// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_HTTP_STREAM_H_
#define NET_SPDY_SPDY_HTTP_STREAM_H_

#include <list>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/http/http_stream.h"
#include "net/spdy/spdy_read_queue.h"
#include "net/spdy/spdy_session.h"
#include "net/spdy/spdy_stream.h"

namespace net {

class DrainableIOBuffer;
struct HttpRequestInfo;
class HttpResponseInfo;
class IOBuffer;
class SpdySession;
class UploadDataStream;

class NET_EXPORT_PRIVATE SpdyHttpStream : public SpdyStream::Delegate,
                                          public HttpStream {
 public:
  
  SpdyHttpStream(const base::WeakPtr<SpdySession>& spdy_session, bool direct);
  virtual ~SpdyHttpStream();

  SpdyStream* stream() { return stream_.get(); }

  
  void Cancel();

  

  virtual int InitializeStream(const HttpRequestInfo* request_info,
                               RequestPriority priority,
                               const BoundNetLog& net_log,
                               const CompletionCallback& callback) OVERRIDE;

  virtual int SendRequest(const HttpRequestHeaders& headers,
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

  
  virtual void OnRequestHeadersSent() OVERRIDE;
  virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
      const SpdyHeaderBlock& response_headers) OVERRIDE;
  virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) OVERRIDE;
  virtual void OnDataSent() OVERRIDE;
  virtual void OnClose(int status) OVERRIDE;

 private:
  
  bool HasUploadData() const;

  void OnStreamCreated(const CompletionCallback& callback, int rv);

  
  
  
  
  void ReadAndSendRequestBodyData();

  
  
  void OnRequestBodyReadCompleted(int status);

  
  void DoCallback(int rv);

  void ScheduleBufferedReadCallback();

  
  bool DoBufferedReadCallback();
  bool ShouldWaitForMoreBufferedData() const;

  base::WeakPtrFactory<SpdyHttpStream> weak_factory_;

  const base::WeakPtr<SpdySession> spdy_session_;
  bool is_reused_;
  SpdyStreamRequest stream_request_;
  base::WeakPtr<SpdyStream> stream_;

  bool stream_closed_;

  
  int closed_stream_status_;
  SpdyStreamId closed_stream_id_;
  bool closed_stream_has_load_timing_info_;
  LoadTimingInfo closed_stream_load_timing_info_;
  int64 closed_stream_received_bytes_;

  
  const HttpRequestInfo* request_info_;

  
  
  
  HttpResponseInfo* response_info_;

  scoped_ptr<HttpResponseInfo> push_response_info_;

  
  
  SpdyResponseHeadersStatus response_headers_status_;

  
  SpdyReadQueue response_body_queue_;

  CompletionCallback callback_;

  
  scoped_refptr<IOBuffer> user_buffer_;
  int user_buffer_len_;

  
  scoped_refptr<IOBufferWithSize> request_body_buf_;
  int request_body_buf_size_;

  
  bool buffered_read_callback_pending_;
  
  
  bool more_read_data_pending_;

  
  bool direct_;

  DISALLOW_COPY_AND_ASSIGN(SpdyHttpStream);
};

}  

#endif  
