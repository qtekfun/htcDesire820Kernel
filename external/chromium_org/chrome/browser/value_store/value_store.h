// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VALUE_STORE_VALUE_STORE_H_
#define CHROME_BROWSER_VALUE_STORE_VALUE_STORE_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/value_store/value_store_change.h"

class ValueStore {
 public:
  
  enum ErrorCode {
    OK,

    
    
    
    
    
    
    
    
    CORRUPTION,

    
    READ_ONLY,

    
    QUOTA_EXCEEDED,

    
    OTHER_ERROR,
  };

  
  struct Error {
    Error(ErrorCode code,
          const std::string& message,
          scoped_ptr<std::string> key);
    ~Error();

    static scoped_ptr<Error> Create(ErrorCode code,
                                    const std::string& message,
                                    scoped_ptr<std::string> key) {
      return make_scoped_ptr(new Error(code, message, key.Pass()));
    }

    
    const ErrorCode code;

    
    const std::string message;

    
    
    
    
    const scoped_ptr<std::string> key;

   private:
    DISALLOW_COPY_AND_ASSIGN(Error);
  };

  
  class ReadResultType {
   public:
    explicit ReadResultType(scoped_ptr<base::DictionaryValue> settings);
    explicit ReadResultType(scoped_ptr<Error> error);
    ~ReadResultType();

    bool HasError() const { return error_; }

    
    
    
    
    
    base::DictionaryValue& settings() { return *settings_; }
    scoped_ptr<base::DictionaryValue> PassSettings() {
      return settings_.Pass();
    }

    
    const Error& error() const { return *error_; }
    scoped_ptr<Error> PassError() { return error_.Pass(); }

   private:
    scoped_ptr<base::DictionaryValue> settings_;
    scoped_ptr<Error> error_;

    DISALLOW_COPY_AND_ASSIGN(ReadResultType);
  };
  typedef scoped_ptr<ReadResultType> ReadResult;

  
  class WriteResultType {
   public:
    explicit WriteResultType(scoped_ptr<ValueStoreChangeList> changes);
    explicit WriteResultType(scoped_ptr<Error> error);
    ~WriteResultType();

    bool HasError() const { return error_; }

    
    
    
    ValueStoreChangeList& changes() { return *changes_; }
    scoped_ptr<ValueStoreChangeList> PassChanges() { return changes_.Pass(); }

    
    const Error& error() const { return *error_; }
    scoped_ptr<Error> PassError() { return error_.Pass(); }

   private:
    scoped_ptr<ValueStoreChangeList> changes_;
    scoped_ptr<Error> error_;

    DISALLOW_COPY_AND_ASSIGN(WriteResultType);
  };
  typedef scoped_ptr<WriteResultType> WriteResult;

  
  enum WriteOptionsValues {
    
    DEFAULTS = 0,

    
    IGNORE_QUOTA = 1<<1,

    
    NO_GENERATE_CHANGES = 1<<2,
  };
  typedef int WriteOptions;

  virtual ~ValueStore() {}

  
  template<typename T>
  static ReadResult MakeReadResult(scoped_ptr<T> arg) {
    return ReadResult(new ReadResultType(arg.Pass()));
  }

  template<typename T>
  static WriteResult MakeWriteResult(scoped_ptr<T> arg) {
    return WriteResult(new WriteResultType(arg.Pass()));
  }

  
  
  
  
  
  virtual size_t GetBytesInUse(const std::string& key) = 0;

  
  virtual size_t GetBytesInUse(const std::vector<std::string>& keys) = 0;

  
  virtual size_t GetBytesInUse() = 0;

  
  virtual ReadResult Get(const std::string& key) = 0;

  
  virtual ReadResult Get(const std::vector<std::string>& keys) = 0;

  
  virtual ReadResult Get() = 0;

  
  virtual WriteResult Set(WriteOptions options,
                          const std::string& key,
                          const base::Value& value) = 0;

  
  virtual WriteResult Set(
      WriteOptions options, const base::DictionaryValue& values) = 0;

  
  virtual WriteResult Remove(const std::string& key) = 0;

  
  virtual WriteResult Remove(const std::vector<std::string>& keys) = 0;

  
  virtual WriteResult Clear() = 0;
};

#endif  
