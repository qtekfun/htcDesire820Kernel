// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_TEST_UTIL_COMMON_H_
#define NET_SPDY_SPDY_TEST_UTIL_COMMON_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "crypto/ec_private_key.h"
#include "crypto/ec_signature_creator.h"
#include "net/base/completion_callback.h"
#include "net/base/request_priority.h"
#include "net/base/test_completion_callback.h"
#include "net/cert/cert_verifier.h"
#include "net/dns/mock_host_resolver.h"
#include "net/http/http_auth_handler_factory.h"
#include "net/http/http_network_session.h"
#include "net/http/http_response_info.h"
#include "net/http/http_server_properties_impl.h"
#include "net/http/transport_security_state.h"
#include "net/proxy/proxy_service.h"
#include "net/socket/next_proto.h"
#include "net/socket/socket_test_util.h"
#include "net/spdy/spdy_protocol.h"
#include "net/ssl/ssl_config_service_defaults.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_storage.h"
#include "testing/gtest/include/gtest/gtest.h"

class GURL;

namespace net {

class BoundNetLog;
class SpdySession;
class SpdySessionKey;
class SpdySessionPool;
class SpdyStream;
class SpdyStreamRequest;

const char kDefaultURL[] = "http://www.google.com";
const char kUploadData[] = "hello!";
const int kUploadDataSize = arraysize(kUploadData)-1;

std::vector<NextProto> SpdyNextProtos();

MockWrite* ChopWriteFrame(const char* data, int length, int num_chunks);

MockWrite* ChopWriteFrame(const SpdyFrame& frame, int num_chunks);

MockRead* ChopReadFrame(const char* data, int length, int num_chunks);

MockRead* ChopReadFrame(const SpdyFrame& frame, int num_chunks);

void AppendToHeaderBlock(const char* const extra_headers[],
                         int extra_header_count,
                         SpdyHeaderBlock* headers);

MockWrite CreateMockWrite(const SpdyFrame& req);

MockWrite CreateMockWrite(const SpdyFrame& req, int seq);

MockWrite CreateMockWrite(const SpdyFrame& req, int seq, IoMode mode);

MockRead CreateMockRead(const SpdyFrame& resp);

MockRead CreateMockRead(const SpdyFrame& resp, int seq);

MockRead CreateMockRead(const SpdyFrame& resp, int seq, IoMode mode);

int CombineFrames(const SpdyFrame** frames, int num_frames,
                  char* buff, int buff_len);

bool GetSpdyPriority(SpdyMajorVersion version,
                     const SpdyFrame& frame,
                     SpdyPriority* priority);

base::WeakPtr<SpdyStream> CreateStreamSynchronously(
    SpdyStreamType type,
    const base::WeakPtr<SpdySession>& session,
    const GURL& url,
    RequestPriority priority,
    const BoundNetLog& net_log);

class StreamReleaserCallback : public TestCompletionCallbackBase {
 public:
  StreamReleaserCallback();

  virtual ~StreamReleaserCallback();

  
  CompletionCallback MakeCallback(SpdyStreamRequest* request);

 private:
  void OnComplete(SpdyStreamRequest* request, int result);
};

const size_t kSpdyCredentialSlotUnused = 0;

struct SpdyHeaderInfo {
  SpdyFrameType kind;
  SpdyStreamId id;
  SpdyStreamId assoc_id;
  SpdyPriority priority;
  size_t credential_slot;  
  SpdyControlFlags control_flags;
  bool compressed;
  SpdyRstStreamStatus status;
  const char* data;
  uint32 data_length;
  SpdyDataFlags data_flags;
};

class MockECSignatureCreator : public crypto::ECSignatureCreator {
 public:
  explicit MockECSignatureCreator(crypto::ECPrivateKey* key);

  
  virtual bool Sign(const uint8* data,
                    int data_len,
                    std::vector<uint8>* signature) OVERRIDE;
  virtual bool DecodeSignature(const std::vector<uint8>& signature,
                               std::vector<uint8>* out_raw_sig) OVERRIDE;

 private:
  crypto::ECPrivateKey* key_;

  DISALLOW_COPY_AND_ASSIGN(MockECSignatureCreator);
};

class MockECSignatureCreatorFactory : public crypto::ECSignatureCreatorFactory {
 public:
  MockECSignatureCreatorFactory();
  virtual ~MockECSignatureCreatorFactory();

  
  virtual crypto::ECSignatureCreator* Create(
      crypto::ECPrivateKey* key) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(MockECSignatureCreatorFactory);
};

struct SpdySessionDependencies {
  
  explicit SpdySessionDependencies(NextProto protocol);

  
  SpdySessionDependencies(NextProto protocol, ProxyService* proxy_service);

  ~SpdySessionDependencies();

