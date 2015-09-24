// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/common/dom_storage/dom_storage_types.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/WebKit/public/platform/WebStorageArea.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START DOMStorageMsgStart

IPC_STRUCT_BEGIN(DOMStorageMsg_Event_Params)
  
  IPC_STRUCT_MEMBER(base::NullableString16, key)

  
  IPC_STRUCT_MEMBER(base::NullableString16, old_value)

  
  IPC_STRUCT_MEMBER(base::NullableString16, new_value)

  
  IPC_STRUCT_MEMBER(GURL, origin)

  
  IPC_STRUCT_MEMBER(GURL, page_url)

  
  
  IPC_STRUCT_MEMBER(int, connection_id)

  
  
  IPC_STRUCT_MEMBER(int64, namespace_id)
IPC_STRUCT_END()

IPC_ENUM_TRAITS(blink::WebStorageArea::Result)


IPC_MESSAGE_CONTROL1(DOMStorageMsg_Event,
                     DOMStorageMsg_Event_Params)

IPC_MESSAGE_CONTROL1(DOMStorageMsg_AsyncOperationComplete,
                     bool )

IPC_MESSAGE_CONTROL1(DOMStorageMsg_ResetCachedValues,
                     int64 )


IPC_MESSAGE_CONTROL3(DOMStorageHostMsg_OpenStorageArea,
                     int ,
                     int64 ,
                     GURL )

IPC_MESSAGE_CONTROL1(DOMStorageHostMsg_CloseStorageArea,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_2(DOMStorageHostMsg_LoadStorageArea,
                            int ,
                            content::DOMStorageValuesMap,
                            bool )

IPC_MESSAGE_CONTROL4(DOMStorageHostMsg_SetItem,
                     int ,
                     base::string16 ,
                     base::string16 ,
                     GURL )

IPC_MESSAGE_CONTROL3(DOMStorageHostMsg_LogGetItem,
                     int ,
                     base::string16 ,
                     base::NullableString16 )

IPC_MESSAGE_CONTROL3(DOMStorageHostMsg_RemoveItem,
                     int ,
                     base::string16 ,
                     GURL )

IPC_MESSAGE_CONTROL2(DOMStorageHostMsg_Clear,
                     int ,
                     GURL )

IPC_SYNC_MESSAGE_CONTROL0_0(DOMStorageHostMsg_FlushMessages)
