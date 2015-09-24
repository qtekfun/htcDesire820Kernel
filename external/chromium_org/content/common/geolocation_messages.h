// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/public/common/geoposition.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START GeolocationMsgStart

IPC_ENUM_TRAITS(content::Geoposition::ErrorCode)

IPC_STRUCT_TRAITS_BEGIN(content::Geoposition)
  IPC_STRUCT_TRAITS_MEMBER(latitude)
  IPC_STRUCT_TRAITS_MEMBER(longitude)
  IPC_STRUCT_TRAITS_MEMBER(altitude)
  IPC_STRUCT_TRAITS_MEMBER(accuracy)
  IPC_STRUCT_TRAITS_MEMBER(altitude_accuracy)
  IPC_STRUCT_TRAITS_MEMBER(heading)
  IPC_STRUCT_TRAITS_MEMBER(speed)
  IPC_STRUCT_TRAITS_MEMBER(timestamp)
  IPC_STRUCT_TRAITS_MEMBER(error_code)
  IPC_STRUCT_TRAITS_MEMBER(error_message)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED2(GeolocationMsg_PermissionSet,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED1(GeolocationMsg_PositionUpdated,
                    content::Geoposition )


IPC_MESSAGE_CONTROL3(GeolocationHostMsg_RequestPermission,
                     int ,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL3(GeolocationHostMsg_CancelPermissionRequest,
                     int ,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL3(GeolocationHostMsg_StartUpdating,
                     int ,
                     GURL ,
                     bool )

IPC_MESSAGE_CONTROL1(GeolocationHostMsg_StopUpdating,
                     int )
