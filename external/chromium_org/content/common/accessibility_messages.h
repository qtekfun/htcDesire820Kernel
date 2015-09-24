// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/basictypes.h"
#include "content/common/accessibility_node_data.h"
#include "content/common/content_export.h"
#include "content/common/view_message_enums.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/ipc_param_traits.h"
#include "ipc/param_traits_macros.h"
#include "third_party/WebKit/public/web/WebAXEnums.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START AccessibilityMsgStart

IPC_ENUM_TRAITS(blink::WebAXEvent)
IPC_ENUM_TRAITS(blink::WebAXRole)

IPC_ENUM_TRAITS(content::AccessibilityNodeData::BoolAttribute)
IPC_ENUM_TRAITS(content::AccessibilityNodeData::FloatAttribute)
IPC_ENUM_TRAITS(content::AccessibilityNodeData::IntAttribute)
IPC_ENUM_TRAITS(content::AccessibilityNodeData::IntListAttribute)
IPC_ENUM_TRAITS(content::AccessibilityNodeData::StringAttribute)

IPC_STRUCT_TRAITS_BEGIN(content::AccessibilityNodeData)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(role)
  IPC_STRUCT_TRAITS_MEMBER(state)
  IPC_STRUCT_TRAITS_MEMBER(location)
  IPC_STRUCT_TRAITS_MEMBER(string_attributes)
  IPC_STRUCT_TRAITS_MEMBER(int_attributes)
  IPC_STRUCT_TRAITS_MEMBER(float_attributes)
  IPC_STRUCT_TRAITS_MEMBER(bool_attributes)
  IPC_STRUCT_TRAITS_MEMBER(intlist_attributes)
  IPC_STRUCT_TRAITS_MEMBER(html_attributes)
  IPC_STRUCT_TRAITS_MEMBER(child_ids)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(AccessibilityHostMsg_EventParams)
  
  
  IPC_STRUCT_MEMBER(std::vector<content::AccessibilityNodeData>, nodes)

  
  IPC_STRUCT_MEMBER(blink::WebAXEvent, event_type)

  
  IPC_STRUCT_MEMBER(int, id)
IPC_STRUCT_END()


IPC_MESSAGE_ROUTED1(AccessibilityMsg_SetFocus,
                    int )

IPC_MESSAGE_ROUTED1(AccessibilityMsg_DoDefaultAction,
                    int )

IPC_MESSAGE_ROUTED2(AccessibilityMsg_ScrollToMakeVisible,
                    int ,
                    gfx::Rect )

IPC_MESSAGE_ROUTED2(AccessibilityMsg_ScrollToPoint,
                    int ,
                    gfx::Point )

IPC_MESSAGE_ROUTED3(AccessibilityMsg_SetTextSelection,
                    int ,
                    int ,
                    int )

IPC_MESSAGE_ROUTED0(AccessibilityMsg_Events_ACK)


IPC_MESSAGE_ROUTED0(AccessibilityMsg_FatalError)


IPC_MESSAGE_ROUTED1(
    AccessibilityHostMsg_Events,
    std::vector<AccessibilityHostMsg_EventParams>)
