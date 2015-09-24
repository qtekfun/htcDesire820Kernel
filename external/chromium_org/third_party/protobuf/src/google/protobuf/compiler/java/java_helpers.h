// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_JAVA_HELPERS_H__
#define GOOGLE_PROTOBUF_COMPILER_JAVA_HELPERS_H__

#include <string>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace java {

extern const char kThickSeparator[];
extern const char kThinSeparator[];

string UnderscoresToCamelCase(const FieldDescriptor* field);
string UnderscoresToCapitalizedCamelCase(const FieldDescriptor* field);

string UnderscoresToCamelCase(const MethodDescriptor* method);

string StripProto(const string& filename);

string FileClassName(const FileDescriptor* file);

string FileJavaPackage(const FileDescriptor* file);

string JavaPackageToDir(string package_name);

string ToJavaName(const string& full_name, const FileDescriptor* file);

string ClassName(const Descriptor* descriptor);
string ClassName(const EnumDescriptor* descriptor);
string ClassName(const ServiceDescriptor* descriptor);
string ClassName(const FileDescriptor* descriptor);

inline string ExtensionIdentifierName(const FieldDescriptor* descriptor) {
  return ToJavaName(descriptor->full_name(), descriptor->file());
}

string FieldConstantName(const FieldDescriptor *field);

FieldDescriptor::Type GetType(const FieldDescriptor* field);

enum JavaType {
  JAVATYPE_INT,
  JAVATYPE_LONG,
  JAVATYPE_FLOAT,
  JAVATYPE_DOUBLE,
  JAVATYPE_BOOLEAN,
  JAVATYPE_STRING,
  JAVATYPE_BYTES,
  JAVATYPE_ENUM,
  JAVATYPE_MESSAGE
};

JavaType GetJavaType(const FieldDescriptor* field);

const char* BoxedPrimitiveTypeName(JavaType type);

string DefaultValue(const FieldDescriptor* field);
bool IsDefaultValueJavaDefault(const FieldDescriptor* field);

inline bool HasUnknownFields(const Descriptor* descriptor) {
  return descriptor->file()->options().optimize_for() !=
           FileOptions::LITE_RUNTIME ||
         descriptor->file()->options().retain_unknown_fields();
}

inline bool HasGeneratedMethods(const Descriptor* descriptor) {
  return descriptor->file()->options().optimize_for() !=
           FileOptions::CODE_SIZE;
}

inline bool HasEqualsAndHashCode(const Descriptor* descriptor) {
  return descriptor->file()->options().java_generate_equals_and_hash();
}

inline bool HasDescriptorMethods(const Descriptor* descriptor) {
  return descriptor->file()->options().optimize_for() !=
           FileOptions::LITE_RUNTIME;
}
inline bool HasDescriptorMethods(const EnumDescriptor* descriptor) {
  return descriptor->file()->options().optimize_for() !=
           FileOptions::LITE_RUNTIME;
}
inline bool HasDescriptorMethods(const FileDescriptor* descriptor) {
  return descriptor->options().optimize_for() !=
           FileOptions::LITE_RUNTIME;
}

inline bool HasNestedBuilders(const Descriptor* descriptor) {
  
  return descriptor->file()->options().optimize_for() !=
           FileOptions::LITE_RUNTIME;
}

inline bool HasGenericServices(const FileDescriptor *file) {
  return file->service_count() > 0 &&
         file->options().optimize_for() != FileOptions::LITE_RUNTIME &&
         file->options().java_generic_services();
}



string GetBitFieldName(int index);

string GetBitFieldNameForBit(int bitIndex);

string GenerateGetBit(int bitIndex);

string GenerateSetBit(int bitIndex);

string GenerateClearBit(int bitIndex);

string GenerateGetBitFromLocal(int bitIndex);

string GenerateSetBitToLocal(int bitIndex);

string GenerateGetBitMutableLocal(int bitIndex);

string GenerateSetBitMutableLocal(int bitIndex);

}  
}  
}  

}  
#endif  
