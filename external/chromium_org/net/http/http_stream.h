// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_H_
#define NET_HTTP_HTTP_STREAM_H_

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/upload_progress.h"
#include "net/http/http_stream_base.h"

namespace net {

class IOBuffer;

class NET_EXPORT_PRIVATE HttpStream : public HttpStreamBase {
 public:
  HttpStream() {}
  virtual ~HttpStream() {}

  
  virtual UploadProgress GetUploadProgress() const = 0;

  
  
  
  
  
  virtual HttpStream* RenewStreamForAuth() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(HttpStream);
};

}  

#endif  
