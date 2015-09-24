// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_INPUT_STREAM_H_
#define ANDROID_WEBVIEW_BROWSER_INPUT_STREAM_H_

#include "base/basictypes.h"

namespace net {
class IOBuffer;
}

namespace android_webview {

class InputStream {
 public:
  virtual ~InputStream() {}

  
  
  
  
  
  virtual bool BytesAvailable(int* bytes_available) const = 0;

  
  
  
  
  virtual bool Skip(int64_t n, int64_t* bytes_skipped) = 0;

  
  
  
  
  
  
  virtual bool Read(net::IOBuffer* dest, int length, int* bytes_read) = 0;

 protected:
  InputStream() {}
};

} 

#endif 
