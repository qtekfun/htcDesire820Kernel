// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_SYSTEM_CORE_H_
#define MOJO_PUBLIC_SYSTEM_CORE_H_


#include <stdint.h>

#include "mojo/public/system/macros.h"
#include "mojo/public/system/system_export.h"




typedef int64_t MojoTimeTicks;


typedef uint32_t MojoHandle;

#ifdef __cplusplus
const MojoHandle MOJO_HANDLE_INVALID = 0;
#else
#define MOJO_HANDLE_INVALID ((MojoHandle) 0)
#endif


typedef int32_t MojoResult;

#ifdef __cplusplus
const MojoResult MOJO_RESULT_OK = 0;
const MojoResult MOJO_RESULT_CANCELLED = -1;
const MojoResult MOJO_RESULT_UNKNOWN = -2;
const MojoResult MOJO_RESULT_INVALID_ARGUMENT = -3;
const MojoResult MOJO_RESULT_DEADLINE_EXCEEDED = -4;
const MojoResult MOJO_RESULT_NOT_FOUND = -5;
const MojoResult MOJO_RESULT_ALREADY_EXISTS = -6;
const MojoResult MOJO_RESULT_PERMISSION_DENIED = -7;
const MojoResult MOJO_RESULT_RESOURCE_EXHAUSTED = -8;
const MojoResult MOJO_RESULT_FAILED_PRECONDITION = -9;
const MojoResult MOJO_RESULT_ABORTED = -10;
const MojoResult MOJO_RESULT_OUT_OF_RANGE = -11;
const MojoResult MOJO_RESULT_UNIMPLEMENTED = -12;
const MojoResult MOJO_RESULT_INTERNAL = -13;
const MojoResult MOJO_RESULT_UNAVAILABLE = -14;
const MojoResult MOJO_RESULT_DATA_LOSS = -15;
const MojoResult MOJO_RESULT_BUSY = -16;
#else
#define MOJO_RESULT_OK ((MojoResult) 0)
#define MOJO_RESULT_CANCELLED ((MojoResult) -1)
#define MOJO_RESULT_UNKNOWN ((MojoResult) -2)
#define MOJO_RESULT_INVALID_ARGUMENT ((MojoResult) -3)
#define MOJO_RESULT_DEADLINE_EXCEEDED ((MojoResult) -4)
#define MOJO_RESULT_NOT_FOUND ((MojoResult) -5)
#define MOJO_RESULT_ALREADY_EXISTS ((MojoResult) -6)
#define MOJO_RESULT_PERMISSION_DENIED ((MojoResult) -7)
#define MOJO_RESULT_RESOURCE_EXHAUSTED ((MojoResult) -8)
#define MOJO_RESULT_FAILED_PRECONDITION ((MojoResult) -9)
#define MOJO_RESULT_ABORTED ((MojoResult) -10)
#define MOJO_RESULT_OUT_OF_RANGE ((MojoResult) -11)
#define MOJO_RESULT_UNIMPLEMENTED ((MojoResult) -12)
#define MOJO_RESULT_INTERNAL ((MojoResult) -13)
#define MOJO_RESULT_UNAVAILABLE ((MojoResult) -14)
#define MOJO_RESULT_DATA_LOSS ((MojoResult) -15)
#define MOJO_RESULT_BUSY ((MojoResult) -16)
#endif


typedef uint64_t MojoDeadline;

#ifdef __cplusplus
const MojoDeadline MOJO_DEADLINE_INDEFINITE = static_cast<MojoDeadline>(-1);
#else
#define MOJO_DEADLINE_INDEFINITE = ((MojoDeadline) -1);
#endif


typedef uint32_t MojoWaitFlags;

#ifdef __cplusplus
const MojoWaitFlags MOJO_WAIT_FLAG_NONE = 0;
const MojoWaitFlags MOJO_WAIT_FLAG_READABLE = 1 << 0;
const MojoWaitFlags MOJO_WAIT_FLAG_WRITABLE = 1 << 1;
const MojoWaitFlags MOJO_WAIT_FLAG_EVERYTHING = ~0;
#else
#define MOJO_WAIT_FLAG_NONE ((MojoWaitFlags) 0)
#define MOJO_WAIT_FLAG_READABLE ((MojoWaitFlags) 1 << 0)
#define MOJO_WAIT_FLAG_WRITABLE ((MojoWaitFlags) 1 << 1)
#define MOJO_WAIT_FLAG_EVERYTHING (~((MojoWaitFlags) 0))
#endif



typedef uint32_t MojoWriteMessageFlags;

#ifdef __cplusplus
const MojoWriteMessageFlags MOJO_WRITE_MESSAGE_FLAG_NONE = 0;
#else
#define MOJO_WRITE_MESSAGE_FLAG_NONE ((MojoWriteMessageFlags) 0)
#endif


typedef uint32_t MojoReadMessageFlags;

#ifdef __cplusplus
const MojoReadMessageFlags MOJO_READ_MESSAGE_FLAG_NONE = 0;
const MojoReadMessageFlags MOJO_READ_MESSAGE_FLAG_MAY_DISCARD = 1 << 0;
#else
#define MOJO_READ_MESSAGE_FLAG_NONE ((MojoReadMessageFlags) 0)
#define MOJO_READ_MESSAGE_FLAG_MAY_DISCARD ((MojoReadMessageFlags) 1 << 0)
#endif


typedef uint32_t MojoCreateDataPipeOptionsFlags;

