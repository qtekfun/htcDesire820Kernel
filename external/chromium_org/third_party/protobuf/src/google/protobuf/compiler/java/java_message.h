// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_MESSAGE_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_MESSAGE_H__

#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/java/java_field.h>

namespace google {
namespace protobuf {
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace java {

class MessageGenerator {
 public:
  explicit MessageGenerator(const Descriptor* descriptor);
  ~MessageGenerator();

  
  
  
  void GenerateStaticVariables(io::Printer* printer);

  
  
  void GenerateStaticVariableInitializers(io::Printer* printer);

  
  void Generate(io::Printer* printer);

  
  void GenerateInterface(io::Printer* printer);

  
  
  void GenerateExtensionRegistrationCode(io::Printer* printer);

 private:
  enum UseMemoization {
    MEMOIZE,
    DONT_MEMOIZE
  };

  void GenerateMessageSerializationMethods(io::Printer* printer);
  void GenerateParseFromMethods(io::Printer* printer);
  void GenerateSerializeOneField(io::Printer* printer,
                                 const FieldDescriptor* field);
  void GenerateSerializeOneExtensionRange(
      io::Printer* printer, const Descriptor::ExtensionRange* range);

  void GenerateBuilder(io::Printer* printer);
  void GenerateCommonBuilderMethods(io::Printer* printer);
  void GenerateDescriptorMethods(io::Printer* printer);
  void GenerateBuilderParsingMethods(io::Printer* printer);
  void GenerateIsInitialized(io::Printer* printer,
      UseMemoization useMemoization);
  void GenerateEqualsAndHashCode(io::Printer* printer);

  void GenerateParser(io::Printer* printer);
  void GenerateParsingConstructor(io::Printer* printer);

  const Descriptor* descriptor_;
  FieldGeneratorMap field_generators_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(MessageGenerator);
};

}  
}  
}  

}  
#endif  
