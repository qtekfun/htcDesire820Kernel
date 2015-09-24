// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"
#include "content/common/content_export.h"
#include "content/common/content_param_traits.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START PluginProcessMsgStart

IPC_MESSAGE_CONTROL2(PluginProcessMsg_CreateChannel,
                     int ,
                     bool )

IPC_MESSAGE_CONTROL0(PluginProcessMsg_NotifyRenderersOfPendingShutdown)

IPC_MESSAGE_CONTROL3(PluginProcessMsg_ClearSiteData,
                     std::string ,
                     uint64 ,
                     uint64 )


IPC_MESSAGE_CONTROL1(PluginProcessHostMsg_ChannelCreated,
                     IPC::ChannelHandle )

IPC_MESSAGE_CONTROL1(PluginProcessHostMsg_ChannelDestroyed,
                     int )

IPC_MESSAGE_CONTROL1(PluginProcessHostMsg_ClearSiteDataResult,
                     bool )

#if defined(OS_WIN)
IPC_MESSAGE_CONTROL2(PluginProcessHostMsg_PluginWindowDestroyed,
                     HWND ,
                     HWND )
#endif

#if defined(USE_X11)
IPC_SYNC_MESSAGE_CONTROL1_1(PluginProcessHostMsg_MapNativeViewId,
                           gfx::NativeViewId ,
                           gfx::PluginWindowHandle )
#endif

#if defined(OS_MACOSX)

IPC_MESSAGE_CONTROL3(PluginProcessHostMsg_PluginSelectWindow,
                     uint32 ,
                     gfx::Rect ,
                     bool )

IPC_MESSAGE_CONTROL3(PluginProcessHostMsg_PluginShowWindow,
                     uint32 ,
                     gfx::Rect ,
                     bool )

IPC_MESSAGE_CONTROL2(PluginProcessHostMsg_PluginHideWindow,
                     uint32 ,
                     gfx::Rect )

IPC_MESSAGE_CONTROL1(PluginProcessHostMsg_PluginSetCursorVisibility,
                     bool )
#endif
