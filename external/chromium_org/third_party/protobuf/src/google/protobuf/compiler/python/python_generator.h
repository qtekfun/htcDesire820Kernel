// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_PYTHON_GENERATOR_H__
#define GOOGLE_PROTOBUF_COMPILER_PYTHON_GENERATOR_H__

#include <string>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/stubs/common.h>

namespace google {
namespace protobuf {

class Descriptor;
class EnumDescriptor;
class EnumValueDescriptor;
class FieldDescriptor;
class ServiceDescriptor;

namespace io { class Printer; }

namespace compiler {
namespace python {

class LIBPROTOC_EXPORT Generator : public CodeGenerator {
 public:
  Generator();
  virtual ~Generator();

  
  virtual bool Generate(const FileDescriptor* file,
                        const string& parameter,
                        GeneratorContext* generator_context,
                        string* error) const;

 private:
  void PrintImports() const;
  void PrintFileDescriptor() const;
  void PrintTopLevelEnums() const;
  void PrintAllNestedEnumsInFile() const;
  void PrintNestedEnums(const Descriptor& descriptor) const;
  void PrintEnum(const EnumDescriptor& enum_descriptor) const;

  void PrintTopLevelExtensions() const;

  void PrintFieldDescriptor(
      const FieldDescriptor& field, bool is_extension) const;
  void PrintFieldDescriptorsInDescriptor(
      const Descriptor& message_descriptor,
      bool is_extension,
      const string& list_variable_name,
      int (Descriptor::*CountFn)() const,
      const FieldDescriptor* (Descriptor::*GetterFn)(int) const) const;
  void PrintFieldsInDescriptor(const Descriptor& message_descriptor) const;
  void PrintExtensionsInDescriptor(const Descriptor& message_descriptor) const;
  void PrintMessageDescriptors() const;
  void PrintDescriptor(const Descriptor& message_descriptor) const;
  void PrintNestedDescriptors(const Descriptor& containing_descriptor) const;

  void PrintMessages() const;
  void PrintMessage(const Descriptor& message_descriptor) const;
  void PrintNestedMessages(const Descriptor& containing_descriptor) const;

  void FixForeignFieldsInDescriptors() const;
  void FixForeignFieldsInDescriptor(
      const Descriptor& descriptor,
      const Descriptor* containing_descriptor) const;
  void FixForeignFieldsInField(const Descriptor* containing_type,
                               const FieldDescriptor& field,
                               const string& python_dict_name) const;
  void AddMessageToFileDescriptor(const Descriptor& descriptor) const;
  string FieldReferencingExpression(const Descriptor* containing_type,
                                    const FieldDescriptor& field,
                                    const string& python_dict_name) const;
  template <typename DescriptorT>
  void FixContainingTypeInDescriptor(
      const DescriptorT& descriptor,
      const Descriptor* containing_descriptor) const;

  void FixForeignFieldsInExtensions() const;
  void FixForeignFieldsInExtension(
      const FieldDescriptor& extension_field) const;
  void FixForeignFieldsInNestedExtensions(const Descriptor& descriptor) const;

  void PrintServices() const;
  void PrintServiceDescriptor(const ServiceDescriptor& descriptor) const;
  void PrintServiceClass(const ServiceDescriptor& descriptor) const;
  void PrintServiceStub(const ServiceDescriptor& descriptor) const;

  void PrintEnumValueDescriptor(const EnumValueDescriptor& descriptor) const;
  string OptionsValue(const string& class_name,
                      const string& serialized_options) const;
  bool GeneratingDescriptorProto() const;

  template <typename DescriptorT>
  string ModuleLevelDescriptorName(const DescriptorT& descriptor) const;
  string ModuleLevelMessageName(const Descriptor& descriptor) const;
  string ModuleLevelServiceDescriptorName(
      const ServiceDescriptor& descriptor) const;

  template <typename DescriptorT, typename DescriptorProtoT>
  void PrintSerializedPbInterval(
      const DescriptorT& descriptor, DescriptorProtoT& proto) const;

  void FixAllDescriptorOptions() const;
  void FixOptionsForField(const FieldDescriptor& field) const;
  void FixOptionsForEnum(const EnumDescriptor& descriptor) const;
  void FixOptionsForMessage(const Descriptor& descriptor) const;

  
  
  mutable Mutex mutex_;
  mutable const FileDescriptor* file_;  
  mutable string file_descriptor_serialized_;
  mutable io::Printer* printer_;  

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Generator);
};

}  
}  
}  

}  
#endif  
