// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_TEST_UTILS_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_TEST_UTILS_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "extensions/common/manifest.h"

class AsyncExtensionFunction;
class Browser;
class UIThreadExtensionFunction;

namespace base {
class Value;
class DictionaryValue;
class ListValue;
}

namespace extensions {
class Extension;
}

namespace extension_function_test_utils {

base::Value* ParseJSON(const std::string& data);
base::ListValue* ParseList(const std::string& data);
base::DictionaryValue* ParseDictionary(const std::string& data);

bool GetBoolean(base::DictionaryValue* val, const std::string& key);
int GetInteger(base::DictionaryValue* val, const std::string& key);
std::string GetString(base::DictionaryValue* val, const std::string& key);

base::DictionaryValue* ToDictionary(base::Value* val);

base::ListValue* ToList(base::Value* val);

scoped_refptr<extensions::Extension> CreateEmptyExtension();

scoped_refptr<extensions::Extension> CreateEmptyExtensionWithLocation(
    extensions::Manifest::Location location);

scoped_refptr<extensions::Extension> CreateEmptyExtension(
    const std::string& id_input);

scoped_refptr<extensions::Extension> CreateExtension(
    extensions::Manifest::Location location,
    base::DictionaryValue* test_extension_value,
    const std::string& id_input);

scoped_refptr<extensions::Extension> CreateExtension(
    base::DictionaryValue* test_extension_value);

bool HasPrivacySensitiveFields(base::DictionaryValue* val);

enum RunFunctionFlags {
  NONE = 0,
  INCLUDE_INCOGNITO = 1 << 0
};

std::string RunFunctionAndReturnError(UIThreadExtensionFunction* function,
                                      const std::string& args,
                                      Browser* browser,
                                      RunFunctionFlags flags);
std::string RunFunctionAndReturnError(UIThreadExtensionFunction* function,
                                      const std::string& args,
                                      Browser* browser);

base::Value* RunFunctionAndReturnSingleResult(
    UIThreadExtensionFunction* function,
    const std::string& args,
    Browser* browser,
    RunFunctionFlags flags);
base::Value* RunFunctionAndReturnSingleResult(
    UIThreadExtensionFunction* function,
    const std::string& args,
    Browser* browser);

bool RunFunction(UIThreadExtensionFunction* function,
                 const std::string& args,
                 Browser* browser,
                 RunFunctionFlags flags);

} 

#endif  
