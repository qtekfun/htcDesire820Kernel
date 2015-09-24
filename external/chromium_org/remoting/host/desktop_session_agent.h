// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SESSION_AGENT_H_
#define REMOTING_HOST_DESKTOP_SESSION_AGENT_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_platform_file.h"
#include "remoting/host/client_session_control.h"
#include "remoting/protocol/clipboard_stub.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"
#include "third_party/webrtc/modules/desktop_capture/screen_capturer.h"

namespace IPC {
class ChannelProxy;
class Message;
}  

namespace remoting {

class AudioCapturer;
class AudioPacket;
class AutoThreadTaskRunner;
class DesktopEnvironment;
class DesktopEnvironmentFactory;
class InputInjector;
class RemoteInputFilter;
class ScreenControls;
class ScreenResolution;

namespace protocol {
class InputEventTracker;
}  

class DesktopSessionAgent
    : public base::RefCountedThreadSafe<DesktopSessionAgent>,
      public IPC::Listener,
      public webrtc::DesktopCapturer::Callback,
      public webrtc::ScreenCapturer::MouseShapeObserver,
      public ClientSessionControl {
 public:
  class Delegate {
   public:
    virtual ~Delegate();

    
    virtual DesktopEnvironmentFactory& desktop_environment_factory() = 0;

    
    
    virtual void OnNetworkProcessDisconnected() = 0;
  };

  DesktopSessionAgent(
      scoped_refptr<AutoThreadTaskRunner> audio_capture_task_runner,
      scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
      scoped_refptr<AutoThreadTaskRunner> input_task_runner,
      scoped_refptr<AutoThreadTaskRunner> io_task_runner,
      scoped_refptr<AutoThreadTaskRunner> video_capture_task_runner);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual webrtc::SharedMemory* CreateSharedMemory(size_t size) OVERRIDE;
  virtual void OnCaptureCompleted(webrtc::DesktopFrame* frame) OVERRIDE;

  
  virtual void OnCursorShapeChanged(
      webrtc::MouseCursorShape* cursor_shape) OVERRIDE;

  
  
  void InjectClipboardEvent(const protocol::ClipboardEvent& event);

  
  void ProcessAudioPacket(scoped_ptr<AudioPacket> packet);

  
  
  
  bool Start(const base::WeakPtr<Delegate>& delegate,
             IPC::PlatformFileForTransit* desktop_pipe_out);

  
  void Stop();

 protected:
  friend class base::RefCountedThreadSafe<DesktopSessionAgent>;

  virtual ~DesktopSessionAgent();

  
  virtual const std::string& client_jid() const OVERRIDE;
  virtual void DisconnectSession() OVERRIDE;
  virtual void OnLocalMouseMoved(
    const webrtc::DesktopVector& position) OVERRIDE;
  virtual void SetDisableInputs(bool disable_inputs) OVERRIDE;

  
  void OnStartSessionAgent(const std::string& authenticated_jid,
                           const ScreenResolution& resolution,
                           bool virtual_terminal);

  
  void OnCaptureFrame();

  
  void OnSharedBufferCreated(int id);

  
  void OnInjectClipboardEvent(const std::string& serialized_event);
  void OnInjectKeyEvent(const std::string& serialized_event);
  void OnInjectMouseEvent(const std::string& serialized_event);

  
  
  void SetScreenResolution(const ScreenResolution& resolution);

  
  void SendToNetwork(IPC::Message* message);

  
  void StartAudioCapturer();

  
  void StopAudioCapturer();

  
  void StartVideoCapturer();

  
  void StopVideoCapturer();

 private:
  class SharedBuffer;
  friend class SharedBuffer;

  
  void OnSharedBufferDeleted(int id);

  
  void CloseDesktopPipeHandle();

  
  scoped_refptr<AutoThreadTaskRunner> audio_capture_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> caller_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> input_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> io_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> video_capture_task_runner_;

  
  scoped_ptr<AudioCapturer> audio_capturer_;

  std::string client_jid_;

  
  base::WeakPtrFactory<ClientSessionControl> control_factory_;

  base::WeakPtr<Delegate> delegate_;

  
  scoped_ptr<DesktopEnvironment> desktop_environment_;

  
  scoped_ptr<InputInjector> input_injector_;

  
  scoped_ptr<protocol::InputEventTracker> input_tracker_;

  
  scoped_ptr<RemoteInputFilter> remote_input_filter_;

  
  scoped_ptr<ScreenControls> screen_controls_;

  
  scoped_ptr<IPC::ChannelProxy> network_channel_;

  
  
  IPC::PlatformFileForTransit desktop_pipe_;

  
  webrtc::DesktopSize current_size_;

  
  int next_shared_buffer_id_;

  
  int shared_buffers_;

  
  bool started_;

  
  scoped_ptr<webrtc::ScreenCapturer> video_capturer_;

  
  
  scoped_ptr<webrtc::DesktopFrame> last_frame_;

  DISALLOW_COPY_AND_ASSIGN(DesktopSessionAgent);
};

}  

#endif  
