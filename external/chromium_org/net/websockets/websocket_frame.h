// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_FRAME_H_
#define NET_WEBSOCKETS_WEBSOCKET_FRAME_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class IOBuffer;
class IOBufferWithSize;

struct NET_EXPORT WebSocketFrameHeader {
  typedef int OpCode;

  
  
  enum OpCodeEnum {
    kOpCodeContinuation = 0x0,
    kOpCodeText = 0x1,
    kOpCodeBinary = 0x2,
    kOpCodeDataUnused = 0x3,
    kOpCodeClose = 0x8,
    kOpCodePing = 0x9,
    kOpCodePong = 0xA,
    kOpCodeControlUnused = 0xB,
  };

  
  
  static bool IsKnownDataOpCode(OpCode opcode) {
    return opcode == kOpCodeContinuation || opcode == kOpCodeText ||
           opcode == kOpCodeBinary;
  }

  
  
  static bool IsKnownControlOpCode(OpCode opcode) {
    return opcode == kOpCodeClose || opcode == kOpCodePing ||
           opcode == kOpCodePong;
  }

  
  
  enum {
    kBaseHeaderSize = 2,
    kMaximumExtendedLengthSize = 8,
    kMaskingKeyLength = 4
  };

  
  explicit WebSocketFrameHeader(OpCode opCode)
      : final(false),
        reserved1(false),
        reserved2(false),
        reserved3(false),
        opcode(opCode),
        masked(false),
        payload_length(0) {}

  
  scoped_ptr<WebSocketFrameHeader> Clone() const;

  
  void CopyFrom(const WebSocketFrameHeader& source);

  
  
  bool final;
  bool reserved1;
  bool reserved2;
  bool reserved3;
  OpCode opcode;
  bool masked;
  uint64 payload_length;

 private:
  DISALLOW_COPY_AND_ASSIGN(WebSocketFrameHeader);
};

struct NET_EXPORT_PRIVATE WebSocketFrame {
  
  explicit WebSocketFrame(WebSocketFrameHeader::OpCode opcode);
  ~WebSocketFrame();

  
  WebSocketFrameHeader header;

  
  
  scoped_refptr<IOBuffer> data;
};

struct NET_EXPORT WebSocketFrameChunk {
  WebSocketFrameChunk();
  ~WebSocketFrameChunk();

  
  
  scoped_ptr<WebSocketFrameHeader> header;

  
  bool final_chunk;

  
  
  scoped_refptr<IOBufferWithSize> data;
};

struct WebSocketMaskingKey {
  char key[WebSocketFrameHeader::kMaskingKeyLength];
};

NET_EXPORT int GetWebSocketFrameHeaderSize(const WebSocketFrameHeader& header);

// the number of bytes written.
NET_EXPORT int WriteWebSocketFrameHeader(const WebSocketFrameHeader& header,
                                         const WebSocketMaskingKey* masking_key,
                                         char* buffer,
                                         int buffer_size);

NET_EXPORT WebSocketMaskingKey GenerateWebSocketMaskingKey();

NET_EXPORT void MaskWebSocketFramePayload(
    const WebSocketMaskingKey& masking_key,
    uint64 frame_offset,
    char* data,
    int data_size);

}  

#endif  
