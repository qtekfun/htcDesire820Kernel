// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_MESSAGE_H_
#define DBUS_MESSAGE_H_

#include <string>
#include <vector>
#include <dbus/dbus.h>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "dbus/dbus_export.h"
#include "dbus/file_descriptor.h"
#include "dbus/object_path.h"

namespace google {
namespace protobuf {

class MessageLite;

}  
}  


namespace dbus {

class MessageWriter;
class MessageReader;

#if !defined(DBUS_TYPE_UNIX_FD)
#define DBUS_TYPE_UNIX_FD      ((int) 'h')
#endif

CHROME_DBUS_EXPORT bool IsDBusTypeUnixFdSupported();

class CHROME_DBUS_EXPORT Message {
 public:
  
  
  
  
  enum MessageType {
    MESSAGE_INVALID = DBUS_MESSAGE_TYPE_INVALID,
    MESSAGE_METHOD_CALL = DBUS_MESSAGE_TYPE_METHOD_CALL,
    MESSAGE_METHOD_RETURN = DBUS_MESSAGE_TYPE_METHOD_RETURN,
    MESSAGE_SIGNAL = DBUS_MESSAGE_TYPE_SIGNAL,
    MESSAGE_ERROR = DBUS_MESSAGE_TYPE_ERROR,
 };

  
  
  enum DataType {
    INVALID_DATA = DBUS_TYPE_INVALID,
    BYTE = DBUS_TYPE_BYTE,
    BOOL = DBUS_TYPE_BOOLEAN,
    INT16 = DBUS_TYPE_INT16,
    UINT16 = DBUS_TYPE_UINT16,
    INT32 = DBUS_TYPE_INT32,
    UINT32 = DBUS_TYPE_UINT32,
    INT64 = DBUS_TYPE_INT64,
    UINT64 = DBUS_TYPE_UINT64,
    DOUBLE = DBUS_TYPE_DOUBLE,
    STRING = DBUS_TYPE_STRING,
    OBJECT_PATH = DBUS_TYPE_OBJECT_PATH,
    ARRAY = DBUS_TYPE_ARRAY,
    STRUCT = DBUS_TYPE_STRUCT,
    DICT_ENTRY = DBUS_TYPE_DICT_ENTRY,
    VARIANT = DBUS_TYPE_VARIANT,
    UNIX_FD = DBUS_TYPE_UNIX_FD,
  };

  
  
  MessageType GetMessageType();

  
  
  std::string GetMessageTypeAsString();

  DBusMessage* raw_message() { return raw_message_; }

  
  bool SetDestination(const std::string& destination);
  bool SetPath(const ObjectPath& path);
  bool SetInterface(const std::string& interface);
  bool SetMember(const std::string& member);
  bool SetErrorName(const std::string& error_name);
  bool SetSender(const std::string& sender);
  void SetSerial(uint32 serial);
  void SetReplySerial(uint32 reply_serial);
  

  
  
  std::string GetDestination();
  ObjectPath GetPath();
  std::string GetInterface();
  std::string GetMember();
  std::string GetErrorName();
  std::string GetSender();
  std::string GetSignature();
  
  uint32 GetSerial();
  uint32 GetReplySerial();

  
  
  
  std::string ToString();

 protected:
  
  Message();
  virtual ~Message();

  
  void Init(DBusMessage* raw_message);

 private:
  
  std::string ToStringInternal(const std::string& indent,
                               MessageReader* reader);

  DBusMessage* raw_message_;
  DISALLOW_COPY_AND_ASSIGN(Message);
};

class CHROME_DBUS_EXPORT MethodCall : public Message {
 public:
  
  
  
  
  
  
  
  
  
  
  MethodCall(const std::string& interface_name,
             const std::string& method_name);

  
  
  
  static MethodCall* FromRawMessage(DBusMessage* raw_message);

 private:
  
  
  MethodCall();

  DISALLOW_COPY_AND_ASSIGN(MethodCall);
};

class CHROME_DBUS_EXPORT Signal : public Message {
 public:
  
  
  
  
  
  
  
  
  
  
  Signal(const std::string& interface_name,
         const std::string& method_name);

  
  
  
  static Signal* FromRawMessage(DBusMessage* raw_message);

 private:
  
  
  Signal();

  DISALLOW_COPY_AND_ASSIGN(Signal);
};

class CHROME_DBUS_EXPORT Response : public Message {
 public:
  
  
  static scoped_ptr<Response> FromRawMessage(DBusMessage* raw_message);

  
  
  
  static scoped_ptr<Response> FromMethodCall(MethodCall* method_call);

  
  
  static scoped_ptr<Response> CreateEmpty();

 protected:
  
  Response();

 private:
  DISALLOW_COPY_AND_ASSIGN(Response);
};

class CHROME_DBUS_EXPORT ErrorResponse: public Response {
 public:
  
  
  static scoped_ptr<ErrorResponse> FromRawMessage(DBusMessage* raw_message);

  
  
  
  
  static scoped_ptr<ErrorResponse> FromMethodCall(
      MethodCall* method_call,
      const std::string& error_name,
      const std::string& error_message);

