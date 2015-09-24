// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VALUE_STORE_TESTING_VALUE_STORE_H_
#define CHROME_BROWSER_VALUE_STORE_TESTING_VALUE_STORE_H_

#include "base/compiler_specific.h"
#include "chrome/browser/value_store/value_store.h"

class TestingValueStore : public ValueStore {
 public:
  TestingValueStore();
  virtual ~TestingValueStore();

  
  
  void set_error_code(ErrorCode error_code) { error_code_ = error_code; }

  
  
  
  
  
  int read_count() { return read_count_; }
  int write_count() { return write_count_; }

  
  virtual size_t GetBytesInUse(const std::string& key) OVERRIDE;
  virtual size_t GetBytesInUse(const std::vector<std::string>& keys) OVERRIDE;
  virtual size_t GetBytesInUse() OVERRIDE;
  virtual ReadResult Get(const std::string& key) OVERRIDE;
  virtual ReadResult Get(const std::vector<std::string>& keys) OVERRIDE;
  virtual ReadResult Get() OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options,
      const std::string& key,
      const Value& value) OVERRIDE;
  virtual WriteResult Set(
      WriteOptions options, const DictionaryValue& values) OVERRIDE;
  virtual WriteResult Remove(const std::string& key) OVERRIDE;
  virtual WriteResult Remove(const std::vector<std::string>& keys) OVERRIDE;
  virtual WriteResult Clear() OVERRIDE;

 private:
  scoped_ptr<ValueStore::Error> TestingError();

  DictionaryValue storage_;
  int read_count_;
  int write_count_;
  ErrorCode error_code_;

  DISALLOW_COPY_AND_ASSIGN(TestingValueStore);
};

#endif  
