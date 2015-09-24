// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"
#include "content/child/plugin_param_traits.h"
#include "content/common/content_export.h"
#include "content/common/content_param_traits.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "webkit/common/cursors/webcursor.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START PluginMsgStart

IPC_STRUCT_BEGIN(PluginMsg_Init_Params)
  IPC_STRUCT_MEMBER(GURL,  url)
  IPC_STRUCT_MEMBER(GURL,  page_url)
  IPC_STRUCT_MEMBER(std::vector<std::string>, arg_names)
  IPC_STRUCT_MEMBER(std::vector<std::string>, arg_values)
  IPC_STRUCT_MEMBER(bool, load_manually)
  IPC_STRUCT_MEMBER(int, host_render_view_routing_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PluginHostMsg_URLRequest_Params)
  IPC_STRUCT_MEMBER(std::string, url)
  IPC_STRUCT_MEMBER(std::string, method)
  IPC_STRUCT_MEMBER(std::string, target)
  IPC_STRUCT_MEMBER(std::vector<char>, buffer)
  IPC_STRUCT_MEMBER(int, notify_id)
  IPC_STRUCT_MEMBER(bool, popups_allowed)
  IPC_STRUCT_MEMBER(bool, notify_redirects)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PluginMsg_DidReceiveResponseParams)
  IPC_STRUCT_MEMBER(unsigned long, id)
  IPC_STRUCT_MEMBER(std::string, mime_type)
  IPC_STRUCT_MEMBER(std::string, headers)
  IPC_STRUCT_MEMBER(uint32, expected_length)
  IPC_STRUCT_MEMBER(uint32, last_modified)
  IPC_STRUCT_MEMBER(bool, request_is_seekable)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PluginMsg_FetchURL_Params)
  IPC_STRUCT_MEMBER(unsigned long, resource_id)
  IPC_STRUCT_MEMBER(int, notify_id)
  IPC_STRUCT_MEMBER(GURL, url)
  IPC_STRUCT_MEMBER(GURL, first_party_for_cookies)
  IPC_STRUCT_MEMBER(std::string, method)
  IPC_STRUCT_MEMBER(std::vector<char>, post_data)
  IPC_STRUCT_MEMBER(GURL, referrer)
  IPC_STRUCT_MEMBER(bool, notify_redirect)
  IPC_STRUCT_MEMBER(bool, is_plugin_src_load)
  IPC_STRUCT_MEMBER(int, render_view_id)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(PluginMsg_UpdateGeometry_Param)
  IPC_STRUCT_MEMBER(gfx::Rect, window_rect)
  IPC_STRUCT_MEMBER(gfx::Rect, clip_rect)
  IPC_STRUCT_MEMBER(TransportDIB::Handle, windowless_buffer0)
  IPC_STRUCT_MEMBER(TransportDIB::Handle, windowless_buffer1)
  IPC_STRUCT_MEMBER(int, windowless_buffer_index)
IPC_STRUCT_END()

