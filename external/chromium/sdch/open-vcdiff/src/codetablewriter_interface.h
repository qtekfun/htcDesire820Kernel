// Copyright 2008 Google Inc. All Rights Reserved.

#ifndef OPEN_VCDIFF_CODETABLEWRITER_INTERFACE_H_
#define OPEN_VCDIFF_CODETABLEWRITER_INTERFACE_H_

#include <stddef.h>  

namespace open_vcdiff {

class OutputStringInterface;

class CodeTableWriterInterface {
 public:
  virtual ~CodeTableWriterInterface() { }

  
  virtual void Add(const char* data, size_t size) = 0;

  
  virtual void Copy(int32_t offset, size_t size) = 0;

  
  virtual void Run(size_t size, unsigned char byte) = 0;

  
  
  
  virtual void Output(OutputStringInterface* out) = 0;

  
  
  virtual size_t target_length() const = 0;
};

}  

#endif  
