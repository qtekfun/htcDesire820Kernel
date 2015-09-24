// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CHROMOTING_MESSAGES_H_
#define REMOTING_HOST_CHROMOTING_MESSAGES_H_

#include "ipc/ipc_platform_file.h"
#include "net/base/ip_endpoint.h"
#include "remoting/host/chromoting_param_traits.h"
#include "remoting/host/screen_resolution.h"
#include "remoting/protocol/transport.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"
#include "third_party/webrtc/modules/desktop_capture/mouse_cursor_shape.h"

#endif  

#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START ChromotingMsgStart


IPC_MESSAGE_CONTROL3(ChromotingDaemonMsg_Crash,
                     std::string ,
                     std::string ,
                     int )


IPC_MESSAGE_CONTROL1(ChromotingDaemonNetworkMsg_Configuration, std::string)

IPC_MESSAGE_CONTROL1(ChromotingDaemonNetworkMsg_TerminalDisconnected,
                     int )

IPC_MESSAGE_CONTROL3(ChromotingDaemonNetworkMsg_DesktopAttached,
                     int ,
                     base::ProcessHandle ,
                     IPC::PlatformFileForTransit )


IPC_MESSAGE_CONTROL3(ChromotingNetworkHostMsg_ConnectTerminal,
                     int ,
                     remoting::ScreenResolution ,
                     bool )

IPC_MESSAGE_CONTROL1(ChromotingNetworkHostMsg_DisconnectTerminal,
                     int )

IPC_MESSAGE_CONTROL2(ChromotingNetworkDaemonMsg_SetScreenResolution,
                     int ,
                     remoting::ScreenResolution )

IPC_STRUCT_BEGIN(SerializedTransportRoute)
  IPC_STRUCT_MEMBER(int, type)
  IPC_STRUCT_MEMBER(net::IPAddressNumber, remote_address)
  IPC_STRUCT_MEMBER(int, remote_port)
  IPC_STRUCT_MEMBER(net::IPAddressNumber, local_address)
  IPC_STRUCT_MEMBER(int, local_port)
IPC_STRUCT_END()

IPC_MESSAGE_CONTROL1(ChromotingNetworkDaemonMsg_AccessDenied,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDaemonMsg_ClientAuthenticated,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDaemonMsg_ClientConnected,
                     std::string )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDaemonMsg_ClientDisconnected,
                     std::string )

IPC_MESSAGE_CONTROL3(ChromotingNetworkDaemonMsg_ClientRouteChange,
                     std::string ,
                     std::string ,
                     SerializedTransportRoute )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDaemonMsg_HostStarted,
                     std::string )

IPC_MESSAGE_CONTROL0(ChromotingNetworkDaemonMsg_HostShutdown)


IPC_MESSAGE_CONTROL1(ChromotingDesktopDaemonMsg_DesktopAttached,
                     IPC::PlatformFileForTransit )

IPC_MESSAGE_CONTROL0(ChromotingDesktopDaemonMsg_InjectSas)


IPC_MESSAGE_CONTROL3(ChromotingDesktopNetworkMsg_CreateSharedBuffer,
                     int ,
                     IPC::PlatformFileForTransit ,
                     uint32 )

IPC_MESSAGE_CONTROL1(ChromotingDesktopNetworkMsg_ReleaseSharedBuffer,
                     int )

IPC_STRUCT_TRAITS_BEGIN(webrtc::MouseCursorShape)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(hotspot)
  IPC_STRUCT_TRAITS_MEMBER(data)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(SerializedDesktopFrame)
  
  IPC_STRUCT_MEMBER(int, shared_buffer_id)

  
  IPC_STRUCT_MEMBER(int, bytes_per_row)

  
  IPC_STRUCT_MEMBER(std::vector<webrtc::DesktopRect>, dirty_region)

  
  IPC_STRUCT_MEMBER(webrtc::DesktopSize, dimensions)

  
  IPC_STRUCT_MEMBER(int, capture_time_ms)

  
  IPC_STRUCT_MEMBER(int64, client_sequence_number)

  
  IPC_STRUCT_MEMBER(webrtc::DesktopVector, dpi)
IPC_STRUCT_END()

IPC_MESSAGE_CONTROL1(ChromotingDesktopNetworkMsg_CaptureCompleted,
                     SerializedDesktopFrame  )

IPC_MESSAGE_CONTROL1(ChromotingDesktopNetworkMsg_CursorShapeChanged,
                     webrtc::MouseCursorShape  )

IPC_MESSAGE_CONTROL1(ChromotingDesktopNetworkMsg_InjectClipboardEvent,
                     std::string  )

IPC_MESSAGE_CONTROL0(ChromotingDesktopNetworkMsg_DisconnectSession)

IPC_MESSAGE_CONTROL1(ChromotingDesktopNetworkMsg_AudioPacket,
                     std::string  )


IPC_MESSAGE_CONTROL3(ChromotingNetworkDesktopMsg_StartSessionAgent,
                     std::string ,
                     remoting::ScreenResolution ,
                     bool )

IPC_MESSAGE_CONTROL0(ChromotingNetworkDesktopMsg_CaptureFrame)

IPC_MESSAGE_CONTROL1(ChromotingNetworkDesktopMsg_InjectClipboardEvent,
                     std::string  )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDesktopMsg_InjectKeyEvent,
                     std::string  )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDesktopMsg_InjectMouseEvent,
                     std::string  )

IPC_MESSAGE_CONTROL1(ChromotingNetworkDesktopMsg_SetScreenResolution,
                     remoting::ScreenResolution )