IPC_SYNC_MESSAGE_CONTROL1_1(PluginMsg_CreateInstance,
                            std::string ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_0(PluginMsg_DestroyInstance,
                            int )

IPC_SYNC_MESSAGE_CONTROL0_1(PluginMsg_GenerateRouteID,
                           int )

IPC_SYNC_MESSAGE_ROUTED1_2(PluginMsg_Init,
                           PluginMsg_Init_Params,
                           bool ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_0(PluginMsg_Paint,
                           gfx::Rect )

IPC_MESSAGE_ROUTED0(PluginMsg_DidPaint)

IPC_SYNC_MESSAGE_ROUTED0_1(PluginMsg_GetPluginScriptableObject,
                           int )

IPC_SYNC_MESSAGE_ROUTED0_2(PluginMsg_GetFormValue,
                           base::string16 ,
                           bool )

IPC_MESSAGE_ROUTED3(PluginMsg_DidFinishLoadWithReason,
                    GURL ,
                    int ,
                    int )

IPC_MESSAGE_ROUTED1(PluginMsg_UpdateGeometry,
                    PluginMsg_UpdateGeometry_Param)

IPC_SYNC_MESSAGE_ROUTED1_0(PluginMsg_UpdateGeometrySync,
                           PluginMsg_UpdateGeometry_Param)

IPC_SYNC_MESSAGE_ROUTED1_0(PluginMsg_SetFocus,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_2(PluginMsg_HandleInputEvent,
                           IPC::WebInputEventPointer ,
                           bool ,
                           WebCursor )

IPC_MESSAGE_ROUTED1(PluginMsg_SetContentAreaFocus,
                    bool )

IPC_SYNC_MESSAGE_ROUTED3_0(PluginMsg_WillSendRequest,
                           unsigned long ,
                           GURL ,
                           int  )

IPC_MESSAGE_ROUTED1(PluginMsg_DidReceiveResponse,
                    PluginMsg_DidReceiveResponseParams)

IPC_MESSAGE_ROUTED3(PluginMsg_DidReceiveData,
                    unsigned long ,
                    std::vector<char> ,
                    int )

IPC_MESSAGE_ROUTED1(PluginMsg_DidFinishLoading,
                    unsigned long )

IPC_MESSAGE_ROUTED1(PluginMsg_DidFail,
                    unsigned long )

IPC_MESSAGE_ROUTED4(PluginMsg_SendJavaScriptStream,
                    GURL ,
                    std::string ,
                    bool ,
                    int )

IPC_MESSAGE_ROUTED2(PluginMsg_DidReceiveManualResponse,
                    GURL ,
                    PluginMsg_DidReceiveResponseParams)

IPC_MESSAGE_ROUTED1(PluginMsg_DidReceiveManualData,
                    std::vector<char> )

IPC_MESSAGE_ROUTED0(PluginMsg_DidFinishManualLoading)

IPC_MESSAGE_ROUTED0(PluginMsg_DidManualLoadFail)

IPC_MESSAGE_ROUTED3(PluginMsg_HandleURLRequestReply,
                    unsigned long ,
                    GURL ,
                    int )

IPC_MESSAGE_ROUTED2(PluginMsg_HTTPRangeRequestReply,
                    unsigned long ,
                    int )

IPC_MESSAGE_CONTROL1(PluginMsg_SignalModalDialogEvent,
                     int )

IPC_MESSAGE_CONTROL1(PluginMsg_ResetModalDialogEvent,
                     int )

IPC_MESSAGE_ROUTED1(PluginMsg_FetchURL,
                    PluginMsg_FetchURL_Params)

IPC_MESSAGE_CONTROL1(PluginHostMsg_DidAbortLoading,
                     int )

#if defined(OS_WIN)
IPC_MESSAGE_ROUTED4(PluginMsg_ImeCompositionUpdated,
                    base::string16 ,
                    std::vector<int> ,
                    std::vector<int>, 
                    int )

IPC_MESSAGE_ROUTED1(PluginMsg_ImeCompositionCompleted,
                    base::string16 )
#endif

#if defined(OS_MACOSX)
IPC_MESSAGE_ROUTED1(PluginMsg_SetWindowFocus,
                    bool )

IPC_MESSAGE_ROUTED0(PluginMsg_ContainerHidden)

IPC_MESSAGE_ROUTED3(PluginMsg_ContainerShown,
                    gfx::Rect ,
                    gfx::Rect ,
                    bool )

IPC_MESSAGE_ROUTED2(PluginMsg_WindowFrameChanged,
                    gfx::Rect ,
                    gfx::Rect )

IPC_MESSAGE_ROUTED1(PluginMsg_ImeCompositionCompleted,
                    base::string16 )

IPC_MESSAGE_ROUTED1(PluginMsg_SetFakeAcceleratedSurfaceWindowHandle,
                    gfx::PluginWindowHandle )
#endif

IPC_SYNC_MESSAGE_ROUTED1_0(PluginHostMsg_SetWindow,
                           gfx::PluginWindowHandle )

IPC_MESSAGE_ROUTED1(PluginHostMsg_URLRequest,
                    PluginHostMsg_URLRequest_Params)

IPC_MESSAGE_ROUTED1(PluginHostMsg_CancelResource,
                    int )

IPC_MESSAGE_ROUTED1(PluginHostMsg_InvalidateRect,
                    gfx::Rect )

IPC_SYNC_MESSAGE_ROUTED1_1(PluginHostMsg_GetWindowScriptNPObject,
                           int ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_1(PluginHostMsg_GetPluginElement,
                           int ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_2(PluginHostMsg_ResolveProxy,
                           GURL ,
                           bool ,
                           std::string )

IPC_MESSAGE_ROUTED3(PluginHostMsg_SetCookie,
                    GURL ,
                    GURL ,
                    std::string )

IPC_SYNC_MESSAGE_ROUTED2_1(PluginHostMsg_GetCookies,
                           GURL ,
                           GURL ,
                           std::string )

IPC_MESSAGE_ROUTED0(PluginHostMsg_CancelDocumentLoad)

IPC_MESSAGE_ROUTED3(PluginHostMsg_InitiateHTTPRangeRequest,
                    std::string ,
                    std::string ,
                    int         )

IPC_MESSAGE_ROUTED0(PluginHostMsg_DidStartLoading)
IPC_MESSAGE_ROUTED0(PluginHostMsg_DidStopLoading)

IPC_MESSAGE_ROUTED2(PluginHostMsg_DeferResourceLoading,
                    unsigned long ,
                    bool )

IPC_SYNC_MESSAGE_CONTROL1_0(PluginHostMsg_SetException,
                            std::string )

IPC_MESSAGE_CONTROL0(PluginHostMsg_PluginShuttingDown)

IPC_MESSAGE_ROUTED2(PluginHostMsg_URLRedirectResponse,
                    bool ,
                    int  )

IPC_SYNC_MESSAGE_ROUTED1_1(PluginHostMsg_CheckIfRunInsecureContent,
                           GURL ,
                           bool )

#if defined(OS_WIN)
IPC_SYNC_MESSAGE_ROUTED2_0(PluginHostMsg_SetWindowlessData,
                           HANDLE ,
                           gfx::NativeViewId )

IPC_MESSAGE_ROUTED2(PluginHostMsg_NotifyIMEStatus,
                    int ,
                    gfx::Rect )
#endif

#if defined(OS_MACOSX)
IPC_MESSAGE_ROUTED1(PluginHostMsg_FocusChanged,
                    bool )

IPC_MESSAGE_ROUTED0(PluginHostMsg_StartIme)


IPC_MESSAGE_ROUTED0(PluginHostMsg_AcceleratedPluginEnabledRendering)

IPC_MESSAGE_ROUTED3(PluginHostMsg_AcceleratedPluginAllocatedIOSurface,
                    int32 ,
                    int32 ,
                    uint32 )

IPC_MESSAGE_ROUTED0(PluginHostMsg_AcceleratedPluginSwappedIOSurface)
#endif


IPC_SYNC_MESSAGE_ROUTED0_0(NPObjectMsg_Release)

IPC_SYNC_MESSAGE_ROUTED1_1(NPObjectMsg_HasMethod,
                           content::NPIdentifier_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED3_2(NPObjectMsg_Invoke,
                           bool ,
                           content::NPIdentifier_Param ,
                           std::vector<content::NPVariant_Param> ,
                           content::NPVariant_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_1(NPObjectMsg_HasProperty,
                           content::NPIdentifier_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_2(NPObjectMsg_GetProperty,
                           content::NPIdentifier_Param ,
                           content::NPVariant_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED2_1(NPObjectMsg_SetProperty,
                           content::NPIdentifier_Param ,
                           content::NPVariant_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_1(NPObjectMsg_RemoveProperty,
                           content::NPIdentifier_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED0_0(NPObjectMsg_Invalidate)

IPC_SYNC_MESSAGE_ROUTED0_2(NPObjectMsg_Enumeration,
                           std::vector<content::NPIdentifier_Param> ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED1_2(NPObjectMsg_Construct,
                           std::vector<content::NPVariant_Param> ,
                           content::NPVariant_Param ,
                           bool )

IPC_SYNC_MESSAGE_ROUTED2_2(NPObjectMsg_Evaluate,
                           std::string ,
                           bool ,
                           content::NPVariant_Param ,
                           bool )
