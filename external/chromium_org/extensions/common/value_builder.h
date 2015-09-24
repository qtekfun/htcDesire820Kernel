// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef EXTENSIONS_COMMON_VALUE_BUILDER_H_
#define EXTENSIONS_COMMON_VALUE_BUILDER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/values.h"

namespace extensions {

class ListBuilder;

class DictionaryBuilder {
 public:
  DictionaryBuilder();
  explicit DictionaryBuilder(const base::DictionaryValue& init);
  ~DictionaryBuilder();

  
  
  DictionaryBuilder& Pass() { return *this; }

  
  scoped_ptr<base::DictionaryValue> Build() { return dict_.Pass(); }

  DictionaryBuilder& Set(const std::string& path, int in_value);
  DictionaryBuilder& Set(const std::string& path, double in_value);
  DictionaryBuilder& Set(const std::string& path, const std::string& in_value);
  DictionaryBuilder& Set(const std::string& path, const string16& in_value);
  DictionaryBuilder& Set(const std::string& path, DictionaryBuilder& in_value);
  DictionaryBuilder& Set(const std::string& path, ListBuilder& in_value);

  
  
  DictionaryBuilder& SetBoolean(const std::string& path, bool in_value);

 private:
  scoped_ptr<base::DictionaryValue> dict_;
};

class ListBuilder {
 public:
  ListBuilder();
  explicit ListBuilder(const base::ListValue& init);
  ~ListBuilder();

  
  
  ListBuilder& Pass() { return *this; }

  
  scoped_ptr<base::ListValue> Build() { return list_.Pass(); }

  ListBuilder& Append(int in_value);
  ListBuilder& Append(double in_value);
  ListBuilder& Append(const std::string& in_value);
  ListBuilder& Append(const string16& in_value);
  ListBuilder& Append(DictionaryBuilder& in_value);
  ListBuilder& Append(ListBuilder& in_value);

  
  
  ListBuilder& AppendBoolean(bool in_value);

 private:
  scoped_ptr<base::ListValue> list_;
};

} 

#endif  
