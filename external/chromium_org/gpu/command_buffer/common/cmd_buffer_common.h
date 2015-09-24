// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_COMMON_CMD_BUFFER_COMMON_H_
#define GPU_COMMAND_BUFFER_COMMON_CMD_BUFFER_COMMON_H_

#include <stddef.h>

#include "base/logging.h"
#include "gpu/command_buffer/common/bitfield_helpers.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gpu {

namespace cmd {
  enum ArgFlags {
    kFixed = 0x0,
    kAtLeastN = 0x1
  };
}  

inline uint32 ComputeNumEntries(size_t size_in_bytes) {
  return static_cast<uint32>(
      (size_in_bytes + sizeof(uint32) - 1) / sizeof(uint32));  
}

inline size_t RoundSizeToMultipleOfEntries(size_t size_in_bytes) {
  return ComputeNumEntries(size_in_bytes) * sizeof(uint32);  
}

struct CommandHeader {
  Uint32 size:21;
  Uint32 command:11;

  GPU_EXPORT static const int32 kMaxSize = (1 << 21) - 1;

  void Init(uint32 _command, int32 _size) {
    DCHECK_LE(_size, kMaxSize);
    command = _command;
    size = _size;
  }

  
  
  template <typename T>
  void SetCmd() {
    COMPILE_ASSERT(T::kArgFlags == cmd::kFixed, Cmd_kArgFlags_not_kFixed);
    Init(T::kCmdId, ComputeNumEntries(sizeof(T)));  
  }

  
  template <typename T>
  void SetCmdBySize(uint32 size_of_data_in_bytes) {
    COMPILE_ASSERT(T::kArgFlags == cmd::kAtLeastN, Cmd_kArgFlags_not_kAtLeastN);
    Init(T::kCmdId,
         ComputeNumEntries(sizeof(T) + size_of_data_in_bytes));  
  }

  
  template <typename T>
  void SetCmdByTotalSize(uint32 size_in_bytes) {
    COMPILE_ASSERT(T::kArgFlags == cmd::kAtLeastN, Cmd_kArgFlags_not_kAtLeastN);
    DCHECK_GE(size_in_bytes, sizeof(T));  
    Init(T::kCmdId, ComputeNumEntries(size_in_bytes));
  }
};

COMPILE_ASSERT(sizeof(CommandHeader) == 4, Sizeof_CommandHeader_is_not_4);

union CommandBufferEntry {
  CommandHeader value_header;
  Uint32 value_uint32;
  Int32 value_int32;
  float value_float;
};

const size_t kCommandBufferEntrySize = 4;

COMPILE_ASSERT(sizeof(CommandBufferEntry) == kCommandBufferEntrySize,
               Sizeof_CommandBufferEntry_is_not_4);

#pragma pack(push, 1)

template <typename T>
void* ImmediateDataAddress(T* cmd) {
  COMPILE_ASSERT(T::kArgFlags == cmd::kAtLeastN, Cmd_kArgFlags_not_kAtLeastN);
  return reinterpret_cast<char*>(cmd) + sizeof(*cmd);
}

template <typename T>
void* NextCmdAddress(void* cmd) {
  COMPILE_ASSERT(T::kArgFlags == cmd::kFixed, Cmd_kArgFlags_not_kFixed);
  return reinterpret_cast<char*>(cmd) + sizeof(T);
}

template <typename T>
void* NextImmediateCmdAddress(void* cmd, uint32 size_of_data_in_bytes) {
  COMPILE_ASSERT(T::kArgFlags == cmd::kAtLeastN, Cmd_kArgFlags_not_kAtLeastN);
  return reinterpret_cast<char*>(cmd) + sizeof(T) +   
      RoundSizeToMultipleOfEntries(size_of_data_in_bytes);
}

template <typename T>
void* NextImmediateCmdAddressTotalSize(void* cmd, uint32 total_size_in_bytes) {
  COMPILE_ASSERT(T::kArgFlags == cmd::kAtLeastN, Cmd_kArgFlags_not_kAtLeastN);
  DCHECK_GE(total_size_in_bytes, sizeof(T));  
  return reinterpret_cast<char*>(cmd) +
      RoundSizeToMultipleOfEntries(total_size_in_bytes);
}

