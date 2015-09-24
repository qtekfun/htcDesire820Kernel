// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_ENUM_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_ENUM_H__

#include <string>
#include <google/protobuf/compiler/cpp/cpp_options.h>
#include <google/protobuf/descriptor.h>


namespace google {
namespace protobuf {
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace cpp {

class EnumGenerator {
 public:
  
  explicit EnumGenerator(const EnumDescriptor* descriptor,
                         const Options& options);
  ~EnumGenerator();

  

  
  
  
  void GenerateDefinition(io::Printer* printer);

  
  
  void GenerateGetEnumDescriptorSpecializations(io::Printer* printer);

  
  
  
  
  void GenerateSymbolImports(io::Printer* printer);

  

  
  
  void GenerateDescriptorInitializer(io::Printer* printer, int index);

  
  
  void GenerateMethods(io::Printer* printer);

 private:
  const EnumDescriptor* descriptor_;
  string classname_;
  Options options_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(EnumGenerator);
};

}  
}  
}  

}  
#endif  