 private:
  
  ErrorResponse();

  DISALLOW_COPY_AND_ASSIGN(ErrorResponse);
};

class CHROME_DBUS_EXPORT MessageWriter {
 public:
  // Data added with Append* will be written to |message|, which may be NULL
  
  explicit MessageWriter(Message* message);
  ~MessageWriter();

  
  void AppendByte(uint8 value);
  void AppendBool(bool value);
  void AppendInt16(int16 value);
  void AppendUint16(uint16 value);
  void AppendInt32(int32 value);
  void AppendUint32(uint32 value);
  void AppendInt64(int64 value);
  void AppendUint64(uint64 value);
  void AppendDouble(double value);
  void AppendString(const std::string& value);
  void AppendObjectPath(const ObjectPath& value);
  void AppendFileDescriptor(const FileDescriptor& value);

  
  
  
  
  
  
  
  
  
  
  
  
  void OpenArray(const std::string& signature, MessageWriter* sub_writer);
  
  void OpenVariant(const std::string& signature, MessageWriter* sub_writer);
  
  void OpenStruct(MessageWriter* sub_writer);
  void OpenDictEntry(MessageWriter* sub_writer);

  
  void CloseContainer(MessageWriter* sub_writer);

  
  
  
  void AppendArrayOfBytes(const uint8* values, size_t length);

  
  
  
  void AppendArrayOfStrings(const std::vector<std::string>& strings);

  
  
  
  void AppendArrayOfObjectPaths(const std::vector<ObjectPath>& object_paths);

  
  
  
  
  
  
  bool AppendProtoAsArrayOfBytes(const google::protobuf::MessageLite& protobuf);

  
  
  
  
  void AppendVariantOfByte(uint8 value);
  void AppendVariantOfBool(bool value);
  void AppendVariantOfInt16(int16 value);
  void AppendVariantOfUint16(uint16 value);
  void AppendVariantOfInt32(int32 value);
  void AppendVariantOfUint32(uint32 value);
  void AppendVariantOfInt64(int64 value);
  void AppendVariantOfUint64(uint64 value);
  void AppendVariantOfDouble(double value);
  void AppendVariantOfString(const std::string& value);
  void AppendVariantOfObjectPath(const ObjectPath& value);

 private:
  
  void AppendBasic(int dbus_type, const void* value);

  
  void AppendVariantOfBasic(int dbus_type, const void* value);

  Message* message_;
  DBusMessageIter raw_message_iter_;
  bool container_is_open_;

  DISALLOW_COPY_AND_ASSIGN(MessageWriter);
};

class CHROME_DBUS_EXPORT MessageReader {
 public:
  
  
  explicit MessageReader(Message* message);
  ~MessageReader();

  
  
  
  
  
  bool HasMoreData();

  
  
  
  bool PopByte(uint8* value);
  bool PopBool(bool* value);
  bool PopInt16(int16* value);
  bool PopUint16(uint16* value);
  bool PopInt32(int32* value);
  bool PopUint32(uint32* value);
  bool PopInt64(int64* value);
  bool PopUint64(uint64* value);
  bool PopDouble(double* value);
  bool PopString(std::string* value);
  bool PopObjectPath(ObjectPath* value);
  bool PopFileDescriptor(FileDescriptor* value);

  
  
  
  
  bool PopArray(MessageReader* sub_reader);
  bool PopStruct(MessageReader* sub_reader);
  bool PopDictEntry(MessageReader* sub_reader);
  bool PopVariant(MessageReader* sub_reader);

  
  
  
  
  
  
  
  
  bool PopArrayOfBytes(uint8** bytes, size_t* length);

  
  
  
  
  
  
  bool PopArrayOfStrings(std::vector<std::string>* strings);

  
  
  
  
  
  
  bool PopArrayOfObjectPaths(std::vector<ObjectPath>* object_paths);

  
  
  
  
  
  
  bool PopArrayOfBytesAsProto(google::protobuf::MessageLite* protobuf);

  
  
  
  
  
  
  
  bool PopVariantOfByte(uint8* value);
  bool PopVariantOfBool(bool* value);
  bool PopVariantOfInt16(int16* value);
  bool PopVariantOfUint16(uint16* value);
  bool PopVariantOfInt32(int32* value);
  bool PopVariantOfUint32(uint32* value);
  bool PopVariantOfInt64(int64* value);
  bool PopVariantOfUint64(uint64* value);
  bool PopVariantOfDouble(double* value);
  bool PopVariantOfString(std::string* value);
  bool PopVariantOfObjectPath(ObjectPath* value);

  
  
  
  Message::DataType GetDataType();

 private:
  
  
  bool CheckDataType(int dbus_type);

  
  bool PopBasic(int dbus_type, void *value);

  
  bool PopContainer(int dbus_type, MessageReader* sub_reader);

  
  bool PopVariantOfBasic(int dbus_type, void* value);

  Message* message_;
  DBusMessageIter raw_message_iter_;

  DISALLOW_COPY_AND_ASSIGN(MessageReader);
};

}  

#endif  
