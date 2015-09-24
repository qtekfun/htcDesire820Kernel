// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_COMMON_CONSTANTS_H_
#define GPU_COMMAND_BUFFER_COMMON_CONSTANTS_H_

#include "gpu/command_buffer/common/types.h"

namespace gpu {

typedef int32 CommandBufferOffset;
const CommandBufferOffset kInvalidCommandBufferOffset = -1;

namespace error {
  enum Error {
    kNoError,
    kInvalidSize,
    kOutOfBounds,
    kUnknownCommand,
    kInvalidArguments,
    kLostContext,
    kGenericError,
    kDeferCommandUntilLater
  };

  
  inline bool IsError(Error error) {
    return error != kNoError && error != kDeferCommandUntilLater;
  }

  
  enum ContextLostReason {
    
    kGuilty,

    
    kInnocent,

    
    kUnknown
  };
}

const int32 kInvalidSharedMemoryId = -1;

const int32 kCommandBufferSharedMemoryId = 4;

const size_t kDefaultMaxProgramCacheMemoryBytes = 6 * 1024 * 1024;

}  

#endif  
