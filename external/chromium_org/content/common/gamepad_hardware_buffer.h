// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GAMEPAD_HARDWARE_BUFFER_H_
#define CONTENT_COMMON_GAMEPAD_HARDWARE_BUFFER_H_

#include "content/common/one_writer_seqlock.h"
#include "third_party/WebKit/public/platform/WebGamepads.h"

namespace content {


struct GamepadHardwareBuffer {
  
  OneWriterSeqLock sequence;
  blink::WebGamepads buffer;
};

}  

#endif  
