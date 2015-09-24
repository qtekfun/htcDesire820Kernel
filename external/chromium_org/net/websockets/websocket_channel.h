// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_CHANNEL_H_
#define NET_WEBSOCKETS_WEBSOCKET_CHANNEL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"  
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/net_export.h"
#include "net/websockets/websocket_event_interface.h"
#include "net/websockets/websocket_frame.h"
#include "net/websockets/websocket_stream.h"
#include "url/gurl.h"

namespace net {

class BoundNetLog;
class IOBuffer;
class URLRequestContext;

class NET_EXPORT WebSocketChannel {
 public:
  
  
  typedef base::Callback<scoped_ptr<WebSocketStreamRequest>(
      const GURL&,
      const std::vector<std::string>&,
      const GURL&,
      URLRequestContext*,
      const BoundNetLog&,
      scoped_ptr<WebSocketStream::ConnectDelegate>)> WebSocketStreamCreator;

  
  
  
  WebSocketChannel(scoped_ptr<WebSocketEventInterface> event_interface,
                   URLRequestContext* url_request_context);
  virtual ~WebSocketChannel();

  
  void SendAddChannelRequest(
      const GURL& socket_url,
      const std::vector<std::string>& requested_protocols,
      const GURL& origin);

  
  
  
  
  
  
  
  
  
  
  
  void SendFrame(bool fin,
                 WebSocketFrameHeader::OpCode op_code,
                 const std::vector<char>& data);

  
  
  
  void SendFlowControl(int64 quota);

  
  
  
  
  
  
  
  
  
  void StartClosingHandshake(uint16 code, const std::string& reason);

  
  
  void SendAddChannelRequestForTesting(
      const GURL& socket_url,
      const std::vector<std::string>& requested_protocols,
      const GURL& origin,
      const WebSocketStreamCreator& creator);

  
  
  
  void SetClosingHandshakeTimeoutForTesting(base::TimeDelta delay);

 private:
  
  
  
  typedef WebSocketEventInterface::ChannelState ChannelState;

  
  
  
  enum State {
    FRESHLY_CONSTRUCTED,
    CONNECTING,
    CONNECTED,
    SEND_CLOSED,  
    RECV_CLOSED,  
                  
                  
    CLOSE_WAIT,   
                  
    CLOSED,       
                  
  };

  
  
  
  enum ExposeError {
    SEND_REAL_ERROR,
    SEND_GOING_AWAY,
  };

  
  
  
  
  
  
  class ConnectDelegate;

  
  void SendAddChannelRequestWithSuppliedCreator(
      const GURL& socket_url,
      const std::vector<std::string>& requested_protocols,
      const GURL& origin,
      const WebSocketStreamCreator& creator);

  
  
  void OnConnectSuccess(scoped_ptr<WebSocketStream> stream);

  
  
  void OnConnectFailure(uint16 websocket_error);

  
  bool InClosingState() const;

  
  ChannelState WriteFrames() WARN_UNUSED_RESULT;

  
  
  
  
  
  ChannelState OnWriteDone(bool synchronous, int result) WARN_UNUSED_RESULT;

  
  ChannelState ReadFrames() WARN_UNUSED_RESULT;

  
  
  
  
  ChannelState OnReadDone(bool synchronous, int result) WARN_UNUSED_RESULT;

  
  ChannelState ProcessFrame(
      scoped_ptr<WebSocketFrame> frame) WARN_UNUSED_RESULT;

  
  
  
  ChannelState HandleFrame(const WebSocketFrameHeader::OpCode opcode,
                           bool final,
                           const scoped_refptr<IOBuffer>& data_buffer,
                           size_t size) WARN_UNUSED_RESULT;

  
  
  
  
  
  ChannelState SendIOBuffer(bool fin,
                            WebSocketFrameHeader::OpCode op_code,
                            const scoped_refptr<IOBuffer>& buffer,
                            size_t size) WARN_UNUSED_RESULT;

  
  
  
  
  
  
  
  
  ChannelState FailChannel(ExposeError expose,
                           uint16 code,
                           const std::string& reason) WARN_UNUSED_RESULT;

  
  
  
  
  ChannelState SendClose(uint16 code,
                         const std::string& reason) WARN_UNUSED_RESULT;

  
  
  
  
  
  void ParseClose(const scoped_refptr<IOBuffer>& buffer,
                  size_t size,
                  uint16* code,
                  std::string* reason);

  
  
  void CloseTimeout();

  
  GURL socket_url_;

  
  const scoped_ptr<WebSocketEventInterface> event_interface_;

  
  URLRequestContext* const url_request_context_;

  
  scoped_ptr<WebSocketStream> stream_;

  
  
  class SendBuffer;
  
  scoped_ptr<SendBuffer> data_being_sent_;
  
  
  scoped_ptr<SendBuffer> data_to_send_next_;

  
  ScopedVector<WebSocketFrame> read_frames_;

  
  
  scoped_ptr<WebSocketStreamRequest> stream_request_;

  
  
  
  int send_quota_low_water_mark_;
  
  
  int send_quota_high_water_mark_;
  
  
  int current_send_quota_;

  
  base::OneShotTimer<WebSocketChannel> timer_;

  
  base::TimeDelta timeout_;

  
  
  
  uint16 closing_code_;
  std::string closing_reason_;

  
  
  State state_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketChannel);
};

}  

#endif  
