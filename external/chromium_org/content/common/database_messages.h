// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "ipc/ipc_platform_file.h"

#define IPC_MESSAGE_START DatabaseMsgStart


IPC_MESSAGE_CONTROL3(DatabaseMsg_UpdateSize,
                     std::string ,
                     base::string16 ,
                     int64 )

IPC_MESSAGE_CONTROL2(DatabaseMsg_UpdateSpaceAvailable,
                     std::string ,
                     int64 )

IPC_MESSAGE_CONTROL1(DatabaseMsg_ResetSpaceAvailable,
                     std::string )

IPC_MESSAGE_CONTROL2(DatabaseMsg_CloseImmediately,
                     std::string ,
                     base::string16 )


IPC_SYNC_MESSAGE_CONTROL2_1(DatabaseHostMsg_OpenFile,
                            base::string16 ,
                            int ,
                            IPC::PlatformFileForTransit )

IPC_SYNC_MESSAGE_CONTROL2_1(DatabaseHostMsg_DeleteFile,
                            base::string16 ,
                            bool ,
                            int )

IPC_SYNC_MESSAGE_CONTROL1_1(DatabaseHostMsg_GetFileAttributes,
                            base::string16 ,
                            int32 )

IPC_SYNC_MESSAGE_CONTROL1_1(DatabaseHostMsg_GetFileSize,
                            base::string16 ,
                            int64 )

IPC_SYNC_MESSAGE_CONTROL1_1(DatabaseHostMsg_GetSpaceAvailable,
                            std::string ,
                            int64 )

IPC_MESSAGE_CONTROL4(DatabaseHostMsg_Opened,
                     std::string ,
                     base::string16 ,
                     base::string16 ,
                     int64 )

IPC_MESSAGE_CONTROL2(DatabaseHostMsg_Modified,
                     std::string ,
                     base::string16 )

IPC_MESSAGE_CONTROL2(DatabaseHostMsg_Closed,
                     std::string ,
                     base::string16 )

IPC_MESSAGE_CONTROL3(DatabaseHostMsg_HandleSqliteError,
                     std::string ,
                     base::string16 ,
                     int  )
