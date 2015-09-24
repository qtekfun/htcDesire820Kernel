// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_VALUES_TEST_UTIL_H_
#define BASE_TEST_VALUES_TEST_UTIL_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"

namespace base {
class DictionaryValue;
class ListValue;
class StringValue;
class Value;


void ExpectDictBooleanValue(bool expected_value,
                            const DictionaryValue& value,
                            const std::string& key);

void ExpectDictDictionaryValue(const DictionaryValue& expected_value,
                               const DictionaryValue& value,
                               const std::string& key);

void ExpectDictIntegerValue(int expected_value,
                            const DictionaryValue& value,
                            const std::string& key);

void ExpectDictListValue(const ListValue& expected_value,
                         const DictionaryValue& value,
                         const std::string& key);

void ExpectDictStringValue(const std::string& expected_value,
                           const DictionaryValue& value,
                           const std::string& key);

void ExpectStringValue(const std::string& expected_str,
                       StringValue* actual);

namespace test {

scoped_ptr<Value> ParseJson(base::StringPiece json);

}  
}  

#endif  