namespace cmd {

#define COMMON_COMMAND_BUFFER_CMDS(OP) \
  OP(Noop)                           \
  OP(SetToken)                       \
  OP(SetBucketSize)                  \
  OP(SetBucketData)                  \
  OP(SetBucketDataImmediate)         \
  OP(GetBucketStart)                 \
  OP(GetBucketData)                  \

enum CommandId {
  #define COMMON_COMMAND_BUFFER_CMD_OP(name) k ## name,

  COMMON_COMMAND_BUFFER_CMDS(COMMON_COMMAND_BUFFER_CMD_OP)

  #undef COMMON_COMMAND_BUFFER_CMD_OP

  kNumCommands,
  kLastCommonId = 255  
};

COMPILE_ASSERT(kNumCommands - 1 <= kLastCommonId, Too_many_common_commands);

const char* GetCommandName(CommandId id);

struct Noop {
  typedef Noop ValueType;
  static const CommandId kCmdId = kNoop;
  static const cmd::ArgFlags kArgFlags = cmd::kAtLeastN;

  void SetHeader(uint32 skip_count) {
    DCHECK_GT(skip_count, 0u);
    header.Init(kCmdId, skip_count);
  }

  void Init(uint32 skip_count) {
    SetHeader(skip_count);
  }

  static void* Set(void* cmd, uint32 skip_count) {
    static_cast<ValueType*>(cmd)->Init(skip_count);
    return NextImmediateCmdAddress<ValueType>(
        cmd, skip_count * sizeof(CommandBufferEntry));  
  }

  CommandHeader header;
};

COMPILE_ASSERT(sizeof(Noop) == 4, Sizeof_Noop_is_not_4);
COMPILE_ASSERT(offsetof(Noop, header) == 0, Offsetof_Noop_header_not_0);

struct SetToken {
  typedef SetToken ValueType;
  static const CommandId kCmdId = kSetToken;
  static const cmd::ArgFlags kArgFlags = cmd::kFixed;

  void SetHeader() {
    header.SetCmd<ValueType>();
  }

  void Init(uint32 _token) {
    SetHeader();
    token = _token;
  }
  static void* Set(void* cmd, uint32 token) {
    static_cast<ValueType*>(cmd)->Init(token);
    return NextCmdAddress<ValueType>(cmd);
  }

  CommandHeader header;
  uint32 token;
};

COMPILE_ASSERT(sizeof(SetToken) == 8, Sizeof_SetToken_is_not_8);
COMPILE_ASSERT(offsetof(SetToken, header) == 0,
               Offsetof_SetToken_header_not_0);
COMPILE_ASSERT(offsetof(SetToken, token) == 4,
               Offsetof_SetToken_token_not_4);

struct SetBucketSize {
  typedef SetBucketSize ValueType;
  static const CommandId kCmdId = kSetBucketSize;
  static const cmd::ArgFlags kArgFlags = cmd::kFixed;

  void SetHeader() {
    header.SetCmd<ValueType>();
  }

  void Init(uint32 _bucket_id, uint32 _size) {
    SetHeader();
    bucket_id = _bucket_id;
    size = _size;
  }
  static void* Set(void* cmd, uint32 _bucket_id, uint32 _size) {
    static_cast<ValueType*>(cmd)->Init(_bucket_id, _size);
    return NextCmdAddress<ValueType>(cmd);
  }

  CommandHeader header;
  uint32 bucket_id;
  uint32 size;
};

COMPILE_ASSERT(sizeof(SetBucketSize) == 12, Sizeof_SetBucketSize_is_not_8);
COMPILE_ASSERT(offsetof(SetBucketSize, header) == 0,
               Offsetof_SetBucketSize_header_not_0);
COMPILE_ASSERT(offsetof(SetBucketSize, bucket_id) == 4,
               Offsetof_SetBucketSize_bucket_id_4);
COMPILE_ASSERT(offsetof(SetBucketSize, size) == 8,
               Offsetof_SetBucketSize_size_8);

struct SetBucketData {
  typedef SetBucketData ValueType;
  static const CommandId kCmdId = kSetBucketData;
  static const cmd::ArgFlags kArgFlags = cmd::kFixed;

  void SetHeader() {
    header.SetCmd<ValueType>();
  }

