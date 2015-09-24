// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_MEDIA_CAST_SESSION_DELEGATE_H_
#define CHROME_RENDERER_MEDIA_CAST_SESSION_DELEGATE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"
#include "base/time/default_tick_clock.h"
#include "chrome/renderer/media/cast_session.h"
#include "content/public/renderer/p2p_socket_client.h"
#include "content/public/renderer/p2p_socket_client_delegate.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_sender.h"

namespace base {
class MessageLoopProxy;
}  

namespace media {
class VideoFrame;

namespace cast {
class CastEnvironment;
class FrameInput;
}  
}  

class CastSessionDelegate
    : public media::cast::PacketSender,
      public content::P2PSocketClientDelegate {
 public:
  typedef
  base::Callback<void(const scoped_refptr<media::cast::FrameInput>&)>
  FrameInputAvailableCallback;

  CastSessionDelegate();
  virtual ~CastSessionDelegate();

  void SetSocketFactory(
      scoped_ptr<CastSession::P2PSocketFactory> socket_factory,
      const net::IPEndPoint& remote_address);

  
  
  void StartAudio(const media::cast::AudioSenderConfig& config,
                  const FrameInputAvailableCallback& callback);
  void StartVideo(const media::cast::VideoSenderConfig& config,
                  const FrameInputAvailableCallback& callback);

 private:
  
  
  void StartSending();

  
  void MaybeStartSending();

  
  virtual bool SendPacket(const media::cast::Packet& packet) OVERRIDE;
  virtual bool SendPackets(const media::cast::PacketList& packets) OVERRIDE;

  
  virtual void OnOpen(const net::IPEndPoint& address) OVERRIDE;
  virtual void OnIncomingTcpConnection(
      const net::IPEndPoint& address,
      content::P2PSocketClient* client) OVERRIDE;
  virtual void OnSendComplete() OVERRIDE;
  virtual void OnError() OVERRIDE;
  virtual void OnDataReceived(const net::IPEndPoint& address,
                              const std::vector<char>& data,
                              const base::TimeTicks& timestamp) OVERRIDE;
 private:
  base::ThreadChecker thread_checker_;
  scoped_refptr<media::cast::CastEnvironment> cast_environment_;
  scoped_ptr<media::cast::CastSender> cast_sender_;
  scoped_ptr<CastSession::P2PSocketFactory> socket_factory_;
  net::IPEndPoint remote_address_;
  scoped_refptr<content::P2PSocketClient> socket_;

  
  
  
  
  base::Thread audio_encode_thread_;
  base::Thread video_encode_thread_;

  
  base::DefaultTickClock clock_;

  
  media::cast::AudioSenderConfig audio_config_;
  media::cast::VideoSenderConfig video_config_;
  bool audio_configured_;
  bool video_configured_;
  std::vector<FrameInputAvailableCallback> frame_input_available_callbacks_;

  
  scoped_refptr<base::MessageLoopProxy> io_message_loop_proxy_;

  DISALLOW_COPY_AND_ASSIGN(CastSessionDelegate);
};

#endif  
