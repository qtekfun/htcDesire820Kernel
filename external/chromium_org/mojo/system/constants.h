// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_CONSTANTS_H_
#define MOJO_SYSTEM_CONSTANTS_H_

#include <stddef.h>

namespace mojo {
namespace system {

const size_t kMaxHandleTableSize = 1000000;

const size_t kMaxWaitManyNumHandles = kMaxHandleTableSize;

const size_t kMaxMessageNumBytes = 4 * 1024 * 1024;

const size_t kMaxMessageNumHandles = 10000;

const size_t kMaxDataPipeCapacityBytes = 256 * 1024 * 1024;  

const size_t kDefaultDataPipeCapacityBytes = 1024 * 1024;  

const size_t kDataPipeBufferAlignmentBytes = 16;

}  
}  

#endif  
