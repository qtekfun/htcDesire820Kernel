// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_REGISTRY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_FUNCTION_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "chrome/browser/extensions/extension_function_histogram_value.h"

class ExtensionFunction;

typedef ExtensionFunction* (*ExtensionFunctionFactory)();

template<class T>
ExtensionFunction* NewExtensionFunction() {
  return new T();
}

class ExtensionFunctionRegistry {
 public:
  static ExtensionFunctionRegistry* GetInstance();
  explicit ExtensionFunctionRegistry();
  virtual ~ExtensionFunctionRegistry();

  
  void ResetFunctions();

  
  void GetAllNames(std::vector<std::string>* names);

  
  
  bool OverrideFunction(const std::string& name,
                        ExtensionFunctionFactory factory);

  
  ExtensionFunction* NewFunction(const std::string& name);

  template<class T>
  void RegisterFunction() {
    ExtensionFunctionFactory factory = &NewExtensionFunction<T>;
    factories_[T::function_name()] =
        FactoryEntry(factory, T::histogram_value());
  }

  struct FactoryEntry {
   public:
    explicit FactoryEntry();
    explicit FactoryEntry(ExtensionFunctionFactory factory,
      extensions::functions::HistogramValue histogram_value);

    ExtensionFunctionFactory factory_;
    extensions::functions::HistogramValue histogram_value_;
  };

  typedef std::map<std::string, FactoryEntry> FactoryMap;
  FactoryMap factories_;
};

#endif  
