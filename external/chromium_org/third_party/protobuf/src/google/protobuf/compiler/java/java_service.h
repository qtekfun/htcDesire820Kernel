// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_SERVICE_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_SERVICE_H__

#include <map>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace java {

class ServiceGenerator {
 public:
  explicit ServiceGenerator(const ServiceDescriptor* descriptor);
  ~ServiceGenerator();

  void Generate(io::Printer* printer);

 private:

  
  void GenerateGetDescriptorForType(io::Printer* printer);

  
  void GenerateInterface(io::Printer* printer);

  
  void GenerateNewReflectiveServiceMethod(io::Printer* printer);

  
  void GenerateNewReflectiveBlockingServiceMethod(io::Printer* printer);

  
  void GenerateAbstractMethods(io::Printer* printer);

  
  void GenerateCallMethod(io::Printer* printer);

  
  void GenerateCallBlockingMethod(io::Printer* printer);

  
  enum RequestOrResponse { REQUEST, RESPONSE };
  void GenerateGetPrototype(RequestOrResponse which, io::Printer* printer);

  
  void GenerateStub(io::Printer* printer);

  
  
  enum IsAbstract { IS_ABSTRACT, IS_CONCRETE };
  void GenerateMethodSignature(io::Printer* printer,
                               const MethodDescriptor* method,
                               IsAbstract is_abstract);

  
  void GenerateBlockingStub(io::Printer* printer);

  
  void GenerateBlockingMethodSignature(io::Printer* printer,
                                       const MethodDescriptor* method);

  const ServiceDescriptor* descriptor_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ServiceGenerator);
};

}  
}  
}  

#endif  
}  
