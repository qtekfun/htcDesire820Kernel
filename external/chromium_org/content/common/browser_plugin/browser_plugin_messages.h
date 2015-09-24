// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "base/basictypes.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/values.h"
#include "cc/output/compositor_frame.h"
#include "cc/output/compositor_frame_ack.h"
#include "content/common/content_export.h"
#include "content/common/content_param_traits.h"
#include "content/common/edit_command.h"
#include "content/public/common/browser_plugin_permission_type.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/drop_data.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/WebKit/public/web/WebCompositionUnderline.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"
#include "third_party/WebKit/public/web/WebDragStatus.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"
#include "webkit/common/cursors/webcursor.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START BrowserPluginMsgStart


IPC_ENUM_TRAITS(blink::WebDragStatus)

IPC_STRUCT_BEGIN(BrowserPluginHostMsg_AutoSize_Params)
  IPC_STRUCT_MEMBER(bool, enable)
  IPC_STRUCT_MEMBER(gfx::Size, max_size)
  IPC_STRUCT_MEMBER(gfx::Size, min_size)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(BrowserPluginHostMsg_ResizeGuest_Params)
  
  IPC_STRUCT_MEMBER(bool, size_changed)
  
  
  IPC_STRUCT_MEMBER(uint32, damage_buffer_sequence_id)
  
  IPC_STRUCT_MEMBER(base::SharedMemoryHandle, damage_buffer_handle)
  
  IPC_STRUCT_MEMBER(size_t, damage_buffer_size)
  
  IPC_STRUCT_MEMBER(gfx::Rect, view_rect)
  
  IPC_STRUCT_MEMBER(float, scale_factor)
  
  
  
  IPC_STRUCT_MEMBER(bool, repaint)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(BrowserPluginHostMsg_Attach_Params)
  IPC_STRUCT_MEMBER(std::string, storage_partition_id)
  IPC_STRUCT_MEMBER(bool, persist_storage)
  IPC_STRUCT_MEMBER(bool, focused)
  IPC_STRUCT_MEMBER(bool, visible)
  IPC_STRUCT_MEMBER(bool, opaque)
  IPC_STRUCT_MEMBER(std::string, name)
  IPC_STRUCT_MEMBER(std::string, src)
  IPC_STRUCT_MEMBER(GURL, embedder_frame_url)
  IPC_STRUCT_MEMBER(BrowserPluginHostMsg_AutoSize_Params, auto_size_params)
  IPC_STRUCT_MEMBER(BrowserPluginHostMsg_ResizeGuest_Params,
                    resize_guest_params)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(BrowserPluginMsg_Attach_ACK_Params)
  IPC_STRUCT_MEMBER(std::string, storage_partition_id)
  IPC_STRUCT_MEMBER(bool, persist_storage)
  IPC_STRUCT_MEMBER(std::string, name)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(BrowserPluginMsg_BuffersSwapped_Params)
  IPC_STRUCT_MEMBER(gfx::Size, size)
  IPC_STRUCT_MEMBER(gfx::Rect, damage_rect)
  IPC_STRUCT_MEMBER(std::string, mailbox_name)
  IPC_STRUCT_MEMBER(int, route_id)
  IPC_STRUCT_MEMBER(int, host_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(BrowserPluginMsg_UpdateRect_Params)
  
  IPC_STRUCT_MEMBER(uint32, damage_buffer_sequence_id)

  
  IPC_STRUCT_MEMBER(gfx::Rect, bitmap_rect)

  
  
  
  IPC_STRUCT_MEMBER(gfx::Vector2d, scroll_delta)

  
  IPC_STRUCT_MEMBER(gfx::Rect, scroll_rect)

  
  IPC_STRUCT_MEMBER(gfx::Point, scroll_offset)

  
  
  IPC_STRUCT_MEMBER(std::vector<gfx::Rect>, copy_rects)

  
  
  
  
  
  IPC_STRUCT_MEMBER(gfx::Size, view_size)

  
  
  IPC_STRUCT_MEMBER(float, scale_factor)

  
  IPC_STRUCT_MEMBER(bool, is_resize_ack)

  
  
  IPC_STRUCT_MEMBER(bool, needs_ack)
IPC_STRUCT_END()



IPC_MESSAGE_ROUTED1(BrowserPluginHostMsg_AllocateInstanceID,
                    int )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_ExecuteEditCommand,
                     int ,
                     std::string )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_SetEditCommandsForNextKeyEvent,
                    int ,
                    std::vector<content::EditCommand> )