  static HttpNetworkSession* SpdyCreateSession(
      SpdySessionDependencies* session_deps);
  static HttpNetworkSession* SpdyCreateSessionDeterministic(
      SpdySessionDependencies* session_deps);
  static HttpNetworkSession::Params CreateSessionParams(
      SpdySessionDependencies* session_deps);

  
  scoped_ptr<MockHostResolverBase> host_resolver;
  scoped_ptr<CertVerifier> cert_verifier;
  scoped_ptr<TransportSecurityState> transport_security_state;
  scoped_ptr<ProxyService> proxy_service;
  scoped_refptr<SSLConfigService> ssl_config_service;
  scoped_ptr<MockClientSocketFactory> socket_factory;
  scoped_ptr<DeterministicMockClientSocketFactory> deterministic_socket_factory;
  scoped_ptr<HttpAuthHandlerFactory> http_auth_handler_factory;
  HttpServerPropertiesImpl http_server_properties;
  bool enable_ip_pooling;
  bool enable_compression;
  bool enable_ping;
  bool enable_user_alternate_protocol_ports;
  NextProto protocol;
  size_t stream_initial_recv_window_size;
  SpdySession::TimeFunc time_func;
  std::string trusted_spdy_proxy;
  NetLog* net_log;
};

class SpdyURLRequestContext : public URLRequestContext {
 public:
  explicit SpdyURLRequestContext(NextProto protocol);
  virtual ~SpdyURLRequestContext();

  MockClientSocketFactory& socket_factory() { return socket_factory_; }

 private:
  MockClientSocketFactory socket_factory_;
  net::URLRequestContextStorage storage_;
};

bool HasSpdySession(SpdySessionPool* pool, const SpdySessionKey& key);

base::WeakPtr<SpdySession> CreateInsecureSpdySession(
    const scoped_refptr<HttpNetworkSession>& http_session,
    const SpdySessionKey& key,
    const BoundNetLog& net_log);

void TryCreateInsecureSpdySessionExpectingFailure(
    const scoped_refptr<HttpNetworkSession>& http_session,
    const SpdySessionKey& key,
    Error expected_error,
    const BoundNetLog& net_log);

base::WeakPtr<SpdySession> CreateSecureSpdySession(
    const scoped_refptr<HttpNetworkSession>& http_session,
    const SpdySessionKey& key,
    const BoundNetLog& net_log);

base::WeakPtr<SpdySession> CreateFakeSpdySession(SpdySessionPool* pool,
                                                 const SpdySessionKey& key);

void TryCreateFakeSpdySessionExpectingFailure(SpdySessionPool* pool,
                                              const SpdySessionKey& key,
                                              Error expected_error);

class SpdySessionPoolPeer {
 public:
  explicit SpdySessionPoolPeer(SpdySessionPool* pool);

  void RemoveAliases(const SpdySessionKey& key);
  void DisableDomainAuthenticationVerification();
  void SetEnableSendingInitialData(bool enabled);

 private:
  SpdySessionPool* const pool_;

  DISALLOW_COPY_AND_ASSIGN(SpdySessionPoolPeer);
};

class SpdyTestUtil {
 public:
  explicit SpdyTestUtil(NextProto protocol);

  
  void AddUrlToHeaderBlock(base::StringPiece url,
                           SpdyHeaderBlock* headers) const;

  scoped_ptr<SpdyHeaderBlock> ConstructGetHeaderBlock(
      base::StringPiece url) const;
  scoped_ptr<SpdyHeaderBlock> ConstructGetHeaderBlockForProxy(
      base::StringPiece url) const;
  scoped_ptr<SpdyHeaderBlock> ConstructHeadHeaderBlock(
      base::StringPiece url,
      int64 content_length) const;
  scoped_ptr<SpdyHeaderBlock> ConstructPostHeaderBlock(
      base::StringPiece url,
      int64 content_length) const;
  scoped_ptr<SpdyHeaderBlock> ConstructPutHeaderBlock(
      base::StringPiece url,
      int64 content_length) const;

  
  
  
  SpdyFrame* ConstructSpdyFrame(
      const SpdyHeaderInfo& header_info,
      scoped_ptr<SpdyHeaderBlock> headers) const;

  
  
  
  
  SpdyFrame* ConstructSpdyFrame(const SpdyHeaderInfo& header_info,
                                const char* const extra_headers[],
                                int extra_header_count,
                                const char* const tail_headers[],
                                int tail_header_count) const;

  
  SpdyFrame* ConstructSpdyControlFrame(
      scoped_ptr<SpdyHeaderBlock> headers,
      bool compressed,
      SpdyStreamId stream_id,
      RequestPriority request_priority,
      SpdyFrameType type,
      SpdyControlFlags flags,
      SpdyStreamId associated_stream_id) const;

  
  
  
  
  
  
