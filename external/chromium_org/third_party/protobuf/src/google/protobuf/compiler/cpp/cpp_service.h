// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_SERVICE_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_SERVICE_H__

#include <map>
#include <string>
#include <google/protobuf/stubs/common.h>
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

class ServiceGenerator {
 public:
  
  explicit ServiceGenerator(const ServiceDescriptor* descriptor,
                            const Options& options);
  ~ServiceGenerator();

  

  
  
  void GenerateDeclarations(io::Printer* printer);

  

  
  
  void GenerateDescriptorInitializer(io::Printer* printer, int index);

  
  void GenerateImplementation(io::Printer* printer);

 private:
  enum RequestOrResponse { REQUEST, RESPONSE };
  enum VirtualOrNon { VIRTUAL, NON_VIRTUAL };

  

  
  void GenerateInterface(io::Printer* printer);

  
  void GenerateStubDefinition(io::Printer* printer);

  
  void GenerateMethodSignatures(VirtualOrNon virtual_or_non,
                                io::Printer* printer);

  

  
  
  void GenerateNotImplementedMethods(io::Printer* printer);

  
  void GenerateCallMethod(io::Printer* printer);

  
  void GenerateGetPrototype(RequestOrResponse which, io::Printer* printer);

  
  void GenerateStubMethods(io::Printer* printer);

  const ServiceDescriptor* descriptor_;
  map<string, string> vars_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ServiceGenerator);
};

}  
}  
}  

}  
#endif  
