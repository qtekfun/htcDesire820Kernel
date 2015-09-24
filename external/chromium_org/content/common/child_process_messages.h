// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>
#include <vector>

#include "base/memory/shared_memory.h"
#include "base/tracked_objects.h"
#include "base/values.h"
#include "content/common/content_export.h"
#include "ipc/ipc_message_macros.h"
#include "ui/gfx/gpu_memory_buffer.h"

IPC_ENUM_TRAITS(tracked_objects::ThreadData::Status)

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::LocationSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(file_name)
  IPC_STRUCT_TRAITS_MEMBER(function_name)
  IPC_STRUCT_TRAITS_MEMBER(line_number)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::BirthOnThreadSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(location)
  IPC_STRUCT_TRAITS_MEMBER(thread_name)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::DeathDataSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(count)
  IPC_STRUCT_TRAITS_MEMBER(run_duration_sum)
  IPC_STRUCT_TRAITS_MEMBER(run_duration_max)
  IPC_STRUCT_TRAITS_MEMBER(run_duration_sample)
  IPC_STRUCT_TRAITS_MEMBER(queue_duration_sum)
  IPC_STRUCT_TRAITS_MEMBER(queue_duration_max)
  IPC_STRUCT_TRAITS_MEMBER(queue_duration_sample)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::TaskSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(birth)
  IPC_STRUCT_TRAITS_MEMBER(death_data)
  IPC_STRUCT_TRAITS_MEMBER(death_thread_name)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::ParentChildPairSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(parent)
  IPC_STRUCT_TRAITS_MEMBER(child)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tracked_objects::ProcessDataSnapshot)
  IPC_STRUCT_TRAITS_MEMBER(tasks)
  IPC_STRUCT_TRAITS_MEMBER(descendants)
  IPC_STRUCT_TRAITS_MEMBER(process_id)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(gfx::GpuMemoryBufferType)

IPC_STRUCT_TRAITS_BEGIN(gfx::GpuMemoryBufferHandle)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(handle)
#if defined(OS_MACOSX)
  IPC_STRUCT_TRAITS_MEMBER(io_surface_id)
#endif
IPC_STRUCT_TRAITS_END()

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT

#define IPC_MESSAGE_START ChildProcessMsgStart


IPC_MESSAGE_CONTROL0(ChildProcessMsg_Shutdown)

#if defined(IPC_MESSAGE_LOG_ENABLED)
IPC_MESSAGE_CONTROL1(ChildProcessMsg_SetIPCLoggingEnabled,
                     bool )
#endif

IPC_MESSAGE_CONTROL1(ChildProcessMsg_SetProfilerStatus,
                     tracked_objects::ThreadData::Status )

IPC_MESSAGE_CONTROL1(ChildProcessMsg_GetChildProfilerData,
                     int )

IPC_MESSAGE_CONTROL1(ChildProcessMsg_GetChildHistogramData,
                     int )

IPC_MESSAGE_CONTROL0(ChildProcessMsg_DumpHandles)

#if defined(USE_TCMALLOC)
IPC_MESSAGE_CONTROL0(ChildProcessMsg_GetTcmallocStats)
#endif


IPC_MESSAGE_CONTROL0(ChildProcessHostMsg_ShutdownRequest)

IPC_MESSAGE_CONTROL2(ChildProcessHostMsg_ChildProfilerData,
                     int, 
                     tracked_objects::ProcessDataSnapshot )

IPC_MESSAGE_CONTROL2(ChildProcessHostMsg_ChildHistogramData,
                     int, 
                     std::vector<std::string> )

IPC_SYNC_MESSAGE_CONTROL1_1(ChildProcessHostMsg_GetBrowserHistogram,
                            std::string, 
                            std::string )

IPC_MESSAGE_CONTROL0(ChildProcessHostMsg_DumpHandlesDone)

#if defined(OS_WIN)
IPC_SYNC_MESSAGE_CONTROL1_0(ChildProcessHostMsg_PreCacheFont,
                            LOGFONT )

IPC_MESSAGE_CONTROL0(ChildProcessHostMsg_ReleaseCachedFonts)
#endif  

IPC_SYNC_MESSAGE_CONTROL1_1(ChildProcessHostMsg_SyncAllocateSharedMemory,
                            uint32 ,
                            base::SharedMemoryHandle)

#if defined(USE_TCMALLOC)
IPC_MESSAGE_CONTROL1(ChildProcessHostMsg_TcmallocStats,
                     std::string )
#endif

IPC_SYNC_MESSAGE_CONTROL3_1(ChildProcessHostMsg_SyncAllocateGpuMemoryBuffer,
                            uint32 ,
                            uint32 ,
                            uint32 ,
                            gfx::GpuMemoryBufferHandle)
