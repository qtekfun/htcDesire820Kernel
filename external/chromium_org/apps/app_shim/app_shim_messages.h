// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "apps/app_shim/app_shim_launch.h"
#include "base/files/file_path.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/param_traits_macros.h"

#define IPC_MESSAGE_START AppShimMsgStart

IPC_ENUM_TRAITS_MAX_VALUE(apps::AppShimLaunchType,
                          apps::APP_SHIM_LAUNCH_NUM_TYPES - 1)
IPC_ENUM_TRAITS_MAX_VALUE(apps::AppShimLaunchResult,
                          apps::APP_SHIM_LAUNCH_NUM_RESULTS - 1)
IPC_ENUM_TRAITS_MAX_VALUE(apps::AppShimFocusType,
                          apps::APP_SHIM_FOCUS_NUM_TYPES - 1)

IPC_MESSAGE_CONTROL1(AppShimMsg_LaunchApp_Done,
                     apps::AppShimLaunchResult )

IPC_MESSAGE_CONTROL0(AppShimMsg_Hide)

IPC_MESSAGE_CONTROL0(AppShimMsg_RequestUserAttention)

IPC_MESSAGE_CONTROL4(AppShimHostMsg_LaunchApp,
                     base::FilePath ,
                     std::string ,
                     apps::AppShimLaunchType ,
                     std::vector<base::FilePath> )

IPC_MESSAGE_CONTROL2(AppShimHostMsg_FocusApp,
                     apps::AppShimFocusType ,
                     std::vector<base::FilePath> )

IPC_MESSAGE_CONTROL1(AppShimHostMsg_SetAppHidden,
                     bool )

IPC_MESSAGE_CONTROL0(AppShimHostMsg_QuitApp)
