// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_H_
#define NET_SPDY_SPDY_SESSION_H_

#include <deque>
#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "net/base/io_buffer.h"
#include "net/base/load_states.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/client_socket_pool.h"
#include "net/socket/next_proto.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/stream_socket.h"
#include "net/spdy/buffered_spdy_framer.h"
#include "net/spdy/spdy_buffer.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"
#include "net/spdy/spdy_session_pool.h"
#include "net/spdy/spdy_stream.h"
#include "net/spdy/spdy_write_queue.h"
#include "net/ssl/ssl_config_service.h"
#include "url/gurl.h"

namespace net {

const int kMss = 1430;
const int kMaxSpdyFrameChunkSize = (2 * kMss) - 8;

const size_t kInitialMaxConcurrentStreams = 100;

const int kMaxConcurrentPushedStreams = 1000;

const int kMaxReadBytesWithoutYielding = 32 * 1024;

const int32 kDefaultInitialRecvWindowSize = 10 * 1024 * 1024;  

class BoundNetLog;
struct LoadTimingInfo;
class SpdyStream;
class SSLInfo;

enum SpdyProtocolErrorDetails {
  
  SPDY_ERROR_NO_ERROR,
  SPDY_ERROR_INVALID_CONTROL_FRAME,
  SPDY_ERROR_CONTROL_PAYLOAD_TOO_LARGE,
  SPDY_ERROR_ZLIB_INIT_FAILURE,
  SPDY_ERROR_UNSUPPORTED_VERSION,
  SPDY_ERROR_DECOMPRESS_FAILURE,
  SPDY_ERROR_COMPRESS_FAILURE,
  SPDY_ERROR_CREDENTIAL_FRAME_CORRUPT,
  SPDY_ERROR_INVALID_DATA_FRAME_FLAGS,
  SPDY_ERROR_INVALID_CONTROL_FRAME_FLAGS,
  
  STATUS_CODE_INVALID,
  STATUS_CODE_PROTOCOL_ERROR,
  STATUS_CODE_INVALID_STREAM,
  STATUS_CODE_REFUSED_STREAM,
  STATUS_CODE_UNSUPPORTED_VERSION,
  STATUS_CODE_CANCEL,
  STATUS_CODE_INTERNAL_ERROR,
  STATUS_CODE_FLOW_CONTROL_ERROR,
  STATUS_CODE_STREAM_IN_USE,
  STATUS_CODE_STREAM_ALREADY_CLOSED,
  STATUS_CODE_INVALID_CREDENTIALS,
  STATUS_CODE_FRAME_TOO_LARGE,
  
  PROTOCOL_ERROR_UNEXPECTED_PING,
  PROTOCOL_ERROR_RST_STREAM_FOR_NON_ACTIVE_STREAM,
  PROTOCOL_ERROR_SPDY_COMPRESSION_FAILURE,
  PROTOCOL_ERROR_REQUEST_FOR_SECURE_CONTENT_OVER_INSECURE_SESSION,
  PROTOCOL_ERROR_SYN_REPLY_NOT_RECEIVED,
  PROTOCOL_ERROR_INVALID_WINDOW_UPDATE_SIZE,
  PROTOCOL_ERROR_RECEIVE_WINDOW_VIOLATION,
  NUM_SPDY_PROTOCOL_ERROR_DETAILS
};

COMPILE_ASSERT(STATUS_CODE_INVALID ==
               static_cast<SpdyProtocolErrorDetails>(SpdyFramer::LAST_ERROR),
               SpdyProtocolErrorDetails_SpdyErrors_mismatch);

COMPILE_ASSERT(PROTOCOL_ERROR_UNEXPECTED_PING ==
               static_cast<SpdyProtocolErrorDetails>(
                   RST_STREAM_NUM_STATUS_CODES + STATUS_CODE_INVALID),
               SpdyProtocolErrorDetails_SpdyErrors_mismatch);

class NET_EXPORT_PRIVATE SpdyStreamRequest {
 public:
  SpdyStreamRequest();
  
  ~SpdyStreamRequest();

  
  
  
  
  
  
  
  
  
  
  
  
