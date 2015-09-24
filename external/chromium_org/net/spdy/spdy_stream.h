// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_STREAM_H_
#define NET_SPDY_SPDY_STREAM_H_

#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "net/base/bandwidth_metrics.h"
#include "net/base/io_buffer.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/request_priority.h"
#include "net/socket/ssl_client_socket.h"
#include "net/spdy/spdy_buffer.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"
#include "net/ssl/server_bound_cert_service.h"
#include "net/ssl/ssl_client_cert_type.h"
#include "url/gurl.h"

namespace net {

class AddressList;
class IPEndPoint;
struct LoadTimingInfo;
class SSLCertRequestInfo;
class SSLInfo;
class SpdySession;

enum SpdyStreamType {
  
  
  SPDY_BIDIRECTIONAL_STREAM,
  
  
  SPDY_REQUEST_RESPONSE_STREAM,
  
  
  SPDY_PUSH_STREAM
};

enum SpdySendStatus {
  MORE_DATA_TO_SEND,
  NO_MORE_DATA_TO_SEND
};

enum SpdyResponseHeadersStatus {
  RESPONSE_HEADERS_ARE_INCOMPLETE,
  RESPONSE_HEADERS_ARE_COMPLETE
};

class NET_EXPORT_PRIVATE SpdyStream {
 public:
  
  class NET_EXPORT_PRIVATE Delegate {
   public:
    Delegate() {}

    
    
    virtual void OnRequestHeadersSent() = 0;

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual SpdyResponseHeadersStatus OnResponseHeadersUpdated(
        const SpdyHeaderBlock& response_headers) = 0;

    
    
    
    
    
    
    virtual void OnDataReceived(scoped_ptr<SpdyBuffer> buffer) = 0;

    
    
    virtual void OnDataSent() = 0;

    
    
    
    
    
    
    
    virtual void OnClose(int status) = 0;

   protected:
    virtual ~Delegate() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  
  SpdyStream(SpdyStreamType type,
             const base::WeakPtr<SpdySession>& session,
             const GURL& url,
             RequestPriority priority,
             int32 initial_send_window_size,
             int32 initial_recv_window_size,
             const BoundNetLog& net_log);

  ~SpdyStream();

  
  
  
  void SetDelegate(Delegate* delegate);

  
  
  void DetachDelegate();

  
  
  
  base::Time response_time() const { return response_time_; }

  SpdyStreamType type() const { return type_; }

  SpdyStreamId stream_id() const { return stream_id_; }
  void set_stream_id(SpdyStreamId stream_id) { stream_id_ = stream_id; }

  const GURL& url() const { return url_; }

  RequestPriority priority() const { return priority_; }

  int32 send_window_size() const { return send_window_size_; }

  int32 recv_window_size() const { return recv_window_size_; }

  bool send_stalled_by_flow_control() const {
    return send_stalled_by_flow_control_;
  }

  void set_send_stalled_by_flow_control(bool stalled) {
    send_stalled_by_flow_control_ = stalled;
  }

  
  
  
  
  
  
  
  
  
  
  void AdjustSendWindowSize(int32 delta_window_size);

  
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

  int GetPeerAddress(IPEndPoint* address) const;
  int GetLocalAddress(IPEndPoint* address) const;

  
  
  bool WasEverUsed() const;

  const BoundNetLog& net_log() const { return net_log_; }

  base::Time GetRequestTime() const;
  void SetRequestTime(base::Time t);

  
  
  
  
  
  int OnInitialResponseHeadersReceived(const SpdyHeaderBlock& response_headers,
                                       base::Time response_time,
                                       base::TimeTicks recv_first_byte_time);

  
  
  
  
  int OnAdditionalResponseHeadersReceived(
      const SpdyHeaderBlock& additional_response_headers);

  
  
  
  
  
  
  
  
  
  
  
  void OnDataReceived(scoped_ptr<SpdyBuffer> buffer);

  
  // completely written. |frame_size| is the total size of the frame
  
  void OnFrameWriteComplete(SpdyFrameType frame_type, size_t frame_size);

  
  
  
  
  
  void OnClose(int status);

  
  void LogStreamError(int status, const std::string& description);

  
  
