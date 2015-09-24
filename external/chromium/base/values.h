// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_VALUES_H_
#define BASE_VALUES_H_
#pragma once

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/string16.h"
#include "build/build_config.h"

class BinaryValue;
class DictionaryValue;
class FundamentalValue;
class ListValue;
class StringValue;
class Value;

typedef std::vector<Value*> ValueVector;
typedef std::map<std::string, Value*> ValueMap;

class BASE_API Value {
 public:
  enum ValueType {
    TYPE_NULL = 0,
    TYPE_BOOLEAN,
    TYPE_INTEGER,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BINARY,
    TYPE_DICTIONARY,
    TYPE_LIST
  };

  virtual ~Value();

  
  
  
  static Value* CreateNullValue();
  static FundamentalValue* CreateBooleanValue(bool in_value);
  static FundamentalValue* CreateIntegerValue(int in_value);
  static FundamentalValue* CreateDoubleValue(double in_value);
  static StringValue* CreateStringValue(const std::string& in_value);
  static StringValue* CreateStringValue(const string16& in_value);

  
  
  static BinaryValue* CreateBinaryValue(char* buffer, size_t size);

  
  
  
  
  
  ValueType GetType() const { return type_; }

  
  bool IsType(ValueType type) const { return type == type_; }

  
  
  
  
  virtual bool GetAsBoolean(bool* out_value) const;
  virtual bool GetAsInteger(int* out_value) const;
  virtual bool GetAsDouble(double* out_value) const;
  virtual bool GetAsString(std::string* out_value) const;
  virtual bool GetAsString(string16* out_value) const;
  virtual bool GetAsList(ListValue** out_value);

  
  
  
  
  
  virtual Value* DeepCopy() const;

  
  virtual bool Equals(const Value* other) const;

  
  
  static bool Equals(const Value* a, const Value* b);

 protected:
  
  
  explicit Value(ValueType type);

 private:
  Value();

  ValueType type_;

  DISALLOW_COPY_AND_ASSIGN(Value);
};

class BASE_API FundamentalValue : public Value {
 public:
  explicit FundamentalValue(bool in_value);
  explicit FundamentalValue(int in_value);
  explicit FundamentalValue(double in_value);
  virtual ~FundamentalValue();

  
  virtual bool GetAsBoolean(bool* out_value) const;
  virtual bool GetAsInteger(int* out_value) const;
  virtual bool GetAsDouble(double* out_value) const;
  virtual FundamentalValue* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  union {
    bool boolean_value_;
    int integer_value_;
    double double_value_;
  };

  DISALLOW_COPY_AND_ASSIGN(FundamentalValue);
};

class BASE_API StringValue : public Value {
 public:
  
  explicit StringValue(const std::string& in_value);

  
  explicit StringValue(const string16& in_value);

  virtual ~StringValue();

  
  virtual bool GetAsString(std::string* out_value) const;
  virtual bool GetAsString(string16* out_value) const;
  virtual StringValue* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  std::string value_;

  DISALLOW_COPY_AND_ASSIGN(StringValue);
};

class BASE_API BinaryValue: public Value {
 public:
  virtual ~BinaryValue();

  
  
  
  static BinaryValue* Create(char* buffer, size_t size);

  
  
  
  
  static BinaryValue* CreateWithCopiedBuffer(const char* buffer, size_t size);

  size_t GetSize() const { return size_; }
  char* GetBuffer() { return buffer_; }
  const char* GetBuffer() const { return buffer_; }

  
  virtual BinaryValue* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  
  
  BinaryValue(char* buffer, size_t size);

  char* buffer_;
  size_t size_;

  DISALLOW_COPY_AND_ASSIGN(BinaryValue);
};

class BASE_API DictionaryValue : public Value {
 public:
  DictionaryValue();
  virtual ~DictionaryValue();

  
  bool HasKey(const std::string& key) const;

  
  size_t size() const { return dictionary_.size(); }

  
  bool empty() const { return dictionary_.empty(); }

  
  void Clear();

  
  
  
  
  
  
  
  
  
  void Set(const std::string& path, Value* in_value);

  
  
  void SetBoolean(const std::string& path, bool in_value);
  void SetInteger(const std::string& path, int in_value);
  void SetDouble(const std::string& path, double in_value);
  void SetString(const std::string& path, const std::string& in_value);
  void SetString(const std::string& path, const string16& in_value);

  
  
  void SetWithoutPathExpansion(const std::string& key, Value* in_value);

  
  
  
  
  
  
  
  bool Get(const std::string& path, Value** out_value) const;

  
  
  
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool GetDouble(const std::string& path, double* out_value) const;
  bool GetString(const std::string& path, std::string* out_value) const;
  bool GetString(const std::string& path, string16* out_value) const;
  bool GetStringASCII(const std::string& path, std::string* out_value) const;
  bool GetBinary(const std::string& path, BinaryValue** out_value) const;
  bool GetDictionary(const std::string& path,
                     DictionaryValue** out_value) const;
  bool GetList(const std::string& path, ListValue** out_value) const;

  
  
  bool GetWithoutPathExpansion(const std::string& key,
                               Value** out_value) const;
  bool GetIntegerWithoutPathExpansion(const std::string& key,
                                      int* out_value) const;
  bool GetDoubleWithoutPathExpansion(const std::string& key,
                                   double* out_value) const;
  bool GetStringWithoutPathExpansion(const std::string& key,
                                     std::string* out_value) const;
  bool GetStringWithoutPathExpansion(const std::string& key,
                                     string16* out_value) const;
  bool GetDictionaryWithoutPathExpansion(const std::string& key,
                                         DictionaryValue** out_value) const;
  bool GetListWithoutPathExpansion(const std::string& key,
                                   ListValue** out_value) const;

  
  
  
  
  
  
  bool Remove(const std::string& path, Value** out_value);

  
  
  bool RemoveWithoutPathExpansion(const std::string& key, Value** out_value);

  
  
  DictionaryValue* DeepCopyWithoutEmptyChildren();

  
  
  
  
  void MergeDictionary(const DictionaryValue* dictionary);

  
  
  
  
  
  
  class BASE_API key_iterator
      : private std::iterator<std::input_iterator_tag, const std::string> {
   public:
    explicit key_iterator(ValueMap::const_iterator itr) { itr_ = itr; }
    key_iterator operator++() {
      ++itr_;
      return *this;
    }
    const std::string& operator*() { return itr_->first; }
    bool operator!=(const key_iterator& other) { return itr_ != other.itr_; }
    bool operator==(const key_iterator& other) { return itr_ == other.itr_; }

   private:
    ValueMap::const_iterator itr_;
  };

  key_iterator begin_keys() const { return key_iterator(dictionary_.begin()); }
  key_iterator end_keys() const { return key_iterator(dictionary_.end()); }

  
  virtual DictionaryValue* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  ValueMap dictionary_;

  DISALLOW_COPY_AND_ASSIGN(DictionaryValue);
};

class BASE_API ListValue : public Value {
 public:
  typedef ValueVector::iterator iterator;
  typedef ValueVector::const_iterator const_iterator;

  ListValue();
  ~ListValue();

  
  void Clear();

  
  size_t GetSize() const { return list_.size(); }

  
  bool empty() const { return list_.empty(); }

  
  
  
  
  
  bool Set(size_t index, Value* in_value);

  
  
  
  bool Get(size_t index, Value** out_value) const;

  
  
  
  bool GetBoolean(size_t index, bool* out_value) const;
  bool GetInteger(size_t index, int* out_value) const;
  bool GetDouble(size_t index, double* out_value) const;
  bool GetString(size_t index, std::string* out_value) const;
  bool GetString(size_t index, string16* out_value) const;
  bool GetBinary(size_t index, BinaryValue** out_value) const;
  bool GetDictionary(size_t index, DictionaryValue** out_value) const;
  bool GetList(size_t index, ListValue** out_value) const;

  
  
  
  
  
  bool Remove(size_t index, Value** out_value);

  
  
  int Remove(const Value& value);

  
  void Append(Value* in_value);

  
  
  
  bool AppendIfNotPresent(Value* in_value);

  
  
  bool Insert(size_t index, Value* in_value);

  
  void Swap(ListValue* other) {
    list_.swap(other->list_);
  }

  
  ListValue::iterator begin() { return list_.begin(); }
  ListValue::iterator end() { return list_.end(); }

  ListValue::const_iterator begin() const { return list_.begin(); }
  ListValue::const_iterator end() const { return list_.end(); }

  
  virtual bool GetAsList(ListValue** out_value);
  virtual ListValue* DeepCopy() const;
  virtual bool Equals(const Value* other) const;

 private:
  ValueVector list_;

  DISALLOW_COPY_AND_ASSIGN(ListValue);
};

class BASE_API ValueSerializer {
 public:
  virtual ~ValueSerializer();

  virtual bool Serialize(const Value& root) = 0;

  
  
  
  
  
  
  virtual Value* Deserialize(int* error_code, std::string* error_str) = 0;
};

#endif  
