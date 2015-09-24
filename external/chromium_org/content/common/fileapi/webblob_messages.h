// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "content/common/content_export.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "webkit/common/blob/blob_data.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START BlobMsgStart


IPC_MESSAGE_CONTROL1(BlobHostMsg_StartBuilding,
                     std::string )
IPC_MESSAGE_CONTROL2(BlobHostMsg_AppendBlobDataItem,
                     std::string ,
                     webkit_blob::BlobData::Item)
IPC_SYNC_MESSAGE_CONTROL3_0(BlobHostMsg_SyncAppendSharedMemory,
                            std::string ,
                            base::SharedMemoryHandle,
                            size_t )
IPC_MESSAGE_CONTROL2(BlobHostMsg_FinishBuilding,
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL1(BlobHostMsg_IncrementRefCount,
                     std::string )
IPC_MESSAGE_CONTROL1(BlobHostMsg_DecrementRefCount,
                     std::string )
IPC_MESSAGE_CONTROL2(BlobHostMsg_RegisterPublicURL,
                     GURL,
                     std::string )
IPC_MESSAGE_CONTROL1(BlobHostMsg_RevokePublicURL,
                     GURL)


IPC_MESSAGE_CONTROL2(StreamHostMsg_StartBuilding,
                     GURL ,
                     std::string )

IPC_MESSAGE_CONTROL2(StreamHostMsg_AppendBlobDataItem,
                     GURL ,
                     webkit_blob::BlobData::Item)

IPC_SYNC_MESSAGE_CONTROL3_0(StreamHostMsg_SyncAppendSharedMemory,
                            GURL ,
                            base::SharedMemoryHandle,
                            size_t )

IPC_MESSAGE_CONTROL1(StreamHostMsg_FinishBuilding,
                     GURL )

IPC_MESSAGE_CONTROL1(StreamHostMsg_AbortBuilding,
                     GURL )

IPC_MESSAGE_CONTROL2(StreamHostMsg_Clone,
                     GURL ,
                     GURL )

IPC_MESSAGE_CONTROL1(StreamHostMsg_Remove,
                     GURL )
