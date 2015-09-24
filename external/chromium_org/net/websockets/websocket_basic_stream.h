// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_BASIC_STREAM_H_
#define NET_WEBSOCKETS_WEBSOCKET_BASIC_STREAM_H_

#include <string>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "net/websockets/websocket_frame_parser.h"
#include "net/websockets/websocket_stream.h"

namespace net {

class ClientSocketHandle;
class DrainableIOBuffer;
class GrowableIOBuffer;
class IOBufferWithSize;
struct WebSocketFrame;
struct WebSocketFrameChunk;

class NET_EXPORT_PRIVATE WebSocketBasicStream : public WebSocketStream {
 public:
  typedef WebSocketMaskingKey (*WebSocketMaskingKeyGeneratorFunction)();

  
  
  
  WebSocketBasicStream(
      scoped_ptr<ClientSocketHandle> connection,
      const scoped_refptr<GrowableIOBuffer>& http_read_buffer,
      const std::string& sub_protocol,
      const std::string& extensions);

  
  
  virtual ~WebSocketBasicStream();

  
  virtual int ReadFrames(ScopedVector<WebSocketFrame>* frames,
                         const CompletionCallback& callback) OVERRIDE;

  virtual int WriteFrames(ScopedVector<WebSocketFrame>* frames,
                          const CompletionCallback& callback) OVERRIDE;

  virtual void Close() OVERRIDE;

  virtual std::string GetSubProtocol() const OVERRIDE;

  virtual std::string GetExtensions() const OVERRIDE;

  
  

  static scoped_ptr<WebSocketBasicStream> CreateWebSocketBasicStreamForTesting(
      scoped_ptr<ClientSocketHandle> connection,
      const scoped_refptr<GrowableIOBuffer>& http_read_buffer,
      const std::string& sub_protocol,
      const std::string& extensions,
      WebSocketMaskingKeyGeneratorFunction key_generator_function);

 private:
  
  
  int WriteEverything(const scoped_refptr<DrainableIOBuffer>& buffer,
                      const CompletionCallback& callback);

  // Wraps the |callback| to continue writing until everything has been written.
  void OnWriteComplete(const scoped_refptr<DrainableIOBuffer>& buffer,
                       const CompletionCallback& callback,
                       int result);

  
  
  int HandleReadResult(int result, ScopedVector<WebSocketFrame>* frames);

  
  
  
  
  int ConvertChunksToFrames(ScopedVector<WebSocketFrameChunk>* frame_chunks,
                            ScopedVector<WebSocketFrame>* frames);

  
  
  
  
  
  int ConvertChunkToFrame(scoped_ptr<WebSocketFrameChunk> chunk,
                          scoped_ptr<WebSocketFrame>* frame);

  
  
  
  
  
  
  
  scoped_ptr<WebSocketFrame> CreateFrame(
      bool is_final_chunk,
      const scoped_refptr<IOBufferWithSize>& data);

  
  
  void AddToIncompleteControlFrameBody(
      const scoped_refptr<IOBufferWithSize>& data_buffer);

  
  
  void OnReadComplete(ScopedVector<WebSocketFrame>* frames,
                      const CompletionCallback& callback,
                      int result);

  
  
  
  scoped_refptr<IOBufferWithSize> read_buffer_;

  
  
  scoped_ptr<ClientSocketHandle> connection_;

  
  
  
  
  
  scoped_ptr<WebSocketFrameHeader> current_frame_header_;

  
  
  
  scoped_refptr<GrowableIOBuffer> incomplete_control_frame_body_;

  
  
  
  
  scoped_refptr<GrowableIOBuffer> http_read_buffer_;

  
  
  WebSocketFrameParser parser_;

  
  const std::string sub_protocol_;

  
  const std::string extensions_;

  
  
  
  WebSocketMaskingKeyGeneratorFunction generate_websocket_masking_key_;
};

}  

#endif  