  SpdyFrame* ConstructSpdyControlFrame(
      const char* const extra_headers[],
      int extra_header_count,
      bool compressed,
      SpdyStreamId stream_id,
      RequestPriority request_priority,
      SpdyFrameType type,
      SpdyControlFlags flags,
      const char* const* tail_headers,
      int tail_headers_size,
      SpdyStreamId associated_stream_id) const;

  
  std::string ConstructSpdyReplyString(const SpdyHeaderBlock& headers) const;

  
  
  
  SpdyFrame* ConstructSpdySettings(const SettingsMap& settings) const;

  
  
  
  SpdyFrame* ConstructSpdyCredential(const SpdyCredential& credential) const;

  
  
  SpdyFrame* ConstructSpdyPing(uint32 ping_id) const;

  
  
  SpdyFrame* ConstructSpdyGoAway() const;

  
  
  SpdyFrame* ConstructSpdyGoAway(SpdyStreamId last_good_stream_id) const;

  
  
  SpdyFrame* ConstructSpdyWindowUpdate(
      SpdyStreamId stream_id,
      uint32 delta_window_size) const;

  
  
  SpdyFrame* ConstructSpdyRstStream(SpdyStreamId stream_id,
                                    SpdyRstStreamStatus status) const;

  
  
  
  
  
  SpdyFrame* ConstructSpdyGet(const char* const url,
                              bool compressed,
                              SpdyStreamId stream_id,
                              RequestPriority request_priority) const;

  SpdyFrame* ConstructSpdyGetForProxy(const char* const url,
                                      bool compressed,
                                      SpdyStreamId stream_id,
                                      RequestPriority request_priority) const;

  
  
  
  
  
  SpdyFrame* ConstructSpdyGet(const char* const extra_headers[],
                              int extra_header_count,
                              bool compressed,
                              int stream_id,
                              RequestPriority request_priority,
                              bool direct) const;

  
  SpdyFrame* ConstructSpdyConnect(const char* const extra_headers[],
                                  int extra_header_count,
                                  int stream_id,
                                  RequestPriority priority) const;

  
  
  
  
  SpdyFrame* ConstructSpdyPush(const char* const extra_headers[],
                               int extra_header_count,
                               int stream_id,
                               int associated_stream_id,
                               const char* url);
  SpdyFrame* ConstructSpdyPush(const char* const extra_headers[],
                               int extra_header_count,
                               int stream_id,
                               int associated_stream_id,
                               const char* url,
                               const char* status,
                               const char* location);

  SpdyFrame* ConstructSpdyPushHeaders(int stream_id,
                                      const char* const extra_headers[],
                                      int extra_header_count);

  
  
  
  
  SpdyFrame* ConstructSpdyGetSynReply(const char* const extra_headers[],
                                      int extra_header_count,
                                      int stream_id);

  
  
  
  
  SpdyFrame* ConstructSpdyGetSynReplyRedirect(int stream_id);

  
  
  
  SpdyFrame* ConstructSpdySynReplyError(int stream_id);

  
  
  SpdyFrame* ConstructSpdySynReplyError(const char* const status,
                                        const char* const* const extra_headers,
                                        int extra_header_count,
                                        int stream_id);

  
  
  
  
  SpdyFrame* ConstructSpdyPost(const char* url,
                               SpdyStreamId stream_id,
                               int64 content_length,
                               RequestPriority priority,
                               const char* const extra_headers[],
                               int extra_header_count);

  
  
  
  
  SpdyFrame* ConstructChunkedSpdyPost(const char* const extra_headers[],
                                      int extra_header_count);

  
  
  
  
  SpdyFrame* ConstructSpdyPostSynReply(const char* const extra_headers[],
                                       int extra_header_count);

  
  SpdyFrame* ConstructSpdyBodyFrame(int stream_id,
                                    bool fin);

  
  SpdyFrame* ConstructSpdyBodyFrame(int stream_id, const char* data,
                                    uint32 len, bool fin);

  
  SpdyFrame* ConstructWrappedSpdyFrame(const scoped_ptr<SpdyFrame>& frame,
                                       int stream_id);

  const SpdyHeaderInfo MakeSpdyHeader(SpdyFrameType type);

  NextProto protocol() const { return protocol_; }
  SpdyMajorVersion spdy_version() const { return spdy_version_; }
  bool is_spdy2() const { return protocol_ < kProtoSPDY3; }
  scoped_ptr<SpdyFramer> CreateFramer() const;

  const char* GetMethodKey() const;
  const char* GetStatusKey() const;
  const char* GetHostKey() const;
  const char* GetSchemeKey() const;
  const char* GetVersionKey() const;
  const char* GetPathKey() const;

 private:
  
  
  scoped_ptr<SpdyHeaderBlock> ConstructHeaderBlock(
      base::StringPiece method,
      base::StringPiece url,
      int64* content_length) const;

  const NextProto protocol_;
  const SpdyMajorVersion spdy_version_;
};

}  

#endif  
