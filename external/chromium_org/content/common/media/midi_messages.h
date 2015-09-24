// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "media/midi/midi_port_info.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START MIDIMsgStart

IPC_STRUCT_TRAITS_BEGIN(media::MIDIPortInfo)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(manufacturer)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(version)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL3(MIDIHostMsg_RequestSysExPermission,
                     int ,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL3(MIDIHostMsg_CancelSysExPermissionRequest,
                     int ,
                     int ,
                     GURL )


IPC_MESSAGE_ROUTED2(MIDIMsg_SysExPermissionApproved,
                    int ,
                    bool )


IPC_MESSAGE_CONTROL1(MIDIHostMsg_StartSession,
                     int )

IPC_MESSAGE_CONTROL3(MIDIHostMsg_SendData,
                     uint32 ,
                     std::vector<uint8> ,
                     double )


IPC_MESSAGE_CONTROL4(MIDIMsg_SessionStarted,
                     int ,
                     bool ,
                     media::MIDIPortInfoList ,
                     media::MIDIPortInfoList )

IPC_MESSAGE_CONTROL3(MIDIMsg_DataReceived,
                     uint32 ,
                     std::vector<uint8> ,
                     double )

IPC_MESSAGE_CONTROL1(MIDIMsg_AcknowledgeSentData,
                     uint32 )
