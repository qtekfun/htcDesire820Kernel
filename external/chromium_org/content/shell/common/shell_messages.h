// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>
#include <vector>

#include "content/public/common/common_param_traits.h"
#include "content/public/common/page_state.h"
#include "content/shell/common/shell_test_configuration.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/skia/include/core/SkBitmap.h"

#define IPC_MESSAGE_START ShellMsgStart

IPC_STRUCT_TRAITS_BEGIN(content::ShellTestConfiguration)
IPC_STRUCT_TRAITS_MEMBER(current_working_directory)
IPC_STRUCT_TRAITS_MEMBER(temp_path)
IPC_STRUCT_TRAITS_MEMBER(test_url)
IPC_STRUCT_TRAITS_MEMBER(enable_pixel_dumping)
IPC_STRUCT_TRAITS_MEMBER(allow_external_pages)
IPC_STRUCT_TRAITS_MEMBER(expected_pixel_hash)
IPC_STRUCT_TRAITS_MEMBER(initial_size)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_ROUTED0(ShellViewMsg_Reset)

IPC_MESSAGE_CONTROL1(ShellViewMsg_SetWebKitSourceDir,
                     base::FilePath )

IPC_MESSAGE_ROUTED1(ShellViewMsg_SetTestConfiguration,
                    content::ShellTestConfiguration)

IPC_MESSAGE_ROUTED0(ShellViewMsg_NotifyDone)

IPC_MESSAGE_ROUTED3(
    ShellViewMsg_SessionHistory,
    std::vector<int> ,
    std::vector<std::vector<content::PageState> > ,
    std::vector<unsigned> )

IPC_MESSAGE_ROUTED1(ShellViewHostMsg_TextDump,
                    std::string )

IPC_MESSAGE_ROUTED2(ShellViewHostMsg_ImageDump,
                    std::string ,
                    SkBitmap )

IPC_MESSAGE_ROUTED1(ShellViewHostMsg_AudioDump,
                    std::vector<unsigned char> )

IPC_MESSAGE_ROUTED0(ShellViewHostMsg_TestFinished)

IPC_MESSAGE_ROUTED0(ShellViewHostMsg_ResetDone)

IPC_MESSAGE_ROUTED0(ShellViewHostMsg_TestFinishedInSecondaryWindow)

IPC_MESSAGE_ROUTED1(ShellViewHostMsg_OverridePreferences,
                    WebPreferences )
IPC_SYNC_MESSAGE_ROUTED1_1(ShellViewHostMsg_RegisterIsolatedFileSystem,
                           std::vector<base::FilePath> ,
                           std::string )
IPC_SYNC_MESSAGE_ROUTED1_1(ShellViewHostMsg_ReadFileToString,
                           base::FilePath ,
                           std::string )
IPC_MESSAGE_ROUTED1(ShellViewHostMsg_PrintMessage,
                    std::string )
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_ShowDevTools)
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_CloseDevTools)
IPC_MESSAGE_ROUTED1(ShellViewHostMsg_GoToOffset,
                    int )
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_Reload)
IPC_MESSAGE_ROUTED2(ShellViewHostMsg_LoadURLForFrame,
                    GURL ,
                    std::string )
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_ClearAllDatabases)
IPC_MESSAGE_ROUTED1(ShellViewHostMsg_SetDatabaseQuota,
                    int )
IPC_MESSAGE_ROUTED1(ShellViewHostMsg_AcceptAllCookies,
                    bool )
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_DeleteAllCookies)
IPC_MESSAGE_ROUTED1(ShellViewHostMsg_SetDeviceScaleFactor,
                    float )
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_CaptureSessionHistory)
IPC_MESSAGE_ROUTED0(ShellViewHostMsg_CloseRemainingWindows)
