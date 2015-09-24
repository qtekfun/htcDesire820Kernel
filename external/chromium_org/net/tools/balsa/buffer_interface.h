// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_BALSA_BUFFER_INTERFACE_H__
#define NET_TOOLS_BALSA_BUFFER_INTERFACE_H__

namespace net {

class BufferInterface {
 public:

  
  
  virtual int ReadableBytes() const = 0;

  
  
  
  
  virtual int BufferSize() const = 0;

  
  
  
  
  virtual int BytesFree() const = 0;

  
  
  
  
  
  virtual bool Empty() const = 0;

  
  
  virtual bool Full() const = 0;

  
  //   returns the number of characters written.
  
  
  
  
  
  virtual int Write(const char* bytes, int size) = 0;

  
  //   Gets a pointer which can be written to (assigned to).
  
  
  
  
  
  
  
  virtual void GetWritablePtr(char **ptr, int* size) const = 0;

  
  
  
  
  
  
  
  
  
  virtual void GetReadablePtr(char **ptr, int* size) const = 0;

  
  
  
  
  
  
  
  //           and written into
  
  
  
  
  virtual int Read(char* bytes, int size) = 0;

  
  
  virtual void Clear() = 0;

  
  
  
  
  
  virtual bool Reserve(int size) = 0;

  
  
  
  
  
  virtual void AdvanceReadablePtr(int amount_to_advance) = 0;

  
  
  
  //   already be resident (as if it was Written)
  
  
  
  virtual void AdvanceWritablePtr(int amount_to_advance) = 0;

  virtual ~BufferInterface() {}

 protected:
  BufferInterface() {}
};

}  

#endif  

