// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012, Code Aurora Forum. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_PARSER_H_
#define NET_HTTP_HTTP_STREAM_PARSER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/base/net_log.h"
#include "net/base/upload_data_stream.h"
#include "net/http/http_chunked_decoder.h"

namespace net {

class ClientSocketHandle;
class DrainableIOBuffer;
class GrowableIOBuffer;
struct HttpRequestInfo;
class HttpRequestHeaders;
class HttpResponseInfo;
class IOBuffer;
class SSLCertRequestInfo;
class SSLInfo;

class HttpStreamParser  : public ChunkCallback {
 public:
  
  
  
  
  
  HttpStreamParser(ClientSocketHandle* connection,
                   const HttpRequestInfo* request,
                   GrowableIOBuffer* read_buffer,
                   const BoundNetLog& net_log,
                   bool using_proxy = false);

  ~HttpStreamParser();

  
  
  int SendRequest(const std::string& request_line,
                  const HttpRequestHeaders& headers,
                  UploadDataStream* request_body,
                  HttpResponseInfo* response, CompletionCallback* callback);

  int ReadResponseHeaders(CompletionCallback* callback);

  int ReadResponseBody(IOBuffer* buf, int buf_len,
                       CompletionCallback* callback);

  void Close(bool not_reusable);

  uint64 GetUploadProgress() const;

  HttpResponseInfo* GetResponseInfo();

  bool IsResponseBodyComplete() const;

  bool CanFindEndOfResponse() const;

  bool IsMoreDataBuffered() const;

  bool IsConnectionReused() const;

  void SetConnectionReused();

  bool IsConnectionReusable() const;

  void GetSSLInfo(SSLInfo* ssl_info);

  void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  
  virtual void OnChunkAvailable();

 private:
  
  
  enum State {
    STATE_NONE,
    STATE_SENDING_HEADERS,
    STATE_SENDING_BODY,
    STATE_REQUEST_SENT,
    STATE_READ_HEADERS,
    STATE_READ_HEADERS_COMPLETE,
    STATE_BODY_PENDING,
    STATE_READ_BODY,
    STATE_READ_BODY_COMPLETE,
    STATE_DONE
  };

  
  
  enum { kHeaderBufInitialSize = 4096 };

  
  
  
  
  enum { kMaxHeaderBufSize = 256 * 1024 };  

  
  enum { kMaxBufSize = 2 * 1024 * 1024 };  

  
  void OnIOComplete(int result);

  
  int DoLoop(int result);

  
  int DoSendHeaders(int result);
  int DoSendBody(int result);
  int DoReadHeaders();
  int DoReadHeadersComplete(int result);
  int DoReadBody();
  int DoReadBodyComplete(int result);

  
  
  
  
  int ParseResponseHeaders();

  
  
  int DoParseResponseHeaders(int end_of_header_offset);

  
  void CalculateResponseBodySize();

  
  State io_state_;

  
  const HttpRequestInfo* request_;

  
  scoped_refptr<DrainableIOBuffer> request_headers_;

  
  scoped_ptr<UploadDataStream> request_body_;

  
  scoped_refptr<GrowableIOBuffer> read_buf_;

  
  
  int read_buf_unused_offset_;

  
  
  int response_header_start_offset_;

  
  HttpResponseInfo* response_;

  
  
  
  int64 response_body_length_;

  
  int64 response_body_read_;

  
  scoped_ptr<HttpChunkedDecoder> chunked_decoder_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  
  CompletionCallback* user_callback_;

  
  
  
  
  CompletionCallback* scheduled_callback_;

  
  ClientSocketHandle* const connection_;

  bool using_proxy_;

  bool has_to_retry_;

  BoundNetLog net_log_;

  
  CompletionCallbackImpl<HttpStreamParser> io_callback_;

  
  
  scoped_refptr<IOBuffer> chunk_buf_;
  size_t chunk_length_;
  size_t chunk_length_without_encoding_;
  bool sent_last_chunk_;

  DISALLOW_COPY_AND_ASSIGN(HttpStreamParser);
};

}  

#endif  
