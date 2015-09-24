// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "base/sync_socket.h"
#include "content/common/content_export.h"
#include "content/common/media/media_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "media/audio/audio_buffers_state.h"
#include "media/audio/audio_input_ipc.h"
#include "media/audio/audio_output_ipc.h"
#include "media/audio/audio_parameters.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START AudioMsgStart

IPC_ENUM_TRAITS(media::AudioInputIPCDelegate::State)
IPC_ENUM_TRAITS(media::AudioOutputIPCDelegate::State)

IPC_STRUCT_BEGIN(AudioInputHostMsg_CreateStream_Config)
  IPC_STRUCT_MEMBER(media::AudioParameters, params)
  IPC_STRUCT_MEMBER(bool, automatic_gain_control)
  IPC_STRUCT_MEMBER(uint32, shared_memory_count)
IPC_STRUCT_END()


#if defined(OS_WIN)
IPC_MESSAGE_CONTROL4(AudioMsg_NotifyStreamCreated,
                     int ,
                     base::SharedMemoryHandle ,
                     base::SyncSocket::Handle ,
                     uint32 )
#else
IPC_MESSAGE_CONTROL4(AudioMsg_NotifyStreamCreated,
                     int ,
                     base::SharedMemoryHandle ,
                     base::FileDescriptor ,
                     uint32 )
#endif

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL5(AudioInputMsg_NotifyStreamCreated,
                     int ,
                     base::SharedMemoryHandle ,
                     base::SyncSocket::Handle ,
                     uint32 ,
                     uint32 )
#else
IPC_MESSAGE_CONTROL5(AudioInputMsg_NotifyStreamCreated,
                     int ,
                     base::SharedMemoryHandle ,
                     base::FileDescriptor ,
                     uint32 ,
                     uint32 )
#endif

IPC_MESSAGE_CONTROL3(AudioMsg_NotifyDeviceChanged,
                     int ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(AudioMsg_NotifyStreamStateChanged,
                     int ,
                     media::AudioOutputIPCDelegate::State )

IPC_MESSAGE_CONTROL2(AudioInputMsg_NotifyStreamStateChanged,
                     int ,
                     media::AudioInputIPCDelegate::State )

IPC_MESSAGE_CONTROL2(AudioInputMsg_NotifyStreamVolume,
                     int ,
                     double )


IPC_MESSAGE_CONTROL4(AudioHostMsg_CreateStream,
                     int ,
                     int ,
                     int ,
                     media::AudioParameters )

IPC_MESSAGE_CONTROL4(AudioInputHostMsg_CreateStream,
                     int ,
                     int ,
                     int ,
                     AudioInputHostMsg_CreateStream_Config)

IPC_MESSAGE_CONTROL1(AudioHostMsg_PlayStream,
                     int )

IPC_MESSAGE_CONTROL1(AudioInputHostMsg_RecordStream,
                     int )

IPC_MESSAGE_CONTROL1(AudioHostMsg_PauseStream,
                     int )

IPC_MESSAGE_CONTROL1(AudioHostMsg_CloseStream,
                     int )

IPC_MESSAGE_CONTROL1(AudioInputHostMsg_CloseStream,
                     int )

IPC_MESSAGE_CONTROL2(AudioHostMsg_SetVolume,
                     int ,
                     double )

IPC_MESSAGE_CONTROL2(AudioInputHostMsg_SetVolume,
                     int ,
                     double )
