// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_ELEMENT_READER_H_
#define NET_BASE_UPLOAD_ELEMENT_READER_H_

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class IOBuffer;
class UploadBytesElementReader;
class UploadFileElementReader;

class NET_EXPORT UploadElementReader {
 public:
  UploadElementReader() {}
  virtual ~UploadElementReader() {}

  
  
  virtual const UploadBytesElementReader* AsBytesReader() const;

  
  
  virtual const UploadFileElementReader* AsFileReader() const;

  
  
  
  
  virtual int Init(const CompletionCallback& callback) = 0;

  
  
  virtual uint64 GetContentLength() const = 0;

  
  virtual uint64 BytesRemaining() const = 0;

  
  
  virtual bool IsInMemory() const;

  
  
  
  virtual int Read(IOBuffer* buf,
                   int buf_length,
                   const CompletionCallback& callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(UploadElementReader);
};

}  

#endif  