  int StartRequest(SpdyStreamType type,
                   const base::WeakPtr<SpdySession>& session,
                   const GURL& url,
                   RequestPriority priority,
                   const BoundNetLog& net_log,
                   const CompletionCallback& callback);

  
  
  void CancelRequest();

  
  
  
  
  base::WeakPtr<SpdyStream> ReleaseStream();

 private:
  friend class SpdySession;

  
  
  void OnRequestCompleteSuccess(const base::WeakPtr<SpdyStream>& stream);

  
  
  
  void OnRequestCompleteFailure(int rv);

  
  SpdyStreamType type() const { return type_; }
  const GURL& url() const { return url_; }
  RequestPriority priority() const { return priority_; }
  const BoundNetLog& net_log() const { return net_log_; }

  void Reset();

  base::WeakPtrFactory<SpdyStreamRequest> weak_ptr_factory_;
  SpdyStreamType type_;
  base::WeakPtr<SpdySession> session_;
  base::WeakPtr<SpdyStream> stream_;
  GURL url_;
  RequestPriority priority_;
  BoundNetLog net_log_;
  CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(SpdyStreamRequest);
};

class NET_EXPORT SpdySession : public BufferedSpdyFramerVisitorInterface,
                               public SpdyFramerDebugVisitorInterface,
                               public HigherLayeredPool {
 public:
  
  typedef base::TimeTicks (*TimeFunc)(void);

  
  enum FlowControlState {
    FLOW_CONTROL_NONE,
    FLOW_CONTROL_STREAM,
    FLOW_CONTROL_STREAM_AND_SESSION
  };

  
  
  
  
  
  SpdySession(const SpdySessionKey& spdy_session_key,
              const base::WeakPtr<HttpServerProperties>& http_server_properties,
              bool verify_domain_authentication,
              bool enable_sending_initial_data,
              bool enable_compression,
              bool enable_ping_based_connection_checking,
              NextProto default_protocol,
              size_t stream_initial_recv_window_size,
              size_t initial_max_concurrent_streams,
              size_t max_concurrent_streams_limit,
              TimeFunc time_func,
              const HostPortPair& trusted_spdy_proxy,
              NetLog* net_log);

  virtual ~SpdySession();

  const HostPortPair& host_port_pair() const {
    return spdy_session_key_.host_port_proxy_pair().first;
  }
  const HostPortProxyPair& host_port_proxy_pair() const {
    return spdy_session_key_.host_port_proxy_pair();
  }
  const SpdySessionKey& spdy_session_key() const {
    return spdy_session_key_;
  }
  
  
  
  
  
  
  
  
  int GetPushStream(
      const GURL& url,
      base::WeakPtr<SpdyStream>* spdy_stream,
      const BoundNetLog& stream_net_log);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Error InitializeWithSocket(scoped_ptr<ClientSocketHandle> connection,
                             SpdySessionPool* pool,
                             bool is_secure,
                             int certificate_error_code);

  
  
  NextProto protocol() const { return protocol_; }

  
  
  
  
  
  
  
  
  
  
  bool VerifyDomainAuthentication(const std::string& domain);

  
  
  
  void EnqueueStreamWrite(const base::WeakPtr<SpdyStream>& stream,
                          SpdyFrameType frame_type,
                          scoped_ptr<SpdyBufferProducer> producer);

  
  scoped_ptr<SpdyFrame> CreateSynStream(
      SpdyStreamId stream_id,
      RequestPriority priority,
      uint8 credential_slot,
      SpdyControlFlags flags,
      const SpdyHeaderBlock& headers);

  
  
  scoped_ptr<SpdyBuffer> CreateDataBuffer(SpdyStreamId stream_id,
                                          IOBuffer* data,
                                          int len,
                                          SpdyDataFlags flags);

  
  
  
  void CloseActiveStream(SpdyStreamId stream_id, int status);

  
  
  
  void CloseCreatedStream(const base::WeakPtr<SpdyStream>& stream, int status);

  
  
  
  void ResetStream(SpdyStreamId stream_id,
                   SpdyRstStreamStatus status,
                   const std::string& description);

  
  bool IsStreamActive(SpdyStreamId stream_id) const;

  
  
  LoadState GetLoadState() const;

  
  bool GetSSLInfo(SSLInfo* ssl_info,
                  bool* was_npn_negotiated,
                  NextProto* protocol_negotiated);

  
  
  bool GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  
  
  void SendStreamWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size);

  
  
  
  
  
  bool IsClosed() const { return availability_state_ == STATE_CLOSED; }

  
  
  
  
  
  
  
  
  
  void CloseSessionOnError(Error err, const std::string& description);

  
  