IPC_MESSAGE_ROUTED5(
    BrowserPluginHostMsg_ImeSetComposition,
    int ,
    std::string ,
    std::vector<blink::WebCompositionUnderline> ,
    int ,
    int )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_ImeConfirmComposition,
                    int ,
                    std::string ,
                    bool )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_ExtendSelectionAndDelete,
                    int ,
                    int ,
                    int )

IPC_MESSAGE_ROUTED3(
    BrowserPluginHostMsg_SetAutoSize,
    int ,
    BrowserPluginHostMsg_AutoSize_Params ,
    BrowserPluginHostMsg_ResizeGuest_Params )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_Attach,
                    int ,
                    BrowserPluginHostMsg_Attach_Params ,
                    base::DictionaryValue )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_SetFocus,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_HandleInputEvent,
                    int ,
                    gfx::Rect ,
                    IPC::WebInputEventPointer )

IPC_MESSAGE_ROUTED4(BrowserPluginHostMsg_UpdateRect_ACK,
    int ,
    bool ,
    BrowserPluginHostMsg_AutoSize_Params ,
    BrowserPluginHostMsg_ResizeGuest_Params )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_NavigateGuest,
                    int ,
                    std::string )

IPC_MESSAGE_ROUTED5(BrowserPluginHostMsg_BuffersSwappedACK,
                    int ,
                    int ,
                    int ,
                    std::string ,
                    uint32 )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_CopyFromCompositingSurfaceAck,
                    int ,
                    int ,
                    SkBitmap);

IPC_MESSAGE_ROUTED5(BrowserPluginHostMsg_CompositorFrameACK,
                    int ,
                    int ,
                    uint32 ,
                    int ,
                    cc::CompositorFrameAck )

IPC_MESSAGE_ROUTED5(BrowserPluginHostMsg_ReclaimCompositorResources,
                    int ,
                    int ,
                    uint32 ,
                    int ,
                    cc::CompositorFrameAck )

IPC_MESSAGE_ROUTED1(BrowserPluginHostMsg_PluginDestroyed,
                    int )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_SetVisibility,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_SetContentsOpaque,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED5(BrowserPluginHostMsg_DragStatusUpdate,
                    int ,
                    blink::WebDragStatus ,
                    content::DropData ,
                    blink::WebDragOperationsMask ,
                    gfx::Point )

IPC_MESSAGE_ROUTED3(BrowserPluginHostMsg_PluginAtPositionResponse,
                    int ,
                    int ,
                    gfx::Point )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_SetName,
                    int ,
                    std::string )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_LockMouse_ACK,
                    int ,
                    bool )

IPC_MESSAGE_ROUTED1(BrowserPluginHostMsg_UnlockMouse_ACK, int )

IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_UpdateGeometry,
                    int ,
                    gfx::Rect )


IPC_MESSAGE_ROUTED2(BrowserPluginHostMsg_ResizeGuest,
                    int ,
                    BrowserPluginHostMsg_ResizeGuest_Params)


IPC_MESSAGE_ROUTED2(BrowserPluginMsg_AllocateInstanceID_ACK,
                    int ,
                    int )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_Attach_ACK,
                     int ,
                     BrowserPluginMsg_Attach_ACK_Params )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_GuestContentWindowReady,
                     int ,
                     int )

IPC_MESSAGE_CONTROL1(BrowserPluginMsg_GuestGone,
                     int )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_AdvanceFocus,
                     int ,
                     bool )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_ShouldAcceptTouchEvents,
                     int ,
                     bool )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_SetCursor,
                     int ,
                     WebCursor )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_UpdateRect,
                     int ,
                     BrowserPluginMsg_UpdateRect_Params)

IPC_MESSAGE_CONTROL4(BrowserPluginMsg_CopyFromCompositingSurface,
                     int ,
                     int ,
                     gfx::Rect  ,
                     gfx::Size  )

IPC_MESSAGE_ROUTED2(BrowserPluginMsg_PluginAtPositionRequest,
                    int ,
                    gfx::Point )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_UpdatedName,
                     int ,
                     std::string )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_BuffersSwapped,
                     int ,
                     BrowserPluginMsg_BuffersSwapped_Params)

IPC_MESSAGE_CONTROL5(BrowserPluginMsg_CompositorFrameSwapped,
                     int ,
                     cc::CompositorFrame ,
                     int ,
                     uint32 ,
                     int )

IPC_MESSAGE_CONTROL2(BrowserPluginMsg_SetMouseLock,
                     int ,
                     bool )
