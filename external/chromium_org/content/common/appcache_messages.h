// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "webkit/common/appcache/appcache_interfaces.h"

#define IPC_MESSAGE_START AppCacheMsgStart

IPC_ENUM_TRAITS(appcache::EventID)
IPC_ENUM_TRAITS(appcache::Status)

IPC_STRUCT_TRAITS_BEGIN(appcache::AppCacheInfo)
  IPC_STRUCT_TRAITS_MEMBER(manifest_url)
  IPC_STRUCT_TRAITS_MEMBER(creation_time)
  IPC_STRUCT_TRAITS_MEMBER(last_update_time)
  IPC_STRUCT_TRAITS_MEMBER(last_access_time)
  IPC_STRUCT_TRAITS_MEMBER(cache_id)
  IPC_STRUCT_TRAITS_MEMBER(group_id)
  IPC_STRUCT_TRAITS_MEMBER(status)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(is_complete)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(appcache::AppCacheResourceInfo)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(is_master)
  IPC_STRUCT_TRAITS_MEMBER(is_manifest)
  IPC_STRUCT_TRAITS_MEMBER(is_fallback)
  IPC_STRUCT_TRAITS_MEMBER(is_foreign)
  IPC_STRUCT_TRAITS_MEMBER(is_explicit)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL1(AppCacheHostMsg_RegisterHost,
                     int )

IPC_MESSAGE_CONTROL1(AppCacheHostMsg_UnregisterHost,
                     int )

IPC_MESSAGE_CONTROL2(AppCacheHostMsg_SetSpawningHostId,
                     int ,
                     int )

IPC_MESSAGE_CONTROL4(AppCacheHostMsg_SelectCache,
                     int ,
                     GURL  ,
                     int64 ,
                     GURL  )

IPC_MESSAGE_CONTROL3(AppCacheHostMsg_SelectCacheForWorker,
                     int ,
                     int ,
                     int )
IPC_MESSAGE_CONTROL2(AppCacheHostMsg_SelectCacheForSharedWorker,
                     int ,
                     int64 )

IPC_MESSAGE_CONTROL3(AppCacheHostMsg_MarkAsForeignEntry,
                     int ,
                     GURL  ,
                     int64 )

IPC_SYNC_MESSAGE_CONTROL1_1(AppCacheHostMsg_GetStatus,
                            int ,
                            appcache::Status)

IPC_SYNC_MESSAGE_CONTROL1_1(AppCacheHostMsg_StartUpdate,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AppCacheHostMsg_SwapCache,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(AppCacheHostMsg_GetResourceList,
                            int ,
                            std::vector<appcache::AppCacheResourceInfo>
                            )



IPC_MESSAGE_CONTROL2(AppCacheMsg_CacheSelected,
                     int ,
                     appcache::AppCacheInfo)

IPC_MESSAGE_CONTROL2(AppCacheMsg_StatusChanged,
                     std::vector<int> ,
                     appcache::Status)

IPC_MESSAGE_CONTROL2(AppCacheMsg_EventRaised,
                     std::vector<int> ,
                     appcache::EventID)

IPC_MESSAGE_CONTROL4(AppCacheMsg_ProgressEventRaised,
                     std::vector<int> ,
                     GURL ,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(AppCacheMsg_ErrorEventRaised,
                     std::vector<int> ,
                     std::string )

IPC_MESSAGE_CONTROL3(AppCacheMsg_LogMessage,
                     int ,
                     int ,
                     std::string )

IPC_MESSAGE_CONTROL2(AppCacheMsg_ContentBlocked,
                     int ,
                     GURL )
