// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "ipc/ipc_message.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/size.h"

#define IPC_MESSAGE_START ImageMsgStart


IPC_MESSAGE_ROUTED4(ImageMsg_DownloadImage,
                    int ,
                    GURL ,
                    bool ,
                    uint32_t 
)


IPC_MESSAGE_ROUTED5(ImageHostMsg_DidDownloadImage,
                    int ,
                    int ,
                    GURL ,
                    std::vector<SkBitmap> ,
                    std::vector<gfx::Size>)
