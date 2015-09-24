// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PICKLE_H__
#define BASE_PICKLE_H__

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/strings/string16.h"

class Pickle;

class BASE_EXPORT PickleIterator {
 public:
  PickleIterator() : read_ptr_(NULL), read_end_ptr_(NULL) {}
  explicit PickleIterator(const Pickle& pickle);

  
  
  
  
  bool ReadBool(bool* result) WARN_UNUSED_RESULT;
  bool ReadInt(int* result) WARN_UNUSED_RESULT;
  bool ReadLong(long* result) WARN_UNUSED_RESULT;
  bool ReadUInt16(uint16* result) WARN_UNUSED_RESULT;
  bool ReadUInt32(uint32* result) WARN_UNUSED_RESULT;
  bool ReadInt64(int64* result) WARN_UNUSED_RESULT;
  bool ReadUInt64(uint64* result) WARN_UNUSED_RESULT;
  bool ReadFloat(float* result) WARN_UNUSED_RESULT;
  bool ReadString(std::string* result) WARN_UNUSED_RESULT;
  bool ReadWString(std::wstring* result) WARN_UNUSED_RESULT;
  bool ReadString16(base::string16* result) WARN_UNUSED_RESULT;
  bool ReadData(const char** data, int* length) WARN_UNUSED_RESULT;
  bool ReadBytes(const char** data, int length) WARN_UNUSED_RESULT;

  
  
  bool ReadLength(int* result) WARN_UNUSED_RESULT {
    return ReadInt(result) && *result >= 0;
  }

  
  
  bool SkipBytes(int num_bytes) WARN_UNUSED_RESULT {
    return !!GetReadPointerAndAdvance(num_bytes);
  }

 private:
  
  static size_t AlignInt(size_t i, int alignment) {
    return i + (alignment - (i % alignment)) % alignment;
  }

  
  template <typename Type>
  inline bool ReadBuiltinType(Type* result);

  
  template<typename Type>
  inline const char* GetReadPointerAndAdvance();

  
  
  const char* GetReadPointerAndAdvance(int num_bytes);

  
  
  inline const char* GetReadPointerAndAdvance(int num_elements,
                                              size_t size_element);

  
  const char* read_ptr_;
  const char* read_end_ptr_;

  FRIEND_TEST_ALL_PREFIXES(PickleTest, GetReadPointerAndAdvance);
};

// not keep track of the type of data written to it.
class BASE_EXPORT Pickle {
 public:
  
  Pickle();

  
  
  
  explicit Pickle(int header_size);

  
  
  
  
  Pickle(const char* data, int data_len);

  
  Pickle(const Pickle& other);

  
  
  
  
  
  virtual ~Pickle();

  
  Pickle& operator=(const Pickle& other);

  
  size_t size() const { return header_size_ + header_->payload_size; }

  
  const void* data() const { return header_; }

  
  
  
  bool ReadBool(PickleIterator* iter,
                bool* result) const WARN_UNUSED_RESULT {
    return iter->ReadBool(result);
  }
  bool ReadInt(PickleIterator* iter,
               int* result) const WARN_UNUSED_RESULT {
    return iter->ReadInt(result);
  }
  bool ReadLong(PickleIterator* iter,
                long* result) const WARN_UNUSED_RESULT {
    return iter->ReadLong(result);
  }
  bool ReadUInt16(PickleIterator* iter,
                  uint16* result) const WARN_UNUSED_RESULT {
    return iter->ReadUInt16(result);
  }
  bool ReadUInt32(PickleIterator* iter,
                  uint32* result) const WARN_UNUSED_RESULT {
    return iter->ReadUInt32(result);
  }
  bool ReadInt64(PickleIterator* iter,
                 int64* result) const WARN_UNUSED_RESULT {
    return iter->ReadInt64(result);
  }
  bool ReadUInt64(PickleIterator* iter,
                  uint64* result) const WARN_UNUSED_RESULT {
    return iter->ReadUInt64(result);
  }
  bool ReadFloat(PickleIterator* iter,
                 float* result) const WARN_UNUSED_RESULT {
    return iter->ReadFloat(result);
  }
  bool ReadString(PickleIterator* iter,
                  std::string* result) const WARN_UNUSED_RESULT {
    return iter->ReadString(result);
  }
  bool ReadWString(PickleIterator* iter,
                   std::wstring* result) const WARN_UNUSED_RESULT {
    return iter->ReadWString(result);
  }
  bool ReadString16(PickleIterator* iter,
                    base::string16* result) const WARN_UNUSED_RESULT {
    return iter->ReadString16(result);
  }
  
  
  
  
  bool ReadData(PickleIterator* iter,
                const char** data,
                int* length) const WARN_UNUSED_RESULT {
    return iter->ReadData(data, length);
  }
  
  
  
  
  bool ReadBytes(PickleIterator* iter,
                 const char** data,
                 int length) const WARN_UNUSED_RESULT {
    return iter->ReadBytes(data, length);
  }

  
  
