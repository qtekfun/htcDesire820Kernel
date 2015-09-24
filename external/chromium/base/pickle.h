// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PICKLE_H__
#define BASE_PICKLE_H__
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/string16.h"

// not keep track of the type of data written to it.
class BASE_API Pickle {
 public:
  
  Pickle();

  
  
  
  explicit Pickle(int header_size);

  
  
  
  
  Pickle(const char* data, int data_len);

  
  Pickle(const Pickle& other);

  virtual ~Pickle();

  
  Pickle& operator=(const Pickle& other);

  
  size_t size() const { return header_size_ + header_->payload_size; }

  
  const void* data() const { return header_; }

  
  
  
  
  bool ReadBool(void** iter, bool* result) const;
  bool ReadInt(void** iter, int* result) const;
  bool ReadLong(void** iter, long* result) const;
  bool ReadSize(void** iter, size_t* result) const;
  bool ReadUInt16(void** iter, uint16* result) const;
  bool ReadUInt32(void** iter, uint32* result) const;
  bool ReadInt64(void** iter, int64* result) const;
  bool ReadUInt64(void** iter, uint64* result) const;
  bool ReadString(void** iter, std::string* result) const;
  bool ReadWString(void** iter, std::wstring* result) const;
  bool ReadString16(void** iter, string16* result) const;
  bool ReadData(void** iter, const char** data, int* length) const;
  bool ReadBytes(void** iter, const char** data, int length) const;

  
  
  bool ReadLength(void** iter, int* result) const;

  
  
  
  
  bool WriteBool(bool value) {
    return WriteInt(value ? 1 : 0);
  }
  bool WriteInt(int value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteLong(long value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteSize(size_t value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteUInt16(uint16 value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteUInt32(uint32 value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteInt64(int64 value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteUInt64(uint64 value) {
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteString(const std::string& value);
  bool WriteWString(const std::wstring& value);
  bool WriteString16(const string16& value);
  bool WriteData(const char* data, int length);
  bool WriteBytes(const void* data, int data_len);

  
  
  
  
  
  
  
  
  char* BeginWriteData(int length);

  
  
  
  
  
  
  
  
  
  void TrimWriteData(int length);

  
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

  
  
  
  bool IteratorHasRoomFor(const void* iter, int len) const {
    if ((len < 0) || (iter < header_) || iter > end_of_payload())
      return false;
    const char* end_of_region = reinterpret_cast<const char*>(iter) + len;
    
    return (iter <= end_of_region) && (end_of_region <= end_of_payload());
  }

 protected:
  size_t payload_size() const { return header_->payload_size; }

  char* payload() {
    return reinterpret_cast<char*>(header_) + header_size_;
  }
  const char* payload() const {
    return reinterpret_cast<const char*>(header_) + header_size_;
  }

  
  
  char* end_of_payload() {
    
    return payload() + payload_size();
  }
  const char* end_of_payload() const {
    
    return header_ ? payload() + payload_size() : NULL;
  }

  size_t capacity() const {
    return capacity_;
  }

  
  // location that the data should be written at is returned, or NULL if there
  
  
  char* BeginWrite(size_t length);

  
  
  // have to be called after the data is written.
  void EndWrite(char* dest, int length);

  
  
  
  
  bool Resize(size_t new_capacity);

  
  static size_t AlignInt(size_t i, int alignment) {
    return i + (alignment - (i % alignment)) % alignment;
  }

  
  
  
  static void UpdateIter(void** iter, int bytes) {
    *iter = static_cast<char*>(*iter) + AlignInt(bytes, sizeof(uint32));
  }

  
  
  static const char* FindNext(size_t header_size,
                              const char* range_start,
                              const char* range_end);

  
  static const int kPayloadUnit;

 private:
  Header* header_;
  size_t header_size_;  
  
  size_t capacity_;
  size_t variable_buffer_offset_;  

  FRIEND_TEST_ALL_PREFIXES(PickleTest, Resize);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, FindNext);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, FindNextWithIncompleteHeader);
  FRIEND_TEST_ALL_PREFIXES(PickleTest, IteratorHasRoom);
};

#endif  