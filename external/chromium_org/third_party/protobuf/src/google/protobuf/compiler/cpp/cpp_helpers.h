// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_CPP_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_CPP_HELPERS_H__

#include <map>
#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

namespace google {
namespace protobuf {

namespace io {
class Printer;
}

namespace compiler {
namespace cpp {

extern const char kThickSeparator[];
extern const char kThinSeparator[];

string ClassName(const Descriptor* descriptor, bool qualified);
string ClassName(const EnumDescriptor* enum_descriptor, bool qualified);

string SuperClassName(const Descriptor* descriptor);

string FieldName(const FieldDescriptor* field);

string FieldConstantName(const FieldDescriptor *field);

inline const Descriptor* FieldScope(const FieldDescriptor* field) {
  return field->is_extension() ?
    field->extension_scope() : field->containing_type();
}

string FieldMessageTypeName(const FieldDescriptor* field);

string StripProto(const string& filename);

const char* PrimitiveTypeName(FieldDescriptor::CppType type);

const char* DeclaredTypeMethodName(FieldDescriptor::Type type);

string DefaultValue(const FieldDescriptor* field);

string FilenameIdentifier(const string& filename);

string GlobalAddDescriptorsName(const string& filename);

string GlobalAssignDescriptorsName(const string& filename);

string GlobalShutdownFileName(const string& filename);

string EscapeTrigraphs(const string& to_escape);

inline bool HasUnknownFields(const FileDescriptor* file) {
  return file->options().optimize_for() != FileOptions::LITE_RUNTIME ||
         file->options().retain_unknown_fields();
}

bool HasEnumDefinitions(const FileDescriptor* file);

inline bool HasGeneratedMethods(const FileDescriptor* file) {
  return file->options().optimize_for() != FileOptions::CODE_SIZE;
}

inline bool HasDescriptorMethods(const FileDescriptor* file) {
  return file->options().optimize_for() != FileOptions::LITE_RUNTIME;
}

inline bool HasGenericServices(const FileDescriptor* file) {
  return file->service_count() > 0 &&
         file->options().optimize_for() != FileOptions::LITE_RUNTIME &&
         file->options().cc_generic_services();
}

inline bool HasUtf8Verification(const FileDescriptor* file) {
  return file->options().optimize_for() != FileOptions::LITE_RUNTIME;
}

inline bool HasFastArraySerialization(const FileDescriptor* file) {
  return file->options().optimize_for() == FileOptions::SPEED;
}

bool StaticInitializersForced(const FileDescriptor* file);

void PrintHandlingOptionalStaticInitializers(
    const FileDescriptor* file, io::Printer* printer,
    const char* with_static_init, const char* without_static_init,
    const char* var1 = NULL, const string& val1 = "",
    const char* var2 = NULL, const string& val2 = "");

void PrintHandlingOptionalStaticInitializers(
    const map<string, string>& vars, const FileDescriptor* file,
    io::Printer* printer, const char* with_static_init,
    const char* without_static_init);


}  
}  
}  

}  
#endif  
