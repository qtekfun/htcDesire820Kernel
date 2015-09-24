// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_IO_BUFFER_H_
#define NET_BASE_IO_BUFFER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT IOBuffer : public base::RefCountedThreadSafe<IOBuffer> {
 public:
  IOBuffer();
  explicit IOBuffer(int buffer_size);

  char* data() { return data_; }

 protected:
  friend class base::RefCountedThreadSafe<IOBuffer>;

  
  
  explicit IOBuffer(char* data);

  virtual ~IOBuffer();

  char* data_;
};

class NET_EXPORT IOBufferWithSize : public IOBuffer {
 public:
  explicit IOBufferWithSize(int size);

  int size() const { return size_; }

 protected:
  
  
  
  IOBufferWithSize(char* data, int size);
  virtual ~IOBufferWithSize();

  int size_;
};

class NET_EXPORT StringIOBuffer : public IOBuffer {
 public:
  explicit StringIOBuffer(const std::string& s);

  int size() const { return static_cast<int>(string_data_.size()); }

 private:
  virtual ~StringIOBuffer();

  std::string string_data_;
};

// to be written progressively, and Write() function takes an IOBuffer rather
// // payload is the IOBuffer containing the data to be written.
//   int bytes_written = Write(buf, buf->BytesRemaining());
//   buf->DidConsume(bytes_written);
class NET_EXPORT DrainableIOBuffer : public IOBuffer {
 public:
  DrainableIOBuffer(IOBuffer* base, int size);

  
  
  void DidConsume(int bytes);

  
  int BytesRemaining() const;

  
  int BytesConsumed() const;

  
  
  void SetOffset(int bytes);

  int size() const { return size_; }

 private:
  virtual ~DrainableIOBuffer();

  scoped_refptr<IOBuffer> base_;
  int size_;
  int used_;
};

class NET_EXPORT GrowableIOBuffer : public IOBuffer {
 public:
  GrowableIOBuffer();

  
  void SetCapacity(int capacity);
  int capacity() { return capacity_; }

  
  void set_offset(int offset);
  int offset() { return offset_; }

  int RemainingCapacity();
  char* StartOfBuffer();

 private:
  virtual ~GrowableIOBuffer();

  scoped_ptr_malloc<char> real_data_;
  int capacity_;
  int offset_;
};

class NET_EXPORT PickledIOBuffer : public IOBuffer {
 public:
  PickledIOBuffer();

  Pickle* pickle() { return &pickle_; }

  
  
  void Done();

 private:
  virtual ~PickledIOBuffer();

  Pickle pickle_;
};

class NET_EXPORT WrappedIOBuffer : public IOBuffer {
 public:
  explicit WrappedIOBuffer(const char* data);

 protected:
  virtual ~WrappedIOBuffer();
};

}  

#endif  