  bool ReadLength(PickleIterator* iter,
                  int* result) const  WARN_UNUSED_RESULT {
    return iter->ReadLength(result);
  }

  
  
  
  
  bool WriteBool(bool value) {
    return WriteInt(value ? 1 : 0);
  }
  bool WriteInt(int value) {
    return WritePOD(value);
  }
  
  
  
  
  
  bool WriteLongUsingDangerousNonPortableLessPersistableForm(long value) {
    return WritePOD(value);
  }
  bool WriteUInt16(uint16 value) {
    return WritePOD(value);
  }
  bool WriteUInt32(uint32 value) {
    return WritePOD(value);
  }
  bool WriteInt64(int64 value) {
    return WritePOD(value);
  }
  bool WriteUInt64(uint64 value) {
    return WritePOD(value);
  }
  bool WriteFloat(float value) {
    return WritePOD(value);
  }
  bool WriteString(const std::string& value);
  bool WriteWString(const std::wstring& value);
  bool WriteString16(const base::string16& value);
  
  
  bool WriteData(const char* data, int length);
  
  
  
  bool WriteBytes(const void* data, int length);

  
  
  
  void Reserve(size_t additional_capacity);

  
  struct Header {
    uint32 payload_size;  
  };

  
  
  
  template <class T>
  T* headerT() {
    DCHECK_EQ(header_size_, sizeof(T));
    return static_cast<T*>(header_);
  }
  template <class T>
  const T* headerT() const {
    DCHECK_EQ(header_size_, sizeof(T));
    return static_cast<const T*>(header_);
  }

  
  size_t payload_size() const { return header_->payload_size; }

  const char* payload() const {
    return reinterpret_cast<const char*>(header_) + header_size_;
  }

  
  
  const char* end_of_payload() const {
    
    return header_ ? payload() + payload_size() : NULL;
  }

 protected:
  char* mutable_payload() {
    return reinterpret_cast<char*>(header_) + header_size_;
  }

  size_t capacity_after_header() const {
    return capacity_after_header_;
  }

  
  
  void Resize(size_t new_capacity);

  
  static size_t AlignInt(size_t i, int alignment) {
    return i + (alignment - (i % alignment)) % alignment;
  }

  
  
  static const char* FindNext(size_t header_size,
                              const char* range_start,
                              const char* range_end);

  
  static const int kPayloadUnit;

 private:
  friend class PickleIterator;

  Header* header_;
  size_t header_size_;  
  
  
  size_t capacity_after_header_;
  
  
  size_t write_offset_;

  
  template<size_t length> void WriteBytesStatic(const void* data);

  
  template <typename T> bool WritePOD(const T& data) {
    WriteBytesStatic<sizeof(data)>(&data);
    return true;
  }
  inline void WriteBytesCommon(const void* data, size_t length);

  FRIEND_TEST_ALL_PREFIXES(PickleTest, Resize);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, FindNext);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, FindNextWithIncompleteHeader);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, FindNextOverflow);
};

#endif  
