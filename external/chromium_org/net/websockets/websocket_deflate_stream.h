// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_DEFLATE_STREAM_H_
#define NET_WEBSOCKETS_WEBSOCKET_DEFLATE_STREAM_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/websockets/websocket_deflater.h"
#include "net/websockets/websocket_frame.h"
#include "net/websockets/websocket_inflater.h"
#include "net/websockets/websocket_stream.h"

class GURL;

namespace net {

class WebSocketDeflatePredictor;

// a message m1 is read / written before the last frame of a message m2,
class NET_EXPORT_PRIVATE WebSocketDeflateStream : public WebSocketStream {
 public:
  WebSocketDeflateStream(scoped_ptr<WebSocketStream> stream,
                         WebSocketDeflater::ContextTakeOverMode mode,
                         scoped_ptr<WebSocketDeflatePredictor> predictor);
  virtual ~WebSocketDeflateStream();

  
  virtual int ReadFrames(ScopedVector<WebSocketFrame>* frames,
                         const CompletionCallback& callback) OVERRIDE;
  virtual int WriteFrames(ScopedVector<WebSocketFrame>* frames,
                          const CompletionCallback& callback) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual std::string GetSubProtocol() const OVERRIDE;
  virtual std::string GetExtensions() const OVERRIDE;

 private:
  enum ReadingState {
    READING_COMPRESSED_MESSAGE,
    READING_UNCOMPRESSED_MESSAGE,
    NOT_READING,
  };

  enum WritingState {
    WRITING_COMPRESSED_MESSAGE,
    WRITING_UNCOMPRESSED_MESSAGE,
    WRITING_POSSIBLY_COMPRESSED_MESSAGE,
    NOT_WRITING,
  };

  void OnReadComplete(ScopedVector<WebSocketFrame>* frames,
                      const CompletionCallback& callback,
                      int result);

  
  int Deflate(ScopedVector<WebSocketFrame>* frames);
  void OnMessageStart(const ScopedVector<WebSocketFrame>& frames, size_t index);
  int AppendCompressedFrame(const WebSocketFrameHeader& header,
                            ScopedVector<WebSocketFrame>* frames_to_write);
  int AppendPossiblyCompressedMessage(
      ScopedVector<WebSocketFrame>* frames,
      ScopedVector<WebSocketFrame>* frames_to_write);

  
  int Inflate(ScopedVector<WebSocketFrame>* frames);

  int InflateAndReadIfNecessary(ScopedVector<WebSocketFrame>* frames,
                                const CompletionCallback& callback);

  const scoped_ptr<WebSocketStream> stream_;
  WebSocketDeflater deflater_;
  WebSocketInflater inflater_;
  ReadingState reading_state_;
  WritingState writing_state_;
  WebSocketFrameHeader::OpCode current_reading_opcode_;
  WebSocketFrameHeader::OpCode current_writing_opcode_;
  scoped_ptr<WebSocketDeflatePredictor> predictor_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketDeflateStream);
};

}  

#endif  
