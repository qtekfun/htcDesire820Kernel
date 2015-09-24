// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_EXTENSION_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_EXTENSION_H__

#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>

namespace google {
namespace protobuf {
  class FieldDescriptor;       
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace cpp {

class ExtensionGenerator {
 public:
  
  explicit ExtensionGenerator(const FieldDescriptor* desycriptor,
                              const Options& options);
  ~ExtensionGenerator();

  
  void GenerateDeclaration(io::Printer* printer);

  
  void GenerateDefinition(io::Printer* printer);

  
  void GenerateRegistration(io::Printer* printer);

 private:
  const FieldDescriptor* descriptor_;
  string type_traits_;
  Options options_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ExtensionGenerator);
};

}  
}  
}  

}  
#endif  
