// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_EXTENSION_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_EXTENSION_H__

#include <string>

#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {
  class FieldDescriptor;       
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace java {

class ExtensionGenerator {
 public:
  explicit ExtensionGenerator(const FieldDescriptor* descriptor);
  ~ExtensionGenerator();

  void Generate(io::Printer* printer);
  void GenerateNonNestedInitializationCode(io::Printer* printer);
  void GenerateRegistrationCode(io::Printer* printer);

 private:
  const FieldDescriptor* descriptor_;
  string scope_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ExtensionGenerator);
};

}  
}  
}  

}  
#endif  
