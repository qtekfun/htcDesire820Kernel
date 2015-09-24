// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_H_
#define NET_SPDY_SPDY_SESSION_H_
#pragma once

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "net/base/io_buffer.h"
#include "net/base/load_states.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/base/ssl_config_service.h"
#include "net/base/upload_data_stream.h"
#include "net/socket/client_socket.h"
#include "net/socket/client_socket_handle.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_io_buffer.h"
#include "net/spdy/spdy_protocol.h"
#include "net/spdy/spdy_session_pool.h"

namespace net {

const int kMss = 1430;
const int kMaxSpdyFrameChunkSize = (2 * kMss) - spdy::SpdyFrame::size();

class BoundNetLog;
class SpdySettingsStorage;
class SpdyStream;
class SSLInfo;

class SpdySession : public base::RefCounted<SpdySession>,
                    public spdy::SpdyFramerVisitorInterface {
 public:
  
  
  
  
  
  
  
  SpdySession(const HostPortProxyPair& host_port_proxy_pair,
              SpdySessionPool* spdy_session_pool,
              SpdySettingsStorage* spdy_settings,
              NetLog* net_log);

  const HostPortPair& host_port_pair() const {
    return host_port_proxy_pair_.first;
  }
  const HostPortProxyPair& host_port_proxy_pair() const {
    return host_port_proxy_pair_;
  }

  
  
  
  
  
  int GetPushStream(
      const GURL& url,
      scoped_refptr<SpdyStream>* spdy_stream,
      const BoundNetLog& stream_net_log);

  
  
  int CreateStream(
      const GURL& url,
      RequestPriority priority,
      scoped_refptr<SpdyStream>* spdy_stream,
      const BoundNetLog& stream_net_log,
      CompletionCallback* callback);

  
  void CancelPendingCreateStreams(const scoped_refptr<SpdyStream>* spdy_stream);

  
  
  
  
  net::Error InitializeWithSocket(ClientSocketHandle* connection,
                                  bool is_secure,
                                  int certificate_error_code);

  
  
  
  
  
  bool VerifyDomainAuthentication(const std::string& domain);

  
  
  int WriteSynStream(
      spdy::SpdyStreamId stream_id,
      RequestPriority priority,
      spdy::SpdyControlFlags flags,
      const linked_ptr<spdy::SpdyHeaderBlock>& headers);

  
  
  int WriteStreamData(spdy::SpdyStreamId stream_id, net::IOBuffer* data,
                      int len,
                      spdy::SpdyDataFlags flags);

  
  void CloseStream(spdy::SpdyStreamId stream_id, int status);

  
  
  
  void ResetStream(spdy::SpdyStreamId stream_id, spdy::SpdyStatusCodes status);

  
  bool IsStreamActive(spdy::SpdyStreamId stream_id) const;

  
  
  LoadState GetLoadState() const;

  
  bool GetSSLInfo(SSLInfo* ssl_info, bool* was_npn_negotiated);

  
  
  bool GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  
  static void SetSSLMode(bool enable) { use_ssl_ = enable; }
  static bool SSLMode() { return use_ssl_; }

  
  static void set_flow_control(bool enable) { use_flow_control_ = enable; }
  static bool flow_control() { return use_flow_control_; }

  
  static void set_max_concurrent_streams(size_t value) {
    max_concurrent_stream_limit_ = value;
  }
  static size_t max_concurrent_streams() {
      return max_concurrent_stream_limit_;
  }

  
  static void set_enable_ping_based_connection_checking(bool enable) {
    enable_ping_based_connection_checking_ = enable;
  }
  static bool enable_ping_based_connection_checking() {
    return enable_ping_based_connection_checking_;
  }

  
  
  void SendWindowUpdate(spdy::SpdyStreamId stream_id, int delta_window_size);

  
  bool IsClosed() const { return state_ == CLOSED; }

  
  
  
  
  
  
  void CloseSessionOnError(net::Error err, bool remove_from_pool);

  
  
  Value* GetInfoAsValue() const;

  
  
  bool IsReused() const {
    return frames_received_ > 0;
  }

  
  
  bool WasEverUsed() const {
    return connection_->socket()->WasEverUsed();
  }

  void set_spdy_session_pool(SpdySessionPool* pool) {
    spdy_session_pool_ = NULL;
  }

  
  bool is_active() const {
    return !active_streams_.empty();
  }

  
  
  size_t num_active_streams() const { return active_streams_.size(); }
  size_t num_unclaimed_pushed_streams() const {
      return unclaimed_pushed_streams_.size();
  }

  const BoundNetLog& net_log() const { return net_log_; }

  int GetPeerAddress(AddressList* address) const;
  int GetLocalAddress(IPEndPoint* address) const;

 private:
  friend class base::RefCounted<SpdySession>;
  
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, Ping);
  FRIEND_TEST_ALL_PREFIXES(SpdySessionTest, GetActivePushStream);

