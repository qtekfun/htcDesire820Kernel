// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_CLIENT_CHROMOTING_CLIENT_H_
#define REMOTING_CLIENT_CHROMOTING_CLIENT_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "remoting/client/client_config.h"
#include "remoting/client/chromoting_stats.h"
#include "remoting/protocol/client_stub.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/connection_to_host.h"
#include "remoting/protocol/input_stub.h"
#include "remoting/protocol/video_stub.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

namespace protocol {
class TransportFactory;
}  

class AudioDecodeScheduler;
class AudioPlayer;
class ClientContext;
class ClientUserInterface;
class FrameConsumerProxy;
class FrameProducer;
class RectangleUpdateDecoder;
class SignalStrategy;

class ChromotingClient : public protocol::ConnectionToHost::HostEventCallback,
                         public protocol::ClientStub {
 public:
  
  ChromotingClient(const ClientConfig& config,
                   ClientContext* client_context,
                   protocol::ConnectionToHost* connection,
                   ClientUserInterface* user_interface,
                   scoped_refptr<FrameConsumerProxy> frame_consumer,
                   scoped_ptr<AudioPlayer> audio_player);

  virtual ~ChromotingClient();

  
  
  void Start(SignalStrategy* signal_strategy,
             scoped_ptr<protocol::TransportFactory> transport_factory);

  FrameProducer* GetFrameProducer();

  
  ChromotingStats* GetStats();

  
  virtual void SetCapabilities(
      const protocol::Capabilities& capabilities) OVERRIDE;
  virtual void SetPairingResponse(
      const protocol::PairingResponse& pairing_response) OVERRIDE;
  virtual void DeliverHostMessage(
      const protocol::ExtensionMessage& message) OVERRIDE;

  
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  
  virtual void SetCursorShape(
      const protocol::CursorShapeInfo& cursor_shape) OVERRIDE;

  
  virtual void OnConnectionState(
      protocol::ConnectionToHost::State state,
      protocol::ErrorCode error) OVERRIDE;
  virtual void OnConnectionReady(bool ready) OVERRIDE;

 private:
  
  void OnAuthenticated();

  
  void OnChannelsConnected();

  
  ClientConfig config_;
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;
  protocol::ConnectionToHost* connection_;
  ClientUserInterface* user_interface_;
  scoped_refptr<RectangleUpdateDecoder> rectangle_decoder_;

  scoped_ptr<AudioDecodeScheduler> audio_decode_scheduler_;

  
  base::Closure client_done_;

  
  std::string host_capabilities_;

  
  bool host_capabilities_received_;

  
  ChromotingStats stats_;

  
  base::WeakPtr<ChromotingClient> weak_ptr_;
  base::WeakPtrFactory<ChromotingClient> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromotingClient);
};

}  

#endif  
