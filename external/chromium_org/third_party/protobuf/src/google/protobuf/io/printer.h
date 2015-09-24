// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_IO_PRINTER_H__
#define GOOGLE_PROTOBUF_IO_PRINTER_H__

#include <string>
#include <map>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {
namespace io {

class ZeroCopyOutputStream;     

class LIBPROTOBUF_EXPORT Printer {
 public:
  
  
  Printer(ZeroCopyOutputStream* output, char variable_delimiter);
  ~Printer();

  
  
  
  
  
  void Print(const map<string, string>& variables, const char* text);

  
  void Print(const char* text);
  
  void Print(const char* text, const char* variable, const string& value);
  
  void Print(const char* text, const char* variable1, const string& value1,
                               const char* variable2, const string& value2);
  
  void Print(const char* text, const char* variable1, const string& value1,
                               const char* variable2, const string& value2,
                               const char* variable3, const string& value3);
  
  

  
  
  
  void Indent();

  
  
  void Outdent();

  
  
  void PrintRaw(const string& data);

  
  
  void PrintRaw(const char* data);

  
  
  void WriteRaw(const char* data, int size);

  
  
  
  bool failed() const { return failed_; }

 private:
  const char variable_delimiter_;

  ZeroCopyOutputStream* const output_;
  char* buffer_;
  int buffer_size_;

  string indent_;
  bool at_start_of_line_;
  bool failed_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Printer);
};

}  
}  

}  
#endif  
