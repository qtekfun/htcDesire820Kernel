// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_UTIL_H_
#define REMOTING_PROTOCOL_UTIL_H_

#include "net/base/io_buffer.h"

#if defined(USE_SYSTEM_PROTOBUF)
#include <google/protobuf/message_lite.h>
#else
#include "third_party/protobuf/src/google/protobuf/message_lite.h"
#endif

namespace remoting {
namespace protocol {

scoped_refptr<net::IOBufferWithSize> SerializeAndFrameMessage(
    const google::protobuf::MessageLite& msg);

}  
}  

#endif  
