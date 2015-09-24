// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_STREAM_H_
#define NET_SPDY_SPDY_STREAM_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "net/base/bandwidth_metrics.h"
#include "net/base/io_buffer.h"
#include "net/base/upload_data.h"
#include "net/base/net_log.h"
#include "net/spdy/spdy_framer.h"
#include "net/spdy/spdy_protocol.h"

namespace net {

class AddressList;
class IPEndPoint;
class SpdySession;
class SSLCertRequestInfo;
class SSLInfo;

class SpdyStream
    : public base::RefCounted<SpdyStream>,
      public ChunkCallback {
 public:
  
  class Delegate {
   public:
    Delegate() {}

    
    
    virtual bool OnSendHeadersComplete(int status) = 0;

    
    
    virtual int OnSendBody() = 0;

    
    
    
    
    virtual int OnSendBodyComplete(int status, bool* eof) = 0;

    
    
    
    
    
    
    virtual int OnResponseReceived(const spdy::SpdyHeaderBlock& response,
                                   base::Time response_time,
                                   int status) = 0;

    
    virtual void OnDataReceived(const char* data, int length) = 0;

    
    virtual void OnDataSent(int length) = 0;

    
    virtual void OnClose(int status) = 0;

    
    virtual void set_chunk_callback(ChunkCallback* callback) = 0;

   protected:
    friend class base::RefCounted<Delegate>;
    virtual ~Delegate() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  
  SpdyStream(SpdySession* session,
             spdy::SpdyStreamId stream_id,
             bool pushed,
             const BoundNetLog& net_log);

  
  
  
  void SetDelegate(Delegate* delegate);
  Delegate* GetDelegate() { return delegate_; }

  
  
  void DetachDelegate();

  
  bool pushed() const { return pushed_; }

  spdy::SpdyStreamId stream_id() const { return stream_id_; }
  void set_stream_id(spdy::SpdyStreamId stream_id) { stream_id_ = stream_id; }

  bool response_received() const { return response_received_; }
  void set_response_received() { response_received_ = true; }

  
  const std::string& path() const { return path_; }
  void set_path(const std::string& path) { path_ = path; }

  int priority() const { return priority_; }
  void set_priority(int priority) { priority_ = priority; }

  int send_window_size() const { return send_window_size_; }
  void set_send_window_size(int window_size) {
    send_window_size_ = window_size;
  }

  int recv_window_size() const { return recv_window_size_; }
  void set_recv_window_size(int window_size) {
    recv_window_size_ = window_size;
  }

  void set_stalled_by_flow_control(bool stalled) {
    stalled_by_flow_control_ = stalled;
  }

  
  
  
  void IncreaseSendWindowSize(int delta_window_size);

  
  void DecreaseSendWindowSize(int delta_window_size);

  int GetPeerAddress(AddressList* address) const;
  int GetLocalAddress(IPEndPoint* address) const;

  
  
  bool WasEverUsed() const;

  
  
  void IncreaseRecvWindowSize(int delta_window_size);

  
  
  
  
  void DecreaseRecvWindowSize(int delta_window_size);

  const BoundNetLog& net_log() const { return net_log_; }

  const linked_ptr<spdy::SpdyHeaderBlock>& spdy_headers() const;
  void set_spdy_headers(const linked_ptr<spdy::SpdyHeaderBlock>& headers);
  base::Time GetRequestTime() const;
  void SetRequestTime(base::Time t);

  
  
  int OnResponseReceived(const spdy::SpdyHeaderBlock& response);

  
  
  int OnHeaders(const spdy::SpdyHeaderBlock& headers);

  
  
  
  
  
  
  
  void OnDataReceived(const char* buffer, int bytes);

  
  
  
  // |result| is the number of bytes written or a net error code.
  void OnWriteComplete(int bytes);

  
  
  
  
  
  void OnClose(int status);

  void Cancel();
  bool cancelled() const { return cancelled_; }
  bool closed() const { return io_state_ == STATE_DONE; }

  

  
  
  int SendRequest(bool has_upload_data);

  
  int WriteStreamData(IOBuffer* data, int length,
                      spdy::SpdyDataFlags flags);

  
  bool GetSSLInfo(SSLInfo* ssl_info, bool* was_npn_negotiated);

  
  
  bool GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);

  bool is_idle() const {
    return io_state_ == STATE_OPEN || io_state_ == STATE_DONE;
  }

  int response_status() const { return response_status_; }

  
  bool HasUrl() const;

  
  
  GURL GetUrl() const;

  
  virtual void OnChunkAvailable();

 private:
  enum State {
    STATE_NONE,
    STATE_SEND_HEADERS,
    STATE_SEND_HEADERS_COMPLETE,
    STATE_SEND_BODY,
    STATE_SEND_BODY_COMPLETE,
    STATE_WAITING_FOR_RESPONSE,
    STATE_OPEN,
    STATE_DONE
  };

  friend class base::RefCounted<SpdyStream>;
  virtual ~SpdyStream();

  
  int DoLoop(int result);

  
  int DoSendHeaders();
  int DoSendHeadersComplete(int result);
  int DoSendBody();
  int DoSendBodyComplete(int result);
  int DoReadHeaders();
  int DoReadHeadersComplete(int result);
  int DoOpen(int result);

  
  
  void UpdateHistograms();

  
  
  void PushedStreamReplayData();

  
  
  
  bool continue_buffering_data_;

  spdy::SpdyStreamId stream_id_;
  std::string path_;
  int priority_;

  
  bool stalled_by_flow_control_;
  int send_window_size_;
  int recv_window_size_;

  const bool pushed_;
  ScopedBandwidthMetrics metrics_;
  bool response_received_;

  scoped_refptr<SpdySession> session_;

  
  SpdyStream::Delegate* delegate_;

  
  linked_ptr<spdy::SpdyHeaderBlock> request_;

  
  
  base::Time request_time_;

  linked_ptr<spdy::SpdyHeaderBlock> response_;
  base::Time response_time_;

  State io_state_;

  
  
  int response_status_;

  bool cancelled_;
  bool has_upload_data_;

  BoundNetLog net_log_;

  base::TimeTicks send_time_;
  base::TimeTicks recv_first_byte_time_;
  base::TimeTicks recv_last_byte_time_;
  int send_bytes_;
  int recv_bytes_;
  
  std::vector<scoped_refptr<IOBufferWithSize> > pending_buffers_;

  DISALLOW_COPY_AND_ASSIGN(SpdyStream);
};

}  

#endif  
