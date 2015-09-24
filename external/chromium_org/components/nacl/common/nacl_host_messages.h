// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "base/basictypes.h"
#include "base/process/process.h"
#include "build/build_config.h"
#include "components/nacl/common/nacl_types.h"
#include "components/nacl/common/pnacl_types.h"
#include "content/public/common/common_param_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "url/gurl.h"

#define IPC_MESSAGE_START NaClHostMsgStart

IPC_STRUCT_TRAITS_BEGIN(nacl::NaClLaunchParams)
  IPC_STRUCT_TRAITS_MEMBER(manifest_url)
  IPC_STRUCT_TRAITS_MEMBER(render_view_id)
  IPC_STRUCT_TRAITS_MEMBER(permission_bits)
  IPC_STRUCT_TRAITS_MEMBER(uses_irt)
  IPC_STRUCT_TRAITS_MEMBER(enable_dyncode_syscalls)
  IPC_STRUCT_TRAITS_MEMBER(enable_exception_handling)
  IPC_STRUCT_TRAITS_MEMBER(enable_crash_throttling)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(nacl::NaClLaunchResult)
  IPC_STRUCT_TRAITS_MEMBER(imc_channel_handle)
  IPC_STRUCT_TRAITS_MEMBER(ipc_channel_handle)
  IPC_STRUCT_TRAITS_MEMBER(plugin_pid)
  IPC_STRUCT_TRAITS_MEMBER(plugin_child_id)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(nacl::PnaclCacheInfo)
  IPC_STRUCT_TRAITS_MEMBER(pexe_url)
  IPC_STRUCT_TRAITS_MEMBER(abi_version)
  IPC_STRUCT_TRAITS_MEMBER(opt_level)
  IPC_STRUCT_TRAITS_MEMBER(last_modified)
  IPC_STRUCT_TRAITS_MEMBER(etag)
  IPC_STRUCT_TRAITS_MEMBER(has_no_store_header)
IPC_STRUCT_TRAITS_END()

IPC_SYNC_MESSAGE_CONTROL1_2(NaClHostMsg_LaunchNaCl,
                            nacl::NaClLaunchParams ,
                            nacl::NaClLaunchResult ,
                            std::string )

IPC_SYNC_MESSAGE_CONTROL1_1(NaClHostMsg_GetReadonlyPnaclFD,
                            std::string ,
                            IPC::PlatformFileForTransit )

IPC_SYNC_MESSAGE_CONTROL0_1(NaClHostMsg_NaClCreateTemporaryFile,
                            IPC::PlatformFileForTransit )

IPC_MESSAGE_CONTROL3(NaClHostMsg_NexeTempFileRequest,
                     int ,
                     int ,
                     nacl::PnaclCacheInfo )

IPC_MESSAGE_CONTROL3(NaClViewMsg_NexeTempFileReply,
                     int ,
                     bool ,
                     IPC::PlatformFileForTransit )

IPC_MESSAGE_CONTROL2(NaClHostMsg_ReportTranslationFinished,
                     int ,
                     bool )

IPC_MESSAGE_CONTROL2(NaClHostMsg_NaClErrorStatus,
                     int ,
                     int )

IPC_SYNC_MESSAGE_CONTROL2_3(NaClHostMsg_OpenNaClExecutable,
                            int ,
                            GURL ,
                            IPC::PlatformFileForTransit ,
                            uint64 ,
                            uint64 )
