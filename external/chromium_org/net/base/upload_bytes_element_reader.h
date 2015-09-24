// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_BYTES_ELEMENT_READER_H_
#define NET_BASE_UPLOAD_BYTES_ELEMENT_READER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/upload_element_reader.h"

namespace net {

class NET_EXPORT UploadBytesElementReader : public UploadElementReader {
 public:
  UploadBytesElementReader(const char* bytes, uint64 length);
  virtual ~UploadBytesElementReader();

  const char* bytes() const { return bytes_; }
  uint64 length() const { return length_; }

  
  virtual const UploadBytesElementReader* AsBytesReader() const OVERRIDE;
  virtual int Init(const CompletionCallback& callback) OVERRIDE;
  virtual uint64 GetContentLength() const OVERRIDE;
  virtual uint64 BytesRemaining() const OVERRIDE;
  virtual bool IsInMemory() const OVERRIDE;
  virtual int Read(IOBuffer* buf,
                   int buf_length,
                   const CompletionCallback& callback) OVERRIDE;

 private:
  const char* const bytes_;
  const uint64 length_;
  uint64 offset_;

  DISALLOW_COPY_AND_ASSIGN(UploadBytesElementReader);
};

class NET_EXPORT UploadOwnedBytesElementReader
    : public UploadBytesElementReader {
 public:
  
  explicit UploadOwnedBytesElementReader(std::vector<char>* data);
  virtual ~UploadOwnedBytesElementReader();

  
  static UploadOwnedBytesElementReader* CreateWithString(
      const std::string& string);

 private:
  std::vector<char> data_;

  DISALLOW_COPY_AND_ASSIGN(UploadOwnedBytesElementReader);
};

}  

#endif  
