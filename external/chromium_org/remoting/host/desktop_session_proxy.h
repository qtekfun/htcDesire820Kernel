// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_SESSION_PROXY_H_
#define REMOTING_HOST_DESKTOP_SESSION_PROXY_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/process/process.h"
#include "base/sequenced_task_runner_helpers.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_platform_file.h"
#include "remoting/host/audio_capturer.h"
#include "remoting/host/desktop_environment.h"
#include "remoting/host/screen_resolution.h"
#include "remoting/proto/event.pb.h"
#include "remoting/protocol/clipboard_stub.h"
#include "third_party/webrtc/modules/desktop_capture/screen_capturer.h"

namespace base {
class SingleThreadTaskRunner;
}  

namespace IPC {
class ChannelProxy;
class Message;
}  

struct SerializedDesktopFrame;

namespace remoting {

class AudioPacket;
class ClientSession;
class ClientSessionControl;
class DesktopSessionConnector;
struct DesktopSessionProxyTraits;
class IpcAudioCapturer;
class IpcVideoFrameCapturer;
class ScreenControls;

class DesktopSessionProxy
    : public base::RefCountedThreadSafe<DesktopSessionProxy,
                                        DesktopSessionProxyTraits>,
      public IPC::Listener {
 public:
  DesktopSessionProxy(
      scoped_refptr<base::SingleThreadTaskRunner> audio_capture_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
      scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner,
      base::WeakPtr<ClientSessionControl> client_session_control,
      base::WeakPtr<DesktopSessionConnector> desktop_session_connector,
      bool virtual_terminal);

  
  scoped_ptr<AudioCapturer> CreateAudioCapturer();
  scoped_ptr<InputInjector> CreateInputInjector();
  scoped_ptr<ScreenControls> CreateScreenControls();
  scoped_ptr<webrtc::ScreenCapturer> CreateVideoCapturer();
  std::string GetCapabilities() const;
  void SetCapabilities(const std::string& capabilities);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  bool AttachToDesktop(base::ProcessHandle desktop_process,
                       IPC::PlatformFileForTransit desktop_pipe);

  
  
  void DetachFromDesktop();

  
  void DisconnectSession();

  
  
  void SetAudioCapturer(const base::WeakPtr<IpcAudioCapturer>& audio_capturer);

  
  
  void CaptureFrame();

  
  
  void SetVideoCapturer(
      const base::WeakPtr<IpcVideoFrameCapturer> video_capturer);

  
  void InjectClipboardEvent(const protocol::ClipboardEvent& event);
  void InjectKeyEvent(const protocol::KeyEvent& event);
  void InjectMouseEvent(const protocol::MouseEvent& event);
  void StartInputInjector(scoped_ptr<protocol::ClipboardStub> client_clipboard);

  
  void SetScreenResolution(const ScreenResolution& resolution);

 private:
  friend class base::DeleteHelper<DesktopSessionProxy>;
  friend struct DesktopSessionProxyTraits;

  class IpcSharedBufferCore;
  class IpcSharedBuffer;
  typedef std::map<int, scoped_refptr<IpcSharedBufferCore> > SharedBuffers;

  virtual ~DesktopSessionProxy();

  
  scoped_refptr<IpcSharedBufferCore> GetSharedBufferCore(int id);

  
  void OnAudioPacket(const std::string& serialized_packet);

  
  void OnCreateSharedBuffer(int id,
                            IPC::PlatformFileForTransit handle,
                            uint32 size);

  
  void OnReleaseSharedBuffer(int id);

  
  void OnCaptureCompleted(const SerializedDesktopFrame& serialized_frame);

  
  void OnCursorShapeChanged(const webrtc::MouseCursorShape& cursor_shape);

  
  void OnInjectClipboardEvent(const std::string& serialized_event);

  
  
  void PostCaptureCompleted(scoped_ptr<webrtc::DesktopFrame> frame);

  
  
  void PostCursorShape(scoped_ptr<webrtc::MouseCursorShape> cursor_shape);

  
  
  void SendToDesktop(IPC::Message* message);

  
  
  
  
  
  
  scoped_refptr<base::SingleThreadTaskRunner> audio_capture_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> caller_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner_;
  scoped_refptr<base::SingleThreadTaskRunner> video_capture_task_runner_;

  
  base::WeakPtr<IpcAudioCapturer> audio_capturer_;

  
  scoped_ptr<protocol::ClipboardStub> client_clipboard_;

  
  base::WeakPtr<ClientSessionControl> client_session_control_;

  
  
  base::WeakPtr<DesktopSessionConnector> desktop_session_connector_;

  
  base::WeakPtr<IpcVideoFrameCapturer> video_capturer_;

  
  scoped_ptr<IPC::ChannelProxy> desktop_channel_;

  
  base::ProcessHandle desktop_process_;

  int pending_capture_frame_requests_;

  
  
  SharedBuffers shared_buffers_;

  
  
  ScreenResolution screen_resolution_;

  
  bool is_desktop_session_connected_;

  bool virtual_terminal_;

  DISALLOW_COPY_AND_ASSIGN(DesktopSessionProxy);
};

struct DesktopSessionProxyTraits {
  static void Destruct(const DesktopSessionProxy* desktop_session_proxy);
};

}  

#endif  