  struct PendingCreateStream {
    PendingCreateStream(const GURL& url, RequestPriority priority,
                        scoped_refptr<SpdyStream>* spdy_stream,
                        const BoundNetLog& stream_net_log,
                        CompletionCallback* callback)
        : url(&url), priority(priority), spdy_stream(spdy_stream),
          stream_net_log(&stream_net_log), callback(callback) { }

    const GURL* url;
    RequestPriority priority;
    scoped_refptr<SpdyStream>* spdy_stream;
    const BoundNetLog* stream_net_log;
    CompletionCallback* callback;
  };
  typedef std::queue<PendingCreateStream, std::list< PendingCreateStream> >
      PendingCreateStreamQueue;
  typedef std::map<int, scoped_refptr<SpdyStream> > ActiveStreamMap;
  
  typedef std::map<std::string, scoped_refptr<SpdyStream> > PushedStreamMap;
  typedef std::priority_queue<SpdyIOBuffer> OutputQueue;

  struct CallbackResultPair {
    CallbackResultPair() : callback(NULL), result(OK) {}
    CallbackResultPair(CompletionCallback* callback_in, int result_in)
        : callback(callback_in), result(result_in) {}

    CompletionCallback* callback;
    int result;
  };

  typedef std::map<const scoped_refptr<SpdyStream>*, CallbackResultPair>
      PendingCallbackMap;

  enum State {
    IDLE,
    CONNECTING,
    CONNECTED,
    CLOSED
  };

  enum { kDefaultMaxConcurrentStreams = 10 };

  virtual ~SpdySession();

  void ProcessPendingCreateStreams();
  int CreateStreamImpl(
      const GURL& url,
      RequestPriority priority,
      scoped_refptr<SpdyStream>* spdy_stream,
      const BoundNetLog& stream_net_log);

  
  void OnSyn(const spdy::SpdySynStreamControlFrame& frame,
             const linked_ptr<spdy::SpdyHeaderBlock>& headers);
  void OnSynReply(const spdy::SpdySynReplyControlFrame& frame,
                  const linked_ptr<spdy::SpdyHeaderBlock>& headers);
  void OnHeaders(const spdy::SpdyHeadersControlFrame& frame,
                 const linked_ptr<spdy::SpdyHeaderBlock>& headers);
  void OnRst(const spdy::SpdyRstStreamControlFrame& frame);
  void OnGoAway(const spdy::SpdyGoAwayControlFrame& frame);
  void OnPing(const spdy::SpdyPingControlFrame& frame);
  void OnSettings(const spdy::SpdySettingsControlFrame& frame);
  void OnWindowUpdate(const spdy::SpdyWindowUpdateControlFrame& frame);

  
  void OnReadComplete(int result);
  void OnWriteComplete(int result);

  
  void SendSettings();

  
  
  void HandleSettings(const spdy::SpdySettings& settings);

  
  void SendPrefacePingIfNoneInFlight();

  
  void SendPrefacePing();

  
  
  void PlanToSendTrailingPing();

  
  
  void SendTrailingPing();

  
  void WritePingFrame(uint32 unique_id);

  
  
  void PlanToCheckPingStatus();

  
  
  void CheckPingStatus(base::TimeTicks last_check_time);

  
  
  net::Error ReadSocket();

  
  void WriteSocketLater();
  void WriteSocket();

  
  int GetNewStreamId();

  
  
  
  
  void QueueFrame(spdy::SpdyFrame* frame, spdy::SpdyPriority priority,
                  SpdyStream* stream);

  
  void ActivateStream(SpdyStream* stream);
  void DeleteStream(spdy::SpdyStreamId id, int status);

  
  void RemoveFromPool();

  
  
  
  scoped_refptr<SpdyStream> GetActivePushStream(const std::string& url);

  
  
  bool Respond(const spdy::SpdyHeaderBlock& headers,
               const scoped_refptr<SpdyStream> stream);

  void RecordHistograms();

  
  void CloseAllStreams(net::Error status);

  
  
  void InvokeUserStreamCreationCallback(scoped_refptr<SpdyStream>* stream);

  
  virtual void OnError(spdy::SpdyFramer*);
  virtual void OnStreamFrameData(spdy::SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len);
  virtual void OnControl(const spdy::SpdyControlFrame* frame);

