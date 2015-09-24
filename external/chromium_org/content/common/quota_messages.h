// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "base/basictypes.h"
#include "ipc/ipc_message_macros.h"
#include "url/gurl.h"
#include "webkit/common/quota/quota_types.h"

#define IPC_MESSAGE_START QuotaMsgStart

IPC_ENUM_TRAITS(quota::StorageType)
IPC_ENUM_TRAITS(quota::QuotaStatusCode)


IPC_MESSAGE_CONTROL2(QuotaMsg_DidGrantStorageQuota,
                     int ,
                     int64 )

IPC_MESSAGE_CONTROL3(QuotaMsg_DidQueryStorageUsageAndQuota,
                     int ,
                     int64 ,
                     int64 )

IPC_MESSAGE_CONTROL2(QuotaMsg_DidFail,
                     int ,
                     quota::QuotaStatusCode )


IPC_MESSAGE_CONTROL3(QuotaHostMsg_QueryStorageUsageAndQuota,
                     int ,
                     GURL ,
                     quota::StorageType )

IPC_MESSAGE_CONTROL5(QuotaHostMsg_RequestStorageQuota,
                     int ,
                     int ,
                     GURL ,
                     quota::StorageType ,
                     int64 )
