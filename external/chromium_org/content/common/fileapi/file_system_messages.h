// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "url/gurl.h"
#include "webkit/common/fileapi/directory_entry.h"
#include "webkit/common/fileapi/file_system_info.h"
#include "webkit/common/fileapi/file_system_types.h"
#include "webkit/common/quota/quota_types.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START FileSystemMsgStart

IPC_STRUCT_TRAITS_BEGIN(fileapi::DirectoryEntry)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(is_directory)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(fileapi::FileSystemInfo)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(root_url)
  IPC_STRUCT_TRAITS_MEMBER(mount_type)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(fileapi::FileSystemType)
IPC_ENUM_TRAITS(quota::QuotaLimitType)


IPC_MESSAGE_CONTROL3(FileSystemMsg_DidOpenFileSystem,
                     int ,
                     std::string ,
                     GURL )

IPC_MESSAGE_CONTROL4(FileSystemMsg_DidResolveURL,
                     int ,
                     fileapi::FileSystemInfo ,
                     base::FilePath ,
                     bool )
IPC_MESSAGE_CONTROL1(FileSystemMsg_DidSucceed,
                     int )
IPC_MESSAGE_CONTROL2(FileSystemMsg_DidReadMetadata,
                     int ,
                     base::PlatformFileInfo)
IPC_MESSAGE_CONTROL3(FileSystemMsg_DidCreateSnapshotFile,
                     int ,
                     base::PlatformFileInfo,
                     base::FilePath )
IPC_MESSAGE_CONTROL3(FileSystemMsg_DidReadDirectory,
                     int ,
                     std::vector<fileapi::DirectoryEntry> ,
                     bool )
IPC_MESSAGE_CONTROL3(FileSystemMsg_DidWrite,
                     int ,
                     int64 ,
                     bool )
IPC_MESSAGE_CONTROL4(FileSystemMsg_DidOpenFile,
                     int ,
                     IPC::PlatformFileForTransit,
                     int ,
                     quota::QuotaLimitType )
IPC_MESSAGE_CONTROL2(FileSystemMsg_DidFail,
                     int ,
                     base::PlatformFileError )


IPC_MESSAGE_CONTROL3(FileSystemHostMsg_OpenFileSystem,
                     int ,
                     GURL ,
                     fileapi::FileSystemType )

IPC_MESSAGE_CONTROL2(FileSystemHostMsg_ResolveURL,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_DeleteFileSystem,
                     int ,
                     GURL ,
                     fileapi::FileSystemType )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_Move,
                     int ,
                     GURL ,
                     GURL )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_Copy,
                     int ,
                     GURL ,
                     GURL )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_Remove,
                     int ,
                     GURL ,
                     bool )

IPC_MESSAGE_CONTROL2(FileSystemHostMsg_ReadMetadata,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL5(FileSystemHostMsg_Create,
                     int ,
                     GURL ,
                     bool ,
                     bool ,
                     bool )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_Exists,
                     int ,
                     GURL ,
                     bool )

IPC_MESSAGE_CONTROL2(FileSystemHostMsg_ReadDirectory,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL4(FileSystemHostMsg_WriteDeprecated,
                     int ,
                     GURL ,
                     GURL ,
                     int64 )

IPC_MESSAGE_CONTROL4(FileSystemHostMsg_Write,
                     int ,
                     GURL ,
                     std::string ,
                     int64 )

IPC_MESSAGE_CONTROL3(FileSystemHostMsg_Truncate,
                     int ,
                     GURL ,
                     int64 )

IPC_MESSAGE_CONTROL4(FileSystemHostMsg_TouchFile,
                     int ,
                     GURL ,
                     base::Time ,
                     base::Time )

IPC_MESSAGE_CONTROL2(FileSystemHostMsg_CancelWrite,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(FileSystemHostMsg_CreateSnapshotFile,
                     int ,
                     GURL )

IPC_MESSAGE_CONTROL1(FileSystemHostMsg_DidReceiveSnapshotFile,
                     int )

IPC_SYNC_MESSAGE_CONTROL1_1(FileSystemHostMsg_SyncGetPlatformPath,
                            GURL ,
                            base::FilePath )
