// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_BALSA_BALSA_VISITOR_INTERFACE_H_
#define NET_TOOLS_BALSA_BALSA_VISITOR_INTERFACE_H_

#include <cstddef>

namespace net {

class BalsaFrame;
class BalsaHeaders;

class BalsaVisitorInterface {
 public:
  virtual ~BalsaVisitorInterface() {}

  
  
  
  
  
  
  
  
  virtual void ProcessBodyInput(const char *input, size_t size) = 0;

  
  
  
  
  
  
  
  
  
  virtual void ProcessBodyData(const char *input, size_t size) = 0;

  
  
  
  
  
  
  virtual void ProcessHeaderInput(const char *input, size_t size) = 0;

  
  
  
  
  
  
  
  
  virtual void ProcessTrailerInput(const char *input, size_t size) = 0;

  
  
  
  
  
  
  
  
  virtual void ProcessHeaders(const BalsaHeaders& headers) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ProcessRequestFirstLine(const char* line_input,
                                       size_t line_length,
                                       const char* method_input,
                                       size_t method_length,
                                       const char* request_uri_input,
                                       size_t request_uri_length,
                                       const char* version_input,
                                       size_t version_length) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ProcessResponseFirstLine(const char *line_input,
                                        size_t line_length,
                                        const char *version_input,
                                        size_t version_length,
                                        const char *status_input,
                                        size_t status_length,
                                        const char *reason_input,
                                        size_t reason_length) = 0;

  
  
  
  virtual void ProcessChunkLength(size_t chunk_length) = 0;

  
  
  
  
  
  
  
  
  
  virtual void ProcessChunkExtensions(const char* input, size_t size) = 0;

  
  
  virtual void HeaderDone() = 0;

  
  
  virtual void MessageDone() = 0;

  
  
  
  
  virtual void HandleHeaderError(BalsaFrame* framer) = 0;

  
  
  
  
  
  virtual void HandleHeaderWarning(BalsaFrame* framer) = 0;

  
  
  
  
  virtual void HandleChunkingError(BalsaFrame* framer) = 0;

  
  
  
  
  
  
  
  virtual void HandleBodyError(BalsaFrame* framer) = 0;
};

}  

#endif  
