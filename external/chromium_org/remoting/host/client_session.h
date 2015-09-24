// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CLIENT_SESSION_H_
#define REMOTING_HOST_CLIENT_SESSION_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "remoting/host/client_session_control.h"
#include "remoting/host/mouse_clamping_filter.h"
#include "remoting/host/remote_input_filter.h"
#include "remoting/protocol/clipboard_echo_filter.h"
#include "remoting/protocol/clipboard_filter.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/connection_to_client.h"
#include "remoting/protocol/host_stub.h"
#include "remoting/protocol/input_event_tracker.h"
#include "remoting/protocol/input_filter.h"
#include "remoting/protocol/input_stub.h"
#include "remoting/protocol/pairing_registry.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace remoting {

class AudioEncoder;
class AudioScheduler;
class DesktopEnvironment;
class DesktopEnvironmentFactory;
class InputInjector;
class ScreenControls;
class VideoEncoder;
class VideoScheduler;

class ClientSession
    : public base::NonThreadSafe,
      public protocol::HostStub,
      public protocol::ConnectionToClient::EventHandler,
      public ClientSessionControl {
 public:
  
  class EventHandler {
   public:
    
    
    virtual bool OnSessionAuthenticated(ClientSession* client) = 0;

    
    virtual void OnSessionChannelsConnected(ClientSession* client) = 0;

    
    
    
    virtual void OnSessionAuthenticationFailed(ClientSession* client) = 0;

    
    virtual void OnSessionClosed(ClientSession* client) = 0;

    
    
    virtual void OnSessionSequenceNumber(ClientSession* client,
                                         int64 sequence_number) = 0;

    
    
    virtual void OnSessionRouteChange(
        ClientSession* client,
        const std::string& channel_name,
        const protocol::TransportRoute& route) = 0;

   protected:
    virtual ~EventHandler() {}
  };

  
  ClientSession(
      EventHandler* event_handler,
      scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> video_encode_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> network_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner,
      scoped_ptr<protocol::ConnectionToClient> connection,
      DesktopEnvironmentFactory* desktop_environment_factory,
      const base::TimeDelta& max_duration,
      scoped_refptr<protocol::PairingRegistry> pairing_registry);
  virtual ~ClientSession();

  
  virtual void NotifyClientResolution(
      const protocol::ClientResolution& resolution) OVERRIDE;
  virtual void ControlVideo(
      const protocol::VideoControl& video_control) OVERRIDE;
  virtual void ControlAudio(
      const protocol::AudioControl& audio_control) OVERRIDE;
  virtual void SetCapabilities(
      const protocol::Capabilities& capabilities) OVERRIDE;
  virtual void RequestPairing(
      const remoting::protocol::PairingRequest& pairing_request) OVERRIDE;
  virtual void DeliverClientMessage(
      const protocol::ExtensionMessage& message) OVERRIDE;

  
  virtual void OnConnectionAuthenticated(
      protocol::ConnectionToClient* connection) OVERRIDE;
  virtual void OnConnectionChannelsConnected(
      protocol::ConnectionToClient* connection) OVERRIDE;
  virtual void OnConnectionClosed(protocol::ConnectionToClient* connection,
                                  protocol::ErrorCode error) OVERRIDE;
  virtual void OnSequenceNumberUpdated(
      protocol::ConnectionToClient* connection, int64 sequence_number) OVERRIDE;
  virtual void OnRouteChange(
      protocol::ConnectionToClient* connection,
      const std::string& channel_name,
      const protocol::TransportRoute& route) OVERRIDE;

  
  virtual const std::string& client_jid() const OVERRIDE;
  virtual void DisconnectSession() OVERRIDE;
  virtual void OnLocalMouseMoved(
      const webrtc::DesktopVector& position) OVERRIDE;
  virtual void SetDisableInputs(bool disable_inputs) OVERRIDE;

  protocol::ConnectionToClient* connection() const {
    return connection_.get();
  }

  bool is_authenticated() { return auth_input_filter_.enabled();  }

 private:
  
  scoped_ptr<protocol::ClipboardStub> CreateClipboardProxy();

  
  static scoped_ptr<AudioEncoder> CreateAudioEncoder(
      const protocol::SessionConfig& config);

  
  static scoped_ptr<VideoEncoder> CreateVideoEncoder(
      const protocol::SessionConfig& config);

  EventHandler* event_handler_;

  
  scoped_ptr<protocol::ConnectionToClient> connection_;

  std::string client_jid_;

  
  
  base::WeakPtrFactory<ClientSessionControl> control_factory_;

  
  DesktopEnvironmentFactory* desktop_environment_factory_;

  
  scoped_ptr<DesktopEnvironment> desktop_environment_;

  
  protocol::InputFilter host_input_filter_;

  
  protocol::InputEventTracker input_tracker_;

  
  RemoteInputFilter remote_input_filter_;

  
  MouseClampingFilter mouse_clamping_filter_;

  
  
  
  protocol::ClipboardEchoFilter clipboard_echo_filter_;

  
  protocol::InputFilter disable_input_filter_;
  protocol::ClipboardFilter disable_clipboard_filter_;

  
  protocol::InputFilter auth_input_filter_;
  protocol::ClipboardFilter auth_clipboard_filter_;

  
  
  
  base::WeakPtrFactory<protocol::ClipboardStub> client_clipboard_factory_;

  
  
  base::TimeDelta max_duration_;

  
  
  base::OneShotTimer<ClientSession> max_duration_timer_;

  scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> input_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> video_encode_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> network_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> ui_task_runner_;

  
  scoped_refptr<AudioScheduler> audio_scheduler_;
  scoped_refptr<VideoScheduler> video_scheduler_;

  
  scoped_ptr<std::string> client_capabilities_;

  
  std::string host_capabilities_;

  
  scoped_ptr<InputInjector> input_injector_;

  
  scoped_ptr<ScreenControls> screen_controls_;

  
  scoped_refptr<protocol::PairingRegistry> pairing_registry_;

  DISALLOW_COPY_AND_ASSIGN(ClientSession);
};

}  

#endif  