  base::Value* GetInfoAsValue() const;

  
  
  bool IsReused() const;

  
  
  bool WasEverUsed() const {
    return connection_->socket()->WasEverUsed();
  }

  
  
  
  
  
  
  
  
  bool GetLoadTimingInfo(SpdyStreamId stream_id,
                         LoadTimingInfo* load_timing_info) const;

  
  bool is_active() const {
    return !active_streams_.empty() || !created_streams_.empty();
  }

  
  
  size_t num_active_streams() const { return active_streams_.size(); }
  size_t num_unclaimed_pushed_streams() const {
      return unclaimed_pushed_streams_.size();
  }
  size_t num_created_streams() const { return created_streams_.size(); }

  size_t pending_create_stream_queue_size(RequestPriority priority) const {
    DCHECK_GE(priority, MINIMUM_PRIORITY);
    DCHECK_LE(priority, MAXIMUM_PRIORITY);
    return pending_create_stream_queues_[priority].size();
  }

  
  FlowControlState flow_control_state() const {
    return flow_control_state_;
  }

  
  int32 stream_initial_send_window_size() const {
    return stream_initial_send_window_size_;
  }

  
  int32 stream_initial_recv_window_size() const {
    return stream_initial_recv_window_size_;
  }

  
  
  bool IsSendStalled() const {
    return
        flow_control_state_ == FLOW_CONTROL_STREAM_AND_SESSION &&
        session_send_window_size_ == 0;
  }

  const BoundNetLog& net_log() const { return net_log_; }

  int GetPeerAddress(IPEndPoint* address) const;
  int GetLocalAddress(IPEndPoint* address) const;

  
  void AddPooledAlias(const SpdySessionKey& alias_key);

  
  const std::set<SpdySessionKey>& pooled_aliases() const {
    return pooled_aliases_;
  }

  SpdyMajorVersion GetProtocolVersion() const;

  size_t GetDataFrameMinimumSize() const {
    return buffered_spdy_framer_->GetDataFrameMinimumSize();
  }

  size_t GetControlFrameHeaderSize() const {
    return buffered_spdy_framer_->GetControlFrameHeaderSize();
  }

  size_t GetFrameMinimumSize() const {
    return buffered_spdy_framer_->GetFrameMinimumSize();
  }

  size_t GetFrameMaximumSize() const {
    return buffered_spdy_framer_->GetFrameMaximumSize();
  }

  size_t GetDataFrameMaximumPayload() const {
    return buffered_spdy_framer_->GetDataFrameMaximumPayload();
  }

  
  base::WeakPtr<SpdySession> GetWeakPtr();

  
  virtual bool CloseOneIdleConnection() OVERRIDE;

