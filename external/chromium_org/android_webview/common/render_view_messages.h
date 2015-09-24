// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "android_webview/common/aw_hit_test_data.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/skia/include/core/SkColor.h"

#ifndef ANDROID_WEBVIEW_COMMON_RENDER_VIEW_MESSAGES_H_
#define ANDROID_WEBVIEW_COMMON_RENDER_VIEW_MESSAGES_H_

namespace IPC {


}  

#endif  

IPC_STRUCT_TRAITS_BEGIN(android_webview::AwHitTestData)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(extra_data_for_type)
  IPC_STRUCT_TRAITS_MEMBER(href)
  IPC_STRUCT_TRAITS_MEMBER(anchor_text)
  IPC_STRUCT_TRAITS_MEMBER(img_src)
IPC_STRUCT_TRAITS_END()

#define IPC_MESSAGE_START AndroidWebViewMsgStart


IPC_MESSAGE_CONTROL0(AwViewMsg_ClearCache)

IPC_MESSAGE_ROUTED1(AwViewMsg_DocumentHasImages,
                    int )

IPC_MESSAGE_ROUTED2(AwViewMsg_DoHitTest,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(AwViewMsg_SetTextZoomFactor,
                    float )

IPC_MESSAGE_ROUTED0(AwViewMsg_ResetScrollAndScaleState)

IPC_MESSAGE_ROUTED1(AwViewMsg_SetInitialPageScale,
                    double )

IPC_MESSAGE_ROUTED1(AwViewMsg_SetFixedLayoutSize,
                    gfx::Size )

IPC_MESSAGE_ROUTED1(AwViewMsg_SetBackgroundColor,
                    SkColor)

IPC_MESSAGE_CONTROL1(AwViewMsg_SetJsOnlineProperty,
                     bool )


IPC_MESSAGE_ROUTED2(AwViewHostMsg_DocumentHasImagesResponse,
                    int, 
                    bool )

IPC_MESSAGE_ROUTED1(AwViewHostMsg_UpdateHitTestData,
                    android_webview::AwHitTestData)

IPC_MESSAGE_ROUTED1(AwViewHostMsg_PageScaleFactorChanged,
                    float )

IPC_MESSAGE_ROUTED1(AwViewHostMsg_OnContentsSizeChanged,
                    gfx::Size )

IPC_SYNC_MESSAGE_CONTROL2_1(AwViewHostMsg_ShouldOverrideUrlLoading,
                            int ,
                            string16 ,
                            bool )
