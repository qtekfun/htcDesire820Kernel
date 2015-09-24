// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

// protocol buffers can be read and written.  For a few simple

#ifndef GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_H__
#define GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_H__

#include <string>
#include <google/protobuf/stubs/common.h>

namespace google {

namespace protobuf {
namespace io {

class ZeroCopyInputStream;
class ZeroCopyOutputStream;

class LIBPROTOBUF_EXPORT ZeroCopyInputStream {
 public:
  inline ZeroCopyInputStream() {}
  virtual ~ZeroCopyInputStream();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool Next(const void** data, int* size) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void BackUp(int count) = 0;

  
  
  
  
  virtual bool Skip(int count) = 0;

  
  virtual int64 ByteCount() const = 0;


 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ZeroCopyInputStream);
};

class LIBPROTOBUF_EXPORT ZeroCopyOutputStream {
 public:
  inline ZeroCopyOutputStream() {}
  virtual ~ZeroCopyOutputStream();

  // Obtains a buffer into which data can be written.  Any data written
  
  // be written to the output.
  
  
  
  
  
  
  
  
  
  
  
  
  
  //   written to the output (unless BackUp() is called).
  
  
  
  virtual bool Next(void** data, int* size) = 0;

  
  // by Next() is not actually written.  This is needed when you finish
  
  
  
  
  
  
  
  
  // * The caller must not have written anything to the last "count" bytes
  
  
  
  
  
  virtual void BackUp(int count) = 0;

  // Returns the total number of bytes written since this object was created.
  virtual int64 ByteCount() const = 0;


 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ZeroCopyOutputStream);
};

}  
}  

}  
#endif  