 private:
  friend class base::RefCounted<SpdySession>;
  friend class SpdyStreamRequest;
  friend class SpdySessionTest;

  
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, ClientPing);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, FailedPing);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, GetActivePushStream);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, DeleteExpiredPushStreams);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, ProtocolNegotiation);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, ClearSettings);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, AdjustRecvWindowSize);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, AdjustSendWindowSize);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, SessionFlowControlInactiveStream);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, SessionFlowControlNoReceiveLeaks);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, SessionFlowControlNoSendLeaks);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, SessionFlowControlEndToEnd);

  typedef std::deque<base::WeakPtr<SpdyStreamRequest> >
      PendingStreamRequestQueue;

  struct ActiveStreamInfo {
    ActiveStreamInfo();
    explicit ActiveStreamInfo(SpdyStream* stream);
    ~ActiveStreamInfo();

    SpdyStream* stream;
    bool waiting_for_syn_reply;
  };
  typedef std::map<SpdyStreamId, ActiveStreamInfo> ActiveStreamMap;

  struct PushedStreamInfo {
    PushedStreamInfo();
    PushedStreamInfo(SpdyStreamId stream_id, base::TimeTicks creation_time);
    ~PushedStreamInfo();

    SpdyStreamId stream_id;
    base::TimeTicks creation_time;
  };
  typedef std::map<GURL, PushedStreamInfo> PushedStreamMap;

  typedef std::set<SpdyStream*> CreatedStreamSet;

  enum AvailabilityState {
    
    
    STATE_AVAILABLE,
    
    
    STATE_GOING_AWAY,
    
    
    STATE_CLOSED
  };

  enum ReadState {
    READ_STATE_DO_READ,
    READ_STATE_DO_READ_COMPLETE,
  };

  enum WriteState {
    
    WRITE_STATE_IDLE,
    WRITE_STATE_DO_WRITE,
    WRITE_STATE_DO_WRITE_COMPLETE,
  };

  
  enum CloseSessionResult {
    
    SESSION_ALREADY_CLOSED,
    
    
    SESSION_CLOSED_BUT_NOT_REMOVED,
    
    
    SESSION_CLOSED_AND_REMOVED,
  };

  
  
  
  
  Error TryAccessStream(const GURL& url);

  
  
  
  
  
  
  int TryCreateStream(const base::WeakPtr<SpdyStreamRequest>& request,
                      base::WeakPtr<SpdyStream>* stream);

  
  
  int CreateStream(const SpdyStreamRequest& request,
                   base::WeakPtr<SpdyStream>* stream);

  
  
  void CancelStreamRequest(const base::WeakPtr<SpdyStreamRequest>& request);

  
  
  base::WeakPtr<SpdyStreamRequest> GetNextPendingStreamRequest();

  
  
  
  void ProcessPendingStreamRequests();

  
  
  void CloseActiveStreamIterator(ActiveStreamMap::iterator it, int status);

  
  
  void CloseCreatedStreamIterator(CreatedStreamSet::iterator it, int status);

  
  
  void ResetStreamIterator(ActiveStreamMap::iterator it,
                           SpdyRstStreamStatus status,
                           const std::string& description);

  
  
  
  
  
  void EnqueueResetStreamFrame(SpdyStreamId stream_id,
                               RequestPriority priority,
                               SpdyRstStreamStatus status,
                               const std::string& description);

  
  
  
  
  
  void PumpReadLoop(ReadState expected_read_state, int result);

  
  
  
  
  
  int DoReadLoop(ReadState expected_read_state, int result);
  
  int DoRead();
  int DoReadComplete(int result);

  
  
  
  
  
  void PumpWriteLoop(WriteState expected_write_state, int result);

  
  
  
  
  int DoWriteLoop(WriteState expected_write_state, int result);
  
  int DoWrite();
  int DoWriteComplete(int result);

  
  

  
  
  
  void SendInitialData();

  
  void SendSettings(const SettingsMap& settings);

  
  
  void HandleSetting(uint32 id, uint32 value);

  
  void UpdateStreamsSendWindowSize(int32 delta_window_size);

  
  void SendPrefacePingIfNoneInFlight();

  
  void SendPrefacePing();

  
  void SendWindowUpdateFrame(SpdyStreamId stream_id, uint32 delta_window_size,
                             RequestPriority priority);

  
  void WritePingFrame(uint32 unique_id);

  
  
  void PlanToCheckPingStatus();

  
  
  void CheckPingStatus(base::TimeTicks last_check_time);

  
  int GetNewStreamId();

  
  
  void EnqueueSessionWrite(RequestPriority priority,
                           SpdyFrameType frame_type,
                           scoped_ptr<SpdyFrame> frame);

  
  
  void EnqueueWrite(RequestPriority priority,
                    SpdyFrameType frame_type,
                    scoped_ptr<SpdyBufferProducer> producer,
                    const base::WeakPtr<SpdyStream>& stream);

  
  void InsertCreatedStream(scoped_ptr<SpdyStream> stream);

  
  
  scoped_ptr<SpdyStream> ActivateCreatedStream(SpdyStream* stream);

  
  void InsertActivatedStream(scoped_ptr<SpdyStream> stream);

  
  
  
  void DeleteStream(scoped_ptr<SpdyStream> stream, int status);

  
  
  
  base::WeakPtr<SpdyStream> GetActivePushStream(const GURL& url);

  
  
  
  int OnInitialResponseHeadersReceived(const SpdyHeaderBlock& response_headers,
                                       base::Time response_time,
                                       base::TimeTicks recv_first_byte_time,
                                       SpdyStream* stream);

  void RecordPingRTTHistogram(base::TimeDelta duration);
  void RecordHistograms();
  void RecordProtocolErrorHistogram(SpdyProtocolErrorDetails details);

  
  
  
  void DcheckGoingAway() const;

  
  
  
  
  void DcheckClosed() const;

  
  
  
  
  
  void StartGoingAway(SpdyStreamId last_good_stream_id, Error status);

  
  
  
  void MaybeFinishGoingAway();

  
  
  
  
  
  CloseSessionResult DoCloseSession(Error err, const std::string& description);

  
  
  
  
  
  void RemoveFromPool();

  
  
  void LogAbandonedStream(SpdyStream* stream, Error status);

  
  
  void LogAbandonedActiveStream(ActiveStreamMap::const_iterator it,
                                Error status);

  
  
  void CompleteStreamRequest(
      const base::WeakPtr<SpdyStreamRequest>& pending_request);

  
  void DeleteExpiredPushedStreams();

  
  virtual void OnError(SpdyFramer::SpdyError error_code) OVERRIDE;
  virtual void OnStreamError(SpdyStreamId stream_id,
                             const std::string& description) OVERRIDE;
  virtual void OnPing(uint32 unique_id) OVERRIDE;
  virtual void OnRstStream(SpdyStreamId stream_id,
                           SpdyRstStreamStatus status) OVERRIDE;
  virtual void OnGoAway(SpdyStreamId last_accepted_stream_id,
                        SpdyGoAwayStatus status) OVERRIDE;
  virtual void OnDataFrameHeader(SpdyStreamId stream_id,
                                 size_t length,
                                 bool fin) OVERRIDE;
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len,
                                 bool fin) OVERRIDE;
  virtual void OnSettings(bool clear_persisted) OVERRIDE;
  virtual void OnSetting(
      SpdySettingsIds id, uint8 flags, uint32 value) OVERRIDE;
  virtual void OnWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size) OVERRIDE;
  virtual void OnPushPromise(SpdyStreamId stream_id,
                             SpdyStreamId promised_stream_id) OVERRIDE;
  virtual void OnSynStream(SpdyStreamId stream_id,
                           SpdyStreamId associated_stream_id,
                           SpdyPriority priority,
                           uint8 credential_slot,
                           bool fin,
                           bool unidirectional,
                           const SpdyHeaderBlock& headers) OVERRIDE;
  virtual void OnSynReply(
      SpdyStreamId stream_id,
      bool fin,
      const SpdyHeaderBlock& headers) OVERRIDE;
  virtual void OnHeaders(
      SpdyStreamId stream_id,
      bool fin,
      const SpdyHeaderBlock& headers) OVERRIDE;

  
  virtual void OnSendCompressedFrame(
      SpdyStreamId stream_id,
      SpdyFrameType type,
      size_t payload_len,
      size_t frame_len) OVERRIDE;
  virtual void OnReceiveCompressedFrame(
      SpdyStreamId stream_id,
      SpdyFrameType type,
      size_t frame_len) OVERRIDE;

  
  // that is to be written or is being written. Increases the send
  
  
  
  
  void OnWriteBufferConsumed(size_t frame_payload_size,
                             size_t consume_size,
                             SpdyBuffer::ConsumeSource consume_source);

  
  
  
  
  
  
  
  void IncreaseSendWindowSize(int32 delta_window_size);

  
  
  
  
  
  
  
  void DecreaseSendWindowSize(int32 delta_window_size);

  
  
  
  
  
  void OnReadBufferConsumed(size_t consume_size,
                            SpdyBuffer::ConsumeSource consume_source);

  
  
  
  
  
  
  
  void IncreaseRecvWindowSize(int32 delta_window_size);

  
  
  
  
  
  
  void DecreaseRecvWindowSize(int32 delta_window_size);

  
  
  void QueueSendStalledStream(const SpdyStream& stream);

  
  
  void ResumeSendStalledStreams();

  
  
  SpdyStreamId PopStreamToPossiblyResume();

  
  
  

  void set_connection_at_risk_of_loss_time(base::TimeDelta duration) {
    connection_at_risk_of_loss_time_ = duration;
  }

  void set_hung_interval(base::TimeDelta duration) {
    hung_interval_ = duration;
  }

  int64 pings_in_flight() const { return pings_in_flight_; }

  uint32 next_ping_id() const { return next_ping_id_; }

  base::TimeTicks last_activity_time() const { return last_activity_time_; }

  bool check_ping_status_pending() const { return check_ping_status_pending_; }

  size_t max_concurrent_streams() const { return max_concurrent_streams_; }

  
  
  SSLClientSocket* GetSSLClientSocket() const;

  
  
  
  
  base::WeakPtrFactory<SpdySession> weak_factory_;

  
  
  bool in_io_loop_;

  
  const SpdySessionKey spdy_session_key_;

  
  
  std::set<SpdySessionKey> pooled_aliases_;

  
  
  SpdySessionPool* pool_;
  const base::WeakPtr<HttpServerProperties> http_server_properties_;

  
  scoped_ptr<ClientSocketHandle> connection_;

  
  scoped_refptr<IOBuffer> read_buffer_;

  int stream_hi_water_mark_;  

  
  
  PendingStreamRequestQueue pending_create_stream_queues_[NUM_PRIORITIES];

  
  
  
  
  
  
  
  
  
  
  
  ActiveStreamMap active_streams_;

  
  
  
  PushedStreamMap unclaimed_pushed_streams_;

  
  
  
  CreatedStreamSet created_streams_;

  
  SpdyWriteQueue write_queue_;

  

  
  scoped_ptr<SpdyBuffer> in_flight_write_;
  
  SpdyFrameType in_flight_write_frame_type_;
  
  size_t in_flight_write_frame_size_;
  // The stream to notify when |in_flight_write_| has been written to
  
  base::WeakPtr<SpdyStream> in_flight_write_stream_;

  
  bool is_secure_;

  
  int certificate_error_code_;

  
  scoped_ptr<BufferedSpdyFramer> buffered_spdy_framer_;

  
  AvailabilityState availability_state_;
  ReadState read_state_;
  WriteState write_state_;

  
  
  
  
  Error error_on_close_;

  
  size_t max_concurrent_streams_;  
  size_t max_concurrent_streams_limit_;

  
  int streams_initiated_count_;
  int streams_pushed_count_;
  int streams_pushed_and_claimed_count_;
  int streams_abandoned_count_;

  
  
  int total_bytes_received_;

  bool sent_settings_;      
  bool received_settings_;  
                            
  int stalled_streams_;     

  
  int64 pings_in_flight_;

  
  uint32 next_ping_id_;

  
  base::TimeTicks last_ping_sent_time_;

  
  base::TimeTicks last_activity_time_;

  
  size_t last_compressed_frame_len_;

  
  
  base::TimeTicks next_unclaimed_push_stream_sweep_time_;

  
  
  bool check_ping_status_pending_;

  
  bool send_connection_header_prefix_;

  
  FlowControlState flow_control_state_;

  
  
  
  int32 stream_initial_send_window_size_;

  
  
  
  
  
  int32 stream_initial_recv_window_size_;

  
  
  int32 session_send_window_size_;
  int32 session_recv_window_size_;
  int32 session_unacked_recv_window_bytes_;

  
  
  std::deque<SpdyStreamId> stream_send_unstall_queue_[NUM_PRIORITIES];

  BoundNetLog net_log_;

  
  bool verify_domain_authentication_;
  bool enable_sending_initial_data_;
  bool enable_compression_;
  bool enable_ping_based_connection_checking_;

  
  
  NextProto protocol_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  base::TimeDelta connection_at_risk_of_loss_time_;

  
  
  
  
  
  base::TimeDelta hung_interval_;

  
  
  HostPortPair trusted_spdy_proxy_;

  TimeFunc time_func_;
};

}  

#endif  
