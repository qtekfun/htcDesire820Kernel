// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_NATIVE_INPUT_STREAM_READER_H_
#define ANDROID_WEBVIEW_NATIVE_INPUT_STREAM_READER_H_

#include "base/memory/ref_counted.h"

namespace net {
class HttpByteRange;
class IOBuffer;
}

namespace android_webview {

class InputStream;

class InputStreamReader {
 public:
  
  InputStreamReader(android_webview::InputStream* stream);
  virtual ~InputStreamReader();

  
  
  
  
  
  
  
  
  
  virtual int Seek(const net::HttpByteRange& byte_range);

  
  
  
  
  
  virtual int ReadRawData(net::IOBuffer* buffer, int buffer_size);

 private:
  
  
  int VerifyRequestedRange(net::HttpByteRange* byte_range,
                           int* content_size);

  
  
  int SkipToRequestedRange(const net::HttpByteRange& byte_range);

  android_webview::InputStream* stream_;

  DISALLOW_COPY_AND_ASSIGN(InputStreamReader);
};

} 

#endif 
