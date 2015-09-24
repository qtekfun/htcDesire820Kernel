// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_WIRE_FORMAT_H__
#define GOOGLE_PROTOBUF_WIRE_FORMAT_H__

#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/wire_format_lite.h>

#ifndef NDEBUG
#define GOOGLE_PROTOBUF_UTF8_VALIDATION_ENABLED
#endif

namespace google {
namespace protobuf {
  namespace io {
    class CodedInputStream;      
    class CodedOutputStream;     
  }
  class UnknownFieldSet;         
}

namespace protobuf {
namespace internal {

class LIBPROTOBUF_EXPORT WireFormat {
 public:

  
  static inline WireFormatLite::WireType WireTypeForField(
      const FieldDescriptor* field);

  
  static inline WireFormatLite::WireType WireTypeForFieldType(
      FieldDescriptor::Type type);

  
  
  static inline int TagSize(int field_number, FieldDescriptor::Type type);

  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  static bool ParseAndMergePartial(io::CodedInputStream* input,
                                   Message* message);

  
  
  
  
  
  
  
  static void SerializeWithCachedSizes(
      const Message& message,
      int size, io::CodedOutputStream* output);

  
  
  
  
  
  static int ByteSize(const Message& message);


  
  
  
  
  
  static uint32 MakeTag(const FieldDescriptor* field);

  
  
  static bool ParseAndMergeField(
      uint32 tag,
      const FieldDescriptor* field,        
      Message* message,
      io::CodedInputStream* input);

  
  static void SerializeFieldWithCachedSizes(
      const FieldDescriptor* field,        
      const Message& message,
      io::CodedOutputStream* output);

  
  
  
  static int FieldByteSize(
      const FieldDescriptor* field,        
      const Message& message);

  
  
  static bool ParseAndMergeMessageSetItem(
      io::CodedInputStream* input,
      Message* message);
  static void SerializeMessageSetItemWithCachedSizes(
      const FieldDescriptor* field,
      const Message& message,
      io::CodedOutputStream* output);
  static int MessageSetItemByteSize(
      const FieldDescriptor* field,
      const Message& message);

  
  
  
  
  static int FieldDataOnlyByteSize(
      const FieldDescriptor* field,        
      const Message& message);

  enum Operation {
    PARSE,
    SERIALIZE,
  };

  
  static void VerifyUTF8String(const char* data, int size, Operation op);

 private:
  
  static void VerifyUTF8StringFallback(
      const char* data,
      int size,
      Operation op);



  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(WireFormat);
};


inline WireFormatLite::WireType WireFormat::WireTypeForField(
    const FieldDescriptor* field) {
  if (field->options().packed()) {
    return WireFormatLite::WIRETYPE_LENGTH_DELIMITED;
  } else {
    return WireTypeForFieldType(field->type());
  }
}

inline WireFormatLite::WireType WireFormat::WireTypeForFieldType(
    FieldDescriptor::Type type) {
  
  
  return WireFormatLite::WireTypeForFieldType(
      static_cast<WireFormatLite::FieldType>(
        implicit_cast<int>(type)));
}

inline uint32 WireFormat::MakeTag(const FieldDescriptor* field) {
  return WireFormatLite::MakeTag(field->number(), WireTypeForField(field));
}

inline int WireFormat::TagSize(int field_number, FieldDescriptor::Type type) {
  
  
  return WireFormatLite::TagSize(field_number,
      static_cast<WireFormatLite::FieldType>(
        implicit_cast<int>(type)));
}

inline void WireFormat::VerifyUTF8String(const char* data, int size,
    WireFormat::Operation op) {
#ifdef GOOGLE_PROTOBUF_UTF8_VALIDATION_ENABLED
  WireFormat::VerifyUTF8StringFallback(data, size, op);
#else
  
  (void)data; (void)size; (void)op;
#endif
}


}  
}  

}  
#endif  