  virtual bool OnControlFrameHeaderData(spdy::SpdyStreamId stream_id,
                                        const char* header_data,
                                        size_t len);

  virtual void OnDataFrameHeader(const spdy::SpdyDataFrame* frame);

  
  
  
  static void set_connection_at_risk_of_loss_ms(int duration) {
    connection_at_risk_of_loss_ms_ = duration;
  }
  static int connection_at_risk_of_loss_ms() {
    return connection_at_risk_of_loss_ms_;
  }

  static void set_trailing_ping_delay_time_ms(int duration) {
    trailing_ping_delay_time_ms_ = duration;
  }
  static int trailing_ping_delay_time_ms() {
    return trailing_ping_delay_time_ms_;
  }

  static void set_hung_interval_ms(int duration) {
    hung_interval_ms_ = duration;
  }
  static int hung_interval_ms() {
    return hung_interval_ms_;
  }

  int64 pings_in_flight() const { return pings_in_flight_; }

  uint32 next_ping_id() const { return next_ping_id_; }

  base::TimeTicks received_data_time() const { return received_data_time_; }

  bool trailing_ping_pending() const { return trailing_ping_pending_; }

  bool check_ping_status_pending() const { return check_ping_status_pending_; }

  
  CompletionCallbackImpl<SpdySession> read_callback_;
  CompletionCallbackImpl<SpdySession> write_callback_;

  
  
  
  
  ScopedRunnableMethodFactory<SpdySession> method_factory_;

  
  
  
  PendingCallbackMap pending_callback_map_;

  
  const HostPortProxyPair host_port_proxy_pair_;

  
  
  SpdySessionPool* spdy_session_pool_;
  SpdySettingsStorage* const spdy_settings_;

  
  scoped_ptr<ClientSocketHandle> connection_;

  
  scoped_refptr<IOBuffer> read_buffer_;
  bool read_pending_;

  int stream_hi_water_mark_;  

  
  
  PendingCreateStreamQueue create_stream_queues_[NUM_PRIORITIES];

  
  
  
  
  
  
  
  
  ActiveStreamMap active_streams_;
  
  
  PushedStreamMap unclaimed_pushed_streams_;

  
  OutputQueue queue_;

  
  bool write_pending_;            
  SpdyIOBuffer in_flight_write_;  

  
  bool delayed_write_pending_;

  
  bool is_secure_;

  
  int certificate_error_code_;

  
  spdy::SpdyFramer spdy_framer_;

  
  
  
  net::Error error_;
  State state_;

  
  size_t max_concurrent_streams_;  

  
  int streams_initiated_count_;
  int streams_pushed_count_;
  int streams_pushed_and_claimed_count_;
  int streams_abandoned_count_;
  int frames_received_;
  int bytes_received_;
  bool sent_settings_;      
  bool received_settings_;  
                            
  int stalled_streams_;     

  
  int64 pings_in_flight_;

  
  uint32 next_ping_id_;

  
  base::TimeTicks received_data_time_;

  
  
  bool trailing_ping_pending_;

  
  
  bool check_ping_status_pending_;

  
  
  
  bool need_to_send_ping_;

  
  
  
  int initial_send_window_size_;

  
  
  
  
  int initial_recv_window_size_;

  BoundNetLog net_log_;

  static bool use_ssl_;
  static bool use_flow_control_;
  static size_t max_concurrent_stream_limit_;

  
  static bool enable_ping_based_connection_checking_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  static int connection_at_risk_of_loss_ms_;

  
  
  
  
  
  static int trailing_ping_delay_time_ms_;

  
  
  
  static int hung_interval_ms_;
};

class NetLogSpdySynParameter : public NetLog::EventParameters {
 public:
  NetLogSpdySynParameter(const linked_ptr<spdy::SpdyHeaderBlock>& headers,
                         spdy::SpdyControlFlags flags,
                         spdy::SpdyStreamId id,
                         spdy::SpdyStreamId associated_stream);

  const linked_ptr<spdy::SpdyHeaderBlock>& GetHeaders() const {
    return headers_;
  }

  virtual Value* ToValue() const;

 private:
  virtual ~NetLogSpdySynParameter();

  const linked_ptr<spdy::SpdyHeaderBlock> headers_;
  const spdy::SpdyControlFlags flags_;
  const spdy::SpdyStreamId id_;
  const spdy::SpdyStreamId associated_stream_;

  DISALLOW_COPY_AND_ASSIGN(NetLogSpdySynParameter);
};

}  

#endif  
