// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_FIELD_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_FIELD_H__

#include <map>
#include <string>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>

namespace google {
namespace protobuf {
  namespace io {
    class Printer;             
  }
}

namespace protobuf {
namespace compiler {
namespace cpp {

void SetCommonFieldVariables(const FieldDescriptor* descriptor,
                             map<string, string>* variables,
                             const Options& options);

class FieldGenerator {
 public:
  FieldGenerator() {}
  virtual ~FieldGenerator();

  
  
  
  virtual void GeneratePrivateMembers(io::Printer* printer) const = 0;

  
  
  virtual void GenerateAccessorDeclarations(io::Printer* printer) const = 0;

  
  
  virtual void GenerateInlineAccessorDefinitions(
    io::Printer* printer) const = 0;

  
  
  
  virtual void GenerateNonInlineAccessorDefinitions(
    io::Printer* printer) const {}

  
  
  
  virtual void GenerateClearingCode(io::Printer* printer) const = 0;

  
  
  
  
  
  
  virtual void GenerateMergingCode(io::Printer* printer) const = 0;

  
  
  
  
  
  virtual void GenerateSwappingCode(io::Printer* printer) const = 0;

  
  
  
  virtual void GenerateConstructorCode(io::Printer* printer) const = 0;

  
  
  
  virtual void GenerateDestructorCode(io::Printer* printer) const {}

  
  virtual void GenerateDefaultInstanceAllocator(io::Printer* printer) const {}

  
  
  virtual void GenerateShutdownCode(io::Printer* printer) const {}

  
  
  virtual void GenerateMergeFromCodedStream(io::Printer* printer) const = 0;

  
  
  virtual void GenerateMergeFromCodedStreamWithPacking(io::Printer* printer)
      const;

  
  
  virtual void GenerateSerializeWithCachedSizes(io::Printer* printer) const = 0;

  
  
  // method. This must also advance "target" past the written bytes.
  virtual void GenerateSerializeWithCachedSizesToArray(
      io::Printer* printer) const = 0;

  
  
  virtual void GenerateByteSize(io::Printer* printer) const = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGenerator);
};

class FieldGeneratorMap {
 public:
  explicit FieldGeneratorMap(const Descriptor* descriptor, const Options& options);
  ~FieldGeneratorMap();

  const FieldGenerator& get(const FieldDescriptor* field) const;

 private:
  const Descriptor* descriptor_;
  scoped_array<scoped_ptr<FieldGenerator> > field_generators_;

  static FieldGenerator* MakeGenerator(const FieldDescriptor* field,
                                       const Options& options);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldGeneratorMap);
};


}  
}  
}  

}  
#endif  