  void Init(uint32 _bucket_id,
            uint32 _offset,
            uint32 _size,
            uint32 _shared_memory_id,
            uint32 _shared_memory_offset) {
    SetHeader();
    bucket_id = _bucket_id;
    offset = _offset;
    size = _size;
    shared_memory_id = _shared_memory_id;
    shared_memory_offset = _shared_memory_offset;
  }
  static void* Set(void* cmd,
                   uint32 _bucket_id,
                   uint32 _offset,
                   uint32 _size,
                   uint32 _shared_memory_id,
                   uint32 _shared_memory_offset) {
    static_cast<ValueType*>(cmd)->Init(
        _bucket_id,
        _offset,
        _size,
        _shared_memory_id,
        _shared_memory_offset);
    return NextCmdAddress<ValueType>(cmd);
  }

  CommandHeader header;
  uint32 bucket_id;
  uint32 offset;
  uint32 size;
  uint32 shared_memory_id;
  uint32 shared_memory_offset;
};

COMPILE_ASSERT(sizeof(SetBucketData) == 24, Sizeof_SetBucketData_is_not_24);
COMPILE_ASSERT(offsetof(SetBucketData, header) == 0,
               Offsetof_SetBucketData_header_not_0);
COMPILE_ASSERT(offsetof(SetBucketData, bucket_id) == 4,
               Offsetof_SetBucketData_bucket_id_not_4);
COMPILE_ASSERT(offsetof(SetBucketData, offset) == 8,
               Offsetof_SetBucketData_offset_not_8);
COMPILE_ASSERT(offsetof(SetBucketData, size) == 12,
               Offsetof_SetBucketData_size_not_12);
COMPILE_ASSERT(offsetof(SetBucketData, shared_memory_id) == 16,
               Offsetof_SetBucketData_shared_memory_id_not_16);
COMPILE_ASSERT(offsetof(SetBucketData, shared_memory_offset) == 20,
               Offsetof_SetBucketData_shared_memory_offset_not_20);

struct SetBucketDataImmediate {
  typedef SetBucketDataImmediate ValueType;
  static const CommandId kCmdId = kSetBucketDataImmediate;
  static const cmd::ArgFlags kArgFlags = cmd::kAtLeastN;

  void SetHeader(uint32 size) {
    header.SetCmdBySize<ValueType>(size);
  }

  void Init(uint32 _bucket_id,
            uint32 _offset,
            uint32 _size) {
    SetHeader(_size);
    bucket_id = _bucket_id;
    offset = _offset;
    size = _size;
  }
  static void* Set(void* cmd,
                   uint32 _bucket_id,
                   uint32 _offset,
                   uint32 _size) {
    static_cast<ValueType*>(cmd)->Init(
        _bucket_id,
        _offset,
        _size);
    return NextImmediateCmdAddress<ValueType>(cmd, _size);
  }

  CommandHeader header;
  uint32 bucket_id;
  uint32 offset;
  uint32 size;
};

COMPILE_ASSERT(sizeof(SetBucketDataImmediate) == 16,
               Sizeof_SetBucketDataImmediate_is_not_24);
COMPILE_ASSERT(offsetof(SetBucketDataImmediate, header) == 0,
               Offsetof_SetBucketDataImmediate_header_not_0);
COMPILE_ASSERT(offsetof(SetBucketDataImmediate, bucket_id) == 4,
               Offsetof_SetBucketDataImmediate_bucket_id_not_4);
COMPILE_ASSERT(offsetof(SetBucketDataImmediate, offset) == 8,
               Offsetof_SetBucketDataImmediate_offset_not_8);
COMPILE_ASSERT(offsetof(SetBucketDataImmediate, size) == 12,
               Offsetof_SetBucketDataImmediate_size_not_12);

struct GetBucketStart {
  typedef GetBucketStart ValueType;
  static const CommandId kCmdId = kGetBucketStart;
  static const cmd::ArgFlags kArgFlags = cmd::kFixed;

  typedef uint32 Result;

  void SetHeader() {
    header.SetCmd<ValueType>();
  }

