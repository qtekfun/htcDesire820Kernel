// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/public/common/show_desktop_notification_params.h"
#include "ipc/ipc_message_macros.h"

#define IPC_MESSAGE_START DesktopNotificationMsgStart

IPC_STRUCT_TRAITS_BEGIN(content::ShowDesktopNotificationHostMsgParams)
  IPC_STRUCT_TRAITS_MEMBER(origin)
  IPC_STRUCT_TRAITS_MEMBER(icon_url)
  IPC_STRUCT_TRAITS_MEMBER(title)
  IPC_STRUCT_TRAITS_MEMBER(body)
  IPC_STRUCT_TRAITS_MEMBER(direction)
  IPC_STRUCT_TRAITS_MEMBER(replace_id)
  IPC_STRUCT_TRAITS_MEMBER(notification_id)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED1(DesktopNotificationMsg_PostDisplay,
                    int )

IPC_MESSAGE_ROUTED2(DesktopNotificationMsg_PostError,
                    int ,
                    base::string16 )

IPC_MESSAGE_ROUTED2(DesktopNotificationMsg_PostClose,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED1(DesktopNotificationMsg_PostClick,
                    int )

IPC_MESSAGE_ROUTED1(DesktopNotificationMsg_PermissionRequestDone,
                    int )


IPC_MESSAGE_ROUTED1(DesktopNotificationHostMsg_Show,
                    content::ShowDesktopNotificationHostMsgParams)

IPC_MESSAGE_ROUTED1(DesktopNotificationHostMsg_Cancel,
                    int )

IPC_MESSAGE_ROUTED2(DesktopNotificationHostMsg_RequestPermission,
                    GURL ,
                    int )

IPC_SYNC_MESSAGE_ROUTED1_1(DesktopNotificationHostMsg_CheckPermission,
                           GURL ,
                           int )
