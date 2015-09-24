// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_MESSAGE_BUILDER_H_
#define MOJO_PUBLIC_BINDINGS_LIB_MESSAGE_BUILDER_H_

#include <stdint.h>

#include "mojo/public/bindings/lib/buffer.h"

namespace mojo {
struct MessageData;

namespace internal {

class MessageBuilder {
 public:
  MessageBuilder(uint32_t message_name, size_t payload_size);
  ~MessageBuilder();

  Buffer* buffer() { return &buf_; }

  
  
  
  MessageData* Finish();

 private:
  FixedBuffer buf_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(MessageBuilder);
};

}  
}  

#endif  
