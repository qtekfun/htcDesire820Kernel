// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "chrome/common/favicon_url.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/skia/include/core/SkBitmap.h"

#define IPC_MESSAGE_START IconMsgStart

IPC_ENUM_TRAITS(FaviconURL::IconType)

IPC_STRUCT_TRAITS_BEGIN(FaviconURL)
  IPC_STRUCT_TRAITS_MEMBER(icon_url)
  IPC_STRUCT_TRAITS_MEMBER(icon_type)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_ROUTED3(IconMsg_DownloadFavicon,
                    int ,
                    GURL ,
                    int 
)


IPC_MESSAGE_ROUTED2(IconHostMsg_UpdateFaviconURL,
                    int32 ,
                    std::vector<FaviconURL> )

IPC_MESSAGE_ROUTED4(IconHostMsg_DidDownloadFavicon,
                    int ,
                    GURL ,
                    bool ,
                    SkBitmap )