  void Init(uint32 _bucket_id,
            uint32 _result_memory_id,
            uint32 _result_memory_offset,
            uint32 _data_memory_size,
            uint32 _data_memory_id,
            uint32 _data_memory_offset) {
    SetHeader();
    bucket_id = _bucket_id;
    result_memory_id = _result_memory_id;
    result_memory_offset = _result_memory_offset;
    data_memory_size = _data_memory_size;
    data_memory_id = _data_memory_id;
    data_memory_offset = _data_memory_offset;
  }
  static void* Set(void* cmd,
                   uint32 _bucket_id,
                   uint32 _result_memory_id,
                   uint32 _result_memory_offset,
                   uint32 _data_memory_size,
                   uint32 _data_memory_id,
                   uint32 _data_memory_offset) {
    static_cast<ValueType*>(cmd)->Init(
        _bucket_id,
        _result_memory_id,
        _result_memory_offset,
        _data_memory_size,
        _data_memory_id,
        _data_memory_offset);
    return NextCmdAddress<ValueType>(cmd);
  }

  CommandHeader header;
  uint32 bucket_id;
  uint32 result_memory_id;
  uint32 result_memory_offset;
  uint32 data_memory_size;
  uint32 data_memory_id;
  uint32 data_memory_offset;
};

COMPILE_ASSERT(sizeof(GetBucketStart) == 28, Sizeof_GetBucketStart_is_not_28);
COMPILE_ASSERT(offsetof(GetBucketStart, header) == 0,
               Offsetof_GetBucketStart_header_not_0);
COMPILE_ASSERT(offsetof(GetBucketStart, bucket_id) == 4,
               Offsetof_GetBucketStart_bucket_id_not_4);
COMPILE_ASSERT(offsetof(GetBucketStart, result_memory_id) == 8,
               Offsetof_GetBucketStart_result_memory_id_not_8);
COMPILE_ASSERT(offsetof(GetBucketStart, result_memory_offset) == 12,
               Offsetof_GetBucketStart_result_memory_offset_not_12);
COMPILE_ASSERT(offsetof(GetBucketStart, data_memory_size) == 16,
               Offsetof_GetBucketStart_data_memory_size_not_16);
COMPILE_ASSERT(offsetof(GetBucketStart, data_memory_id) == 20,
               Offsetof_GetBucketStart_data_memory_id_not_20);
COMPILE_ASSERT(offsetof(GetBucketStart, data_memory_offset) == 24,
               Offsetof_GetBucketStart_data_memory_offset_not_24);

struct GetBucketData {
  typedef GetBucketData ValueType;
  static const CommandId kCmdId = kGetBucketData;
  static const cmd::ArgFlags kArgFlags = cmd::kFixed;

  void SetHeader() {
    header.SetCmd<ValueType>();
  }

  void Init(uint32 _bucket_id,
            uint32 _offset,
            uint32 _size,
            uint32 _shared_memory_id,
            uint32 _shared_memory_offset) {
    SetHeader();
    bucket_id = _bucket_id;
    offset = _offset;
    size = _size;
    shared_memory_id = _shared_memory_id;
    shared_memory_offset = _shared_memory_offset;
  }
  static void* Set(void* cmd,
                   uint32 _bucket_id,
                   uint32 _offset,
                   uint32 _size,
                   uint32 _shared_memory_id,
                   uint32 _shared_memory_offset) {
    static_cast<ValueType*>(cmd)->Init(
        _bucket_id,
        _offset,
        _size,
        _shared_memory_id,
        _shared_memory_offset);
    return NextCmdAddress<ValueType>(cmd);
  }

  CommandHeader header;
  uint32 bucket_id;
  uint32 offset;
  uint32 size;
  uint32 shared_memory_id;
  uint32 shared_memory_offset;
};

COMPILE_ASSERT(sizeof(GetBucketData) == 24, Sizeof_GetBucketData_is_not_20);
COMPILE_ASSERT(offsetof(GetBucketData, header) == 0,
               Offsetof_GetBucketData_header_not_0);
COMPILE_ASSERT(offsetof(GetBucketData, bucket_id) == 4,
               Offsetof_GetBucketData_bucket_id_not_4);
COMPILE_ASSERT(offsetof(GetBucketData, offset) == 8,
               Offsetof_GetBucketData_offset_not_8);
COMPILE_ASSERT(offsetof(GetBucketData, size) == 12,
               Offsetof_GetBucketData_size_not_12);
COMPILE_ASSERT(offsetof(GetBucketData, shared_memory_id) == 16,
               Offsetof_GetBucketData_shared_memory_id_not_16);
COMPILE_ASSERT(offsetof(GetBucketData, shared_memory_offset) == 20,
               Offsetof_GetBucketData_shared_memory_offset_not_20);

}  

#pragma pack(pop)

}  

#endif  

