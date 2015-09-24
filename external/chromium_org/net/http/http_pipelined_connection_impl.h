// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_CONNECTION_IMPL_H_
#define NET_HTTP_HTTP_PIPELINED_CONNECTION_IMPL_H_

#include <map>
#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/location.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/http/http_pipelined_connection.h"
#include "net/http/http_request_info.h"
#include "net/http/http_stream_parser.h"
#include "net/proxy/proxy_info.h"
#include "net/ssl/ssl_config_service.h"

namespace net {

class ClientSocketHandle;
class GrowableIOBuffer;
class HostPortPair;
class HttpNetworkSession;
class HttpRequestHeaders;
class HttpResponseInfo;
class IOBuffer;
struct LoadTimingInfo;
class SSLCertRequestInfo;
class SSLInfo;

class NET_EXPORT_PRIVATE HttpPipelinedConnectionImpl
    : public HttpPipelinedConnection {
 public:
  class Factory : public HttpPipelinedConnection::Factory {
   public:
    virtual HttpPipelinedConnection* CreateNewPipeline(
        ClientSocketHandle* connection,
        HttpPipelinedConnection::Delegate* delegate,
        const HostPortPair& origin,
        const SSLConfig& used_ssl_config,
        const ProxyInfo& used_proxy_info,
        const BoundNetLog& net_log,
        bool was_npn_negotiated,
        NextProto protocol_negotiated) OVERRIDE;
  };

  HttpPipelinedConnectionImpl(ClientSocketHandle* connection,
                              Delegate* delegate,
                              const HostPortPair& origin,
                              const SSLConfig& used_ssl_config,
                              const ProxyInfo& used_proxy_info,
                              const BoundNetLog& net_log,
                              bool was_npn_negotiated,
                              NextProto protocol_negotiated);
  virtual ~HttpPipelinedConnectionImpl();

  

  
  virtual HttpPipelinedStream* CreateNewStream() OVERRIDE;

  
  virtual int depth() const OVERRIDE;
  virtual bool usable() const OVERRIDE;
  virtual bool active() const OVERRIDE;

  
  virtual const SSLConfig& used_ssl_config() const OVERRIDE;
  virtual const ProxyInfo& used_proxy_info() const OVERRIDE;
  virtual const BoundNetLog& net_log() const OVERRIDE;
  virtual bool was_npn_negotiated() const OVERRIDE;
  virtual NextProto protocol_negotiated() const OVERRIDE;

  

  
  void OnStreamDeleted(int pipeline_id);

  
  
  
  void InitializeParser(int pipeline_id,
                        const HttpRequestInfo* request,
                        const BoundNetLog& net_log);

  int SendRequest(int pipeline_id,
                  const std::string& request_line,
                  const HttpRequestHeaders& headers,
                  HttpResponseInfo* response,
                  const CompletionCallback& callback);

  int ReadResponseHeaders(int pipeline_id,
                          const CompletionCallback& callback);

  int ReadResponseBody(int pipeline_id,
                       IOBuffer* buf, int buf_len,
                       const CompletionCallback& callback);

  void Close(int pipeline_id,
             bool not_reusable);

  UploadProgress GetUploadProgress(int pipeline_id) const;

  HttpResponseInfo* GetResponseInfo(int pipeline_id);

  bool IsResponseBodyComplete(int pipeline_id) const;

  bool CanFindEndOfResponse(int pipeline_id) const;

  bool IsConnectionReused(int pipeline_id) const;

  void SetConnectionReused(int pipeline_id);

  int64 GetTotalReceivedBytes(int pipeline_id) const;

  bool GetLoadTimingInfo(int pipeline_id,
                         LoadTimingInfo* load_timing_info) const;

  void GetSSLInfo(int pipeline_id, SSLInfo* ssl_info);

  void GetSSLCertRequestInfo(int pipeline_id,
                             SSLCertRequestInfo* cert_request_info);

  
  
  void Drain(HttpPipelinedStream* stream, HttpNetworkSession* session);

 private:
  enum StreamState {
    STREAM_CREATED,
    STREAM_BOUND,
    STREAM_SENDING,
    STREAM_SENT,
    STREAM_READ_PENDING,
    STREAM_ACTIVE,
    STREAM_CLOSED,
    STREAM_READ_EVICTED,
    STREAM_UNUSED,
  };
  enum SendRequestState {
    SEND_STATE_START_IMMEDIATELY,
    SEND_STATE_START_NEXT_DEFERRED_REQUEST,
    SEND_STATE_SEND_ACTIVE_REQUEST,
    SEND_STATE_COMPLETE,
    SEND_STATE_EVICT_PENDING_REQUESTS,
    SEND_STATE_NONE,
  };
  enum ReadHeadersState {
    READ_STATE_START_IMMEDIATELY,
    READ_STATE_START_NEXT_DEFERRED_READ,
    READ_STATE_READ_HEADERS,
    READ_STATE_READ_HEADERS_COMPLETE,
    READ_STATE_WAITING_FOR_CLOSE,
    READ_STATE_STREAM_CLOSED,
    READ_STATE_NONE,
    READ_STATE_EVICT_PENDING_READS,
  };

  struct PendingSendRequest {
    PendingSendRequest();
    ~PendingSendRequest();

    int pipeline_id;
    std::string request_line;
    HttpRequestHeaders headers;
    HttpResponseInfo* response;
    CompletionCallback callback;
  };

  struct StreamInfo {
    StreamInfo();
    ~StreamInfo();

    linked_ptr<HttpStreamParser> parser;
    CompletionCallback read_headers_callback;
    CompletionCallback pending_user_callback;
    StreamState state;
    NetLog::Source source;
  };

  typedef std::map<int, StreamInfo> StreamInfoMap;

  
  
  
  
  
  void ActivatePipeline();

  
  
  int DoSendRequestLoop(int result);

  
  void OnSendIOCallback(int result);

  
  
  int DoStartRequestImmediately(int result);

  
  
  
  int DoStartNextDeferredRequest(int result);

  
  int DoSendActiveRequest(int result);

  
  
  
  int DoSendComplete(int result);

  
  
  
  int DoEvictPendingSendRequests(int result);

  
  
  
  int DoReadHeadersLoop(int result);

  
  void OnReadIOCallback(int result);

  
  
  
  void StartNextDeferredRead();

  
  
  
  int DoStartReadImmediately(int result);

  
  
  int DoStartNextDeferredRead(int result);

  
  int DoReadHeaders(int result);

  
  
  
  int DoReadHeadersComplete(int result);

  
  int DoReadWaitForClose(int result);

  
  
  
  int DoReadStreamClosed();

  
  
  
  int DoEvictPendingReadHeaders(int result);

  
  
  void CheckHeadersForPipelineCompatibility(int pipeline_id, int result);

  
  void ReportPipelineFeedback(int pipeline_id, Feedback feedback);

  
  
  
  
  
  
  void QueueUserCallback(int pipeline_id,
                         const CompletionCallback& callback,
                         int rv,
                         const tracked_objects::Location& from_here);

  
  void FireUserCallback(int pipeline_id, int result);

  Delegate* delegate_;
  scoped_ptr<ClientSocketHandle> connection_;
  SSLConfig used_ssl_config_;
  ProxyInfo used_proxy_info_;
  BoundNetLog net_log_;
  bool was_npn_negotiated_;
  
  NextProto protocol_negotiated_;
  scoped_refptr<GrowableIOBuffer> read_buf_;
  int next_pipeline_id_;
  bool active_;
  bool usable_;
  bool completed_one_request_;
  base::WeakPtrFactory<HttpPipelinedConnectionImpl> weak_factory_;

  StreamInfoMap stream_info_map_;

  std::queue<int> request_order_;

  std::queue<PendingSendRequest*> pending_send_request_queue_;
  scoped_ptr<PendingSendRequest> active_send_request_;
  SendRequestState send_next_state_;
  bool send_still_on_call_stack_;

  ReadHeadersState read_next_state_;
  int active_read_id_;
  bool read_still_on_call_stack_;

  DISALLOW_COPY_AND_ASSIGN(HttpPipelinedConnectionImpl);
};

}  

#endif  