#ifdef __cplusplus
const MojoCreateDataPipeOptionsFlags
    MOJO_CREATE_DATA_PIPE_OPTIONS_FLAG_NONE = 0;
const MojoCreateDataPipeOptionsFlags
    MOJO_CREATE_DATA_PIPE_OPTIONS_FLAG_MAY_DISCARD = 1 << 0;
#else
#define MOJO_CREATE_DATA_PIPE_OPTIONS_FLAG_NONE \
    ((MojoCreateDataPipeOptionsFlags) 0)
#define MOJO_CREATE_DATA_PIPE_OPTIONS_FLAG_MAY_DISCARD \
    ((MojoCreateDataPipeOptionsFlags) 1 << 0)
#endif

struct MojoCreateDataPipeOptions {
  size_t struct_size;  
  MojoCreateDataPipeOptionsFlags flags;
  uint32_t element_size;  
  uint32_t capacity_num_elements;  
};


typedef uint32_t MojoWriteDataFlags;

#ifdef __cplusplus
const MojoWriteDataFlags MOJO_WRITE_DATA_FLAG_NONE = 0;
const MojoWriteDataFlags MOJO_WRITE_DATA_FLAG_ALL_OR_NONE = 1 << 0;
#else
#define MOJO_WRITE_DATA_FLAG_NONE ((MojoWriteDataFlags) 0)
#define MOJO_WRITE_DATA_FLAG_ALL_OR_NONE ((MojoWriteDataFlags) 1 << 0)
#endif


typedef uint32_t MojoReadDataFlags;

#ifdef __cplusplus
const MojoReadDataFlags MOJO_READ_DATA_FLAG_NONE = 0;
const MojoReadDataFlags MOJO_READ_DATA_FLAG_ALL_OR_NONE = 1 << 0;
const MojoReadDataFlags MOJO_READ_DATA_FLAG_DISCARD = 1 << 1;
const MojoReadDataFlags MOJO_READ_DATA_FLAG_QUERY = 1 << 2;
#else
#define MOJO_READ_DATA_FLAG_NONE ((MojoReadDataFlags) 0)
#define MOJO_READ_DATA_FLAG_ALL_OR_NONE ((MojoReadDataFlags) 1 << 0)
#define MOJO_READ_DATA_FLAG_DISCARD ((MojoReadDataFlags) 1 << 1)
#define MOJO_READ_DATA_FLAG_QUERY ((MojoReadDataFlags) 1 << 2)
#endif


#ifdef __cplusplus
extern "C" {
#endif

MOJO_SYSTEM_EXPORT MojoTimeTicks MojoGetTimeTicksNow();

MOJO_SYSTEM_EXPORT MojoResult MojoClose(MojoHandle handle);

MOJO_SYSTEM_EXPORT MojoResult MojoWait(MojoHandle handle,
                                       MojoWaitFlags flags,
                                       MojoDeadline deadline);

MOJO_SYSTEM_EXPORT MojoResult MojoWaitMany(const MojoHandle* handles,
                                           const MojoWaitFlags* flags,
                                           uint32_t num_handles,
                                           MojoDeadline deadline);


MOJO_SYSTEM_EXPORT MojoResult MojoCreateMessagePipe(
    MojoHandle* message_pipe_handle_0,
    MojoHandle* message_pipe_handle_1);

MOJO_SYSTEM_EXPORT MojoResult MojoWriteMessage(MojoHandle message_pipe_handle,
                                               const void* bytes,
                                               uint32_t num_bytes,
                                               const MojoHandle* handles,
                                               uint32_t num_handles,
                                               MojoWriteMessageFlags flags);

MOJO_SYSTEM_EXPORT MojoResult MojoReadMessage(MojoHandle message_pipe_handle,
                                              void* bytes,
                                              uint32_t* num_bytes,
                                              MojoHandle* handles,
                                              uint32_t* num_handles,
                                              MojoReadMessageFlags flags);


MOJO_SYSTEM_EXPORT MojoResult MojoCreateDataPipe(
    const struct MojoCreateDataPipeOptions* options,
    MojoHandle* data_pipe_producer_handle,
    MojoHandle* data_pipe_consumer_handle);

MOJO_SYSTEM_EXPORT MojoResult MojoWriteData(
    MojoHandle data_pipe_producer_handle,
    const void* elements,
    uint32_t* num_elements,
    MojoWriteDataFlags flags);

MOJO_SYSTEM_EXPORT MojoResult MojoBeginWriteData(
    MojoHandle data_pipe_producer_handle,
    void** buffer,
    uint32_t* buffer_num_elements,
    MojoWriteDataFlags flags);

MOJO_SYSTEM_EXPORT MojoResult MojoEndWriteData(
    MojoHandle data_pipe_producer_handle,
    uint32_t num_elements_written);

MOJO_SYSTEM_EXPORT MojoResult MojoReadData(
    MojoHandle data_pipe_consumer_handle,
    void* elements,
    uint32_t* num_elements,
    MojoReadDataFlags flags);

MOJO_SYSTEM_EXPORT MojoResult MojoBeginReadData(
    MojoHandle data_pipe_consumer_handle,
    const void** buffer,
    uint32_t* buffer_num_elements,
    MojoReadDataFlags flags);

MOJO_SYSTEM_EXPORT MojoResult MojoEndReadData(
    MojoHandle data_pipe_consumer_handle,
    uint32_t num_elements_read);

#ifdef __cplusplus
}  
#endif

#endif  
