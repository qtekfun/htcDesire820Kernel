// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "ppapi/c/private/ppb_flash.h"
#include "ppapi/proxy/ppapi_param_traits.h"
#include "url/gurl.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START PepperMsgStart

IPC_ENUM_TRAITS(PP_FlashLSORestrictions)


IPC_SYNC_MESSAGE_CONTROL1_1(PepperMsg_GetLocalTimeZoneOffset,
                            base::Time ,
                            double )

IPC_SYNC_MESSAGE_CONTROL2_1(PepperMsg_GetLocalDataRestrictions,
                            GURL ,
                            GURL ,
                            PP_FlashLSORestrictions )
