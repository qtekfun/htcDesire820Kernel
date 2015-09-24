// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_TEST_UTIL_H_
#define NET_SPDY_SPDY_TEST_UTIL_H_
#pragma once

#include "base/basictypes.h"
#include "net/base/cert_verifier.h"
#include "net/base/mock_host_resolver.h"
#include "net/base/request_priority.h"
#include "net/base/ssl_config_service_defaults.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_cache.h"
#include "net/http/http_network_session.h"
#include "net/http/http_network_layer.h"
#include "net/http/http_transaction_factory.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/socket_test_util.h"
#include "net/spdy/spdy_framer.h"
#include "net/url_request/url_request_context.h"

namespace net {

const char kDefaultURL[] = "http://www.google.com";
const char kUploadData[] = "hello!";
const int kUploadDataSize = arraysize(kUploadData)-1;

struct SpdyHeaderInfo {
  spdy::SpdyControlType kind;
  spdy::SpdyStreamId id;
  spdy::SpdyStreamId assoc_id;
  spdy::SpdyPriority priority;
  spdy::SpdyControlFlags control_flags;
  bool compressed;
  spdy::SpdyStatusCodes status;
  const char* data;
  uint32 data_length;
  spdy::SpdyDataFlags data_flags;
};

MockWrite* ChopWriteFrame(const char* data, int length, int num_chunks);

MockWrite* ChopWriteFrame(const spdy::SpdyFrame& frame, int num_chunks);

MockRead* ChopReadFrame(const char* data, int length, int num_chunks);

MockRead* ChopReadFrame(const spdy::SpdyFrame& frame, int num_chunks);

void AppendHeadersToSpdyFrame(const char* const extra_headers[],
                              int extra_header_count,
                              spdy::SpdyHeaderBlock* headers);

// Returns the number of bytes written into *|buffer_handle|
template<class T>
int AppendToBuffer(const char* str,
                   int len,
                   T** buffer_handle,
                   int* buffer_len_remaining) {
  DCHECK_GT(len, 0);
  DCHECK(NULL != buffer_handle) << "NULL buffer handle";
  DCHECK(NULL != *buffer_handle) << "NULL pointer";
  DCHECK(NULL != buffer_len_remaining)
      << "NULL buffer remainder length pointer";
  DCHECK_GE(*buffer_len_remaining, len) << "Insufficient buffer size";
  memcpy(*buffer_handle, str, len);
  *buffer_handle += len;
  *buffer_len_remaining -= len;
  return len;
}

// Returns the number of bytes written
int AppendToBuffer(int val,
                   int len,
                   unsigned char** buffer_handle,
                   int* buffer_len_remaining);

spdy::SpdyFrame* ConstructSpdyPacket(const SpdyHeaderInfo& header_info,
                                     const char* const extra_headers[],
                                     int extra_header_count,
                                     const char* const tail[],
                                     int tail_header_count);

spdy::SpdyFrame* ConstructSpdyControlFrame(const char* const extra_headers[],
                                           int extra_header_count,
                                           bool compressed,
                                           int stream_id,
                                           RequestPriority request_priority,
                                           spdy::SpdyControlType type,
                                           spdy::SpdyControlFlags flags,
                                           const char* const* kHeaders,
                                           int kHeadersSize);
spdy::SpdyFrame* ConstructSpdyControlFrame(const char* const extra_headers[],
                                           int extra_header_count,
                                           bool compressed,
                                           int stream_id,
                                           RequestPriority request_priority,
                                           spdy::SpdyControlType type,
                                           spdy::SpdyControlFlags flags,
                                           const char* const* kHeaders,
                                           int kHeadersSize,
                                           int associated_stream_id);

// Returns the number of bytes written into |buffer|.
int ConstructSpdyReplyString(const char* const extra_headers[],
                             int extra_header_count,
                             char* buffer,
                             int buffer_length);

spdy::SpdyFrame* ConstructSpdySettings(spdy::SpdySettings settings);

spdy::SpdyFrame* ConstructSpdyPing();

spdy::SpdyFrame* ConstructSpdyGoAway();

spdy::SpdyFrame* ConstructSpdyWindowUpdate(spdy::SpdyStreamId,
                                           uint32 delta_window_size);

spdy::SpdyFrame* ConstructSpdyRstStream(spdy::SpdyStreamId stream_id,
                                        spdy::SpdyStatusCodes status);

// Returns the number of bytes written into |buffer|.
int ConstructSpdyHeader(const char* const extra_headers[],
                        int extra_header_count,
                        char* buffer,
                        int buffer_length,
                        int index);

spdy::SpdyFrame* ConstructSpdyGet(const char* const url,
                                  bool compressed,
                                  int stream_id,
                                  RequestPriority request_priority);

spdy::SpdyFrame* ConstructSpdyGet(const char* const extra_headers[],
                                  int extra_header_count,
                                  bool compressed,
                                  int stream_id,
                                  RequestPriority request_priority);

spdy::SpdyFrame* ConstructSpdyGet(const char* const extra_headers[],
                                  int extra_header_count,
                                  bool compressed,
                                  int stream_id,
                                  RequestPriority request_priority,
                                  bool direct);

spdy::SpdyFrame* ConstructSpdyConnect(const char* const extra_headers[],
                                      int extra_header_count,
                                      int stream_id);

spdy::SpdyFrame* ConstructSpdyPush(const char* const extra_headers[],
                                  int extra_header_count,
                                  int stream_id,
                                  int associated_stream_id);
spdy::SpdyFrame* ConstructSpdyPush(const char* const extra_headers[],
                                  int extra_header_count,
                                  int stream_id,
                                  int associated_stream_id,
                                  const char* url);
spdy::SpdyFrame* ConstructSpdyPush(const char* const extra_headers[],
                                  int extra_header_count,
                                  int stream_id,
                                  int associated_stream_id,
                                  const char* url,
                                  const char* status,
                                  const char* location);
spdy::SpdyFrame* ConstructSpdyPush(int stream_id,
                                  int associated_stream_id,
                                  const char* url);

spdy::SpdyFrame* ConstructSpdyPushHeaders(int stream_id,
                                          const char* const extra_headers[],
                                          int extra_header_count);

spdy::SpdyFrame* ConstructSpdyGetSynReply(const char* const extra_headers[],
                                          int extra_header_count,
                                          int stream_id);

spdy::SpdyFrame* ConstructSpdyGetSynReplyRedirect(int stream_id);

spdy::SpdyFrame* ConstructSpdySynReplyError(int stream_id);

spdy::SpdyFrame* ConstructSpdySynReplyError(
    const char* const status,
    const char* const* const extra_headers,
    int extra_header_count,
    int stream_id);

spdy::SpdyFrame* ConstructSpdyPost(int64 content_length,
                                   const char* const extra_headers[],
                                   int extra_header_count);

spdy::SpdyFrame* ConstructChunkedSpdyPost(const char* const extra_headers[],
                                          int extra_header_count);

spdy::SpdyFrame* ConstructSpdyPostSynReply(const char* const extra_headers[],
                                           int extra_header_count);

spdy::SpdyFrame* ConstructSpdyBodyFrame(int stream_id,
                                        bool fin);

spdy::SpdyFrame* ConstructSpdyBodyFrame(int stream_id, const char* data,
                                        uint32 len, bool fin);

spdy::SpdyFrame* ConstructWrappedSpdyFrame(
    const scoped_ptr<spdy::SpdyFrame>& frame, int stream_id);

MockWrite CreateMockWrite(const spdy::SpdyFrame& req);

MockWrite CreateMockWrite(const spdy::SpdyFrame& req, int seq);

MockWrite CreateMockWrite(const spdy::SpdyFrame& req, int seq, bool async);

MockRead CreateMockRead(const spdy::SpdyFrame& resp);

MockRead CreateMockRead(const spdy::SpdyFrame& resp, int seq);

MockRead CreateMockRead(const spdy::SpdyFrame& resp, int seq, bool async);

int CombineFrames(const spdy::SpdyFrame** frames, int num_frames,
                  char* buff, int buff_len);

class SpdySessionDependencies {
 public:
  
  SpdySessionDependencies();

  
  explicit SpdySessionDependencies(ProxyService* proxy_service);

  ~SpdySessionDependencies();

  static HttpNetworkSession* SpdyCreateSession(
      SpdySessionDependencies* session_deps);
  static HttpNetworkSession* SpdyCreateSessionDeterministic(
      SpdySessionDependencies* session_deps);

  
  scoped_ptr<MockHostResolverBase> host_resolver;
  scoped_ptr<CertVerifier> cert_verifier;
  scoped_refptr<ProxyService> proxy_service;
  scoped_refptr<SSLConfigService> ssl_config_service;
  scoped_ptr<MockClientSocketFactory> socket_factory;
  scoped_ptr<DeterministicMockClientSocketFactory> deterministic_socket_factory;
  scoped_ptr<HttpAuthHandlerFactory> http_auth_handler_factory;
};

class SpdyURLRequestContext : public URLRequestContext {
 public:
  SpdyURLRequestContext();

  MockClientSocketFactory& socket_factory() { return socket_factory_; }

 protected:
  virtual ~SpdyURLRequestContext();

 private:
  MockClientSocketFactory socket_factory_;
};

const SpdyHeaderInfo make_spdy_header(spdy::SpdyControlType type);
}  

#endif  
