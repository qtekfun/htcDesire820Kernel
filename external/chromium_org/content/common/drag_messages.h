// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/common/drag_event_source_info.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/drop_data.h"
#include "ipc/ipc_message_macros.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "ui/gfx/point.h"
#include "ui/gfx/vector2d.h"

#define IPC_MESSAGE_START DragMsgStart


IPC_MESSAGE_ROUTED5(DragMsg_TargetDragEnter,
                    content::DropData ,
                    gfx::Point ,
                    gfx::Point ,
                    blink::WebDragOperationsMask ,
                    int )

IPC_MESSAGE_ROUTED4(DragMsg_TargetDragOver,
                    gfx::Point ,
                    gfx::Point ,
                    blink::WebDragOperationsMask ,
                    int )

IPC_MESSAGE_ROUTED0(DragMsg_TargetDragLeave)

IPC_MESSAGE_ROUTED3(DragMsg_TargetDrop,
                    gfx::Point ,
                    gfx::Point ,
                    int )

IPC_MESSAGE_ROUTED4(DragMsg_SourceEndedOrMoved,
                    gfx::Point ,
                    gfx::Point ,
                    bool ,
                    blink::WebDragOperation )

IPC_MESSAGE_ROUTED0(DragMsg_SourceSystemDragEnded)


IPC_MESSAGE_ROUTED5(DragHostMsg_StartDragging,
                    content::DropData ,
                    blink::WebDragOperationsMask ,
                    SkBitmap ,
                    gfx::Vector2d ,
                    content::DragEventSourceInfo )

IPC_MESSAGE_ROUTED1(DragHostMsg_UpdateDragCursor,
                    blink::WebDragOperation )

IPC_MESSAGE_ROUTED0(DragHostMsg_TargetDrop_ACK)
