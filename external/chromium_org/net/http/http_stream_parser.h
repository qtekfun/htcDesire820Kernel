// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_PARSER_H_
#define NET_HTTP_HTTP_STREAM_PARSER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/upload_progress.h"

namespace net {

class ClientSocketHandle;
class DrainableIOBuffer;
class GrowableIOBuffer;
class HttpChunkedDecoder;
struct HttpRequestInfo;
class HttpRequestHeaders;
class HttpResponseInfo;
class IOBuffer;
class IOBufferWithSize;
class SSLCertRequestInfo;
class SSLInfo;
class UploadDataStream;

class NET_EXPORT_PRIVATE HttpStreamParser {
 public:
  
  
  
  
  
  HttpStreamParser(ClientSocketHandle* connection,
                   const HttpRequestInfo* request,
                   GrowableIOBuffer* read_buffer,
                   const BoundNetLog& net_log);
  virtual ~HttpStreamParser();

  
  
  int SendRequest(const std::string& request_line,
                  const HttpRequestHeaders& headers,
                  HttpResponseInfo* response,
                  const CompletionCallback& callback);

  int ReadResponseHeaders(const CompletionCallback& callback);

  int ReadResponseBody(IOBuffer* buf, int buf_len,
                       const CompletionCallback& callback);

  void Close(bool not_reusable);

  
  
  UploadProgress GetUploadProgress() const;

  HttpResponseInfo* GetResponseInfo();

  bool IsResponseBodyComplete() const;

  bool CanFindEndOfResponse() const;

  bool IsMoreDataBuffered() const;

  bool IsConnectionReused() const;

  void SetConnectionReused();

  bool IsConnectionReusable() const;

  int64 received_bytes() const { return received_bytes_; }

  void GetSSLInfo(SSLInfo* ssl_info);

  void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  
  // Returns the number of bytes written to |output|. |output_size| should
  
  
  
  
  
  
  static int EncodeChunk(const base::StringPiece& payload,
                         char* output,
                         size_t output_size);

  
  
  static bool ShouldMergeRequestHeadersAndBody(
      const std::string& request_headers,
      const UploadDataStream* request_body);

  
  static const size_t kChunkHeaderFooterSize;

 private:
  class SeekableIOBuffer;

  
  
  enum State {
    STATE_NONE,
    STATE_SENDING_HEADERS,
    
    
    
    STATE_SENDING_BODY,
    STATE_SEND_REQUEST_READING_BODY,
    STATE_REQUEST_SENT,
    STATE_READ_HEADERS,
    STATE_READ_HEADERS_COMPLETE,
    STATE_BODY_PENDING,
    STATE_READ_BODY,
    STATE_READ_BODY_COMPLETE,
    STATE_DONE
  };

  
  
  static const int kHeaderBufInitialSize = 4 * 1024;  

  
  
  
  
  static const int kMaxHeaderBufSize = kHeaderBufInitialSize * 64;  

  
  static const int kMaxBufSize = 2 * 1024 * 1024;  

  
  void OnIOComplete(int result);

  
  int DoLoop(int result);

  
  int DoSendHeaders(int result);
  int DoSendBody(int result);
  int DoSendRequestReadingBody(int result);
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

  
  scoped_refptr<GrowableIOBuffer> read_buf_;

  
  
  
  int read_buf_unused_offset_;

  
  
  int response_header_start_offset_;

  
  
  int64 received_bytes_;

  
  HttpResponseInfo* response_;

  
  
  
  int64 response_body_length_;

  
  int64 response_body_read_;

  
  scoped_ptr<HttpChunkedDecoder> chunked_decoder_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  
  CompletionCallback callback_;

  
  
  
  
  CompletionCallback scheduled_callback_;

  
  ClientSocketHandle* const connection_;

  BoundNetLog net_log_;

  
  CompletionCallback io_callback_;

  
  scoped_refptr<SeekableIOBuffer> request_body_read_buf_;
  
  
  scoped_refptr<SeekableIOBuffer> request_body_send_buf_;
  bool sent_last_chunk_;

  base::WeakPtrFactory<HttpStreamParser> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(HttpStreamParser);
};

}  

#endif  