  void Cancel();

  
  
  void Close();

  
  base::WeakPtr<SpdyStream> GetWeakPtr();

  

  
  

  
  
  
  
  
  
  int SendRequestHeaders(scoped_ptr<SpdyHeaderBlock> request_headers,
                         SpdySendStatus send_status);

  
  
  
  
  
  void SendData(IOBuffer* data, int length, SpdySendStatus send_status);

  
  bool GetSSLInfo(SSLInfo* ssl_info,
                  bool* was_npn_negotiated,
                  NextProto* protocol_negotiated);

  
  
  bool GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  
  
  
  
  
  void PossiblyResumeIfSendStalled();

  
  
  
  bool IsClosed() const;

  
  
  bool IsIdle() const;

  
  
  NextProto GetProtocol() const;

  int response_status() const { return response_status_; }

  void IncrementRawReceivedBytes(size_t received_bytes) {
    raw_received_bytes_ += received_bytes;
  }

  int64 raw_received_bytes() const { return raw_received_bytes_; }

  bool GetLoadTimingInfo(LoadTimingInfo* load_timing_info) const;

  
  
  
  
  
  
  
  GURL GetUrlFromHeaders() const;

  
  
  
  bool HasUrlFromHeaders() const;

  SpdyMajorVersion GetProtocolVersion() const;

 private:
  class SynStreamBufferProducer;
  class HeaderBufferProducer;

  enum State {
    STATE_NONE,
    STATE_SEND_REQUEST_HEADERS,
    STATE_SEND_REQUEST_HEADERS_COMPLETE,
    STATE_IDLE,
    STATE_CLOSED
  };

  
  int DoLoop(int result);

  
  int DoSendRequestHeaders();
  int DoSendRequestHeadersComplete();
  int DoReadHeaders();
  int DoReadHeadersComplete(int result);
  int DoOpen();

  
  
  void UpdateHistograms();

  
  
  
  void PushedStreamReplayData();

  
  
  scoped_ptr<SpdyFrame> ProduceSynStreamFrame();

  
  
  scoped_ptr<SpdyFrame> ProduceHeaderFrame(
      scoped_ptr<SpdyHeaderBlock> header_block);

  
  
  
  void QueueNextDataFrame();

  
  
  
  
  int MergeWithResponseHeaders(const SpdyHeaderBlock& new_response_headers);

  const SpdyStreamType type_;

  base::WeakPtrFactory<SpdyStream> weak_ptr_factory_;

  
  
  bool in_do_loop_;

  
  
  
  bool continue_buffering_data_;

  SpdyStreamId stream_id_;
  const GURL url_;
  const RequestPriority priority_;
  size_t slot_;

  
  bool send_stalled_by_flow_control_;
  int32 send_window_size_;
  int32 recv_window_size_;
  int32 unacked_recv_window_bytes_;

  ScopedBandwidthMetrics metrics_;

  const base::WeakPtr<SpdySession> session_;

  
  SpdyStream::Delegate* delegate_;

  
  SpdySendStatus send_status_;

  
  
  
  
  scoped_ptr<SpdyHeaderBlock> request_headers_;

  
  scoped_refptr<DrainableIOBuffer> pending_send_data_;

  
  
  base::Time request_time_;

  SpdyHeaderBlock response_headers_;
  SpdyResponseHeadersStatus response_headers_status_;
  base::Time response_time_;

  State io_state_;

  
  
  int response_status_;

  BoundNetLog net_log_;

  base::TimeTicks send_time_;
  base::TimeTicks recv_first_byte_time_;
  base::TimeTicks recv_last_byte_time_;

  
  
  int64 raw_received_bytes_;

  
  
  int send_bytes_;
  int recv_bytes_;

  
  ScopedVector<SpdyBuffer> pending_buffers_;

  std::string domain_bound_private_key_;
  std::string domain_bound_cert_;
  ServerBoundCertService::RequestHandle domain_bound_cert_request_handle_;

  
  SpdyFrameType just_completed_frame_type_;
  size_t just_completed_frame_size_;

  DISALLOW_COPY_AND_ASSIGN(SpdyStream);
};

}  

#endif  
