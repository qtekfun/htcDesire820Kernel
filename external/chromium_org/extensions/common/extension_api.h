// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_EXTENSION_API_H_
#define EXTENSIONS_COMMON_EXTENSION_API_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"
#include "base/values.h"
#include "extensions/common/features/feature.h"
#include "extensions/common/features/feature_provider.h"
#include "extensions/common/url_pattern_set.h"

namespace base {
class DictionaryValue;
class Value;
}

class GURL;

namespace extensions {

class Extension;
class Feature;

class ExtensionAPI {
 public:
  
  
  
  
  static ExtensionAPI* GetSharedInstance();

  
  
  static ExtensionAPI* CreateWithDefaultConfiguration();

  
  
  
  static void SplitDependencyName(const std::string& full_name,
                                  std::string* feature_type,
                                  std::string* feature_name);

  
  
  ExtensionAPI();
  virtual ~ExtensionAPI();

  void RegisterSchemaResource(const std::string& api_name, int resource_id);

  void RegisterDependencyProvider(const std::string& name,
                                  FeatureProvider* provider);

  
  
  
  
  Feature::Availability IsAvailable(const std::string& api_full_name,
                                    const Extension* extension,
                                    Feature::Context context,
                                    const GURL& url);

  
  
  bool IsAnyFeatureAvailableToContext(const std::string& api_name,
                                      const Extension* extension,
                                      Feature::Context context,
                                      const GURL& url);

  
  
  
  
  bool IsPrivileged(const std::string& name);

  
  
  const base::DictionaryValue* GetSchema(const std::string& full_name);

  
  
  
  
  
  
  
  
  
  std::string GetAPINameFromFullName(const std::string& full_name,
                                     std::string* child_name);

 private:
  FRIEND_TEST_ALL_PREFIXES(ExtensionAPITest, DefaultConfigurationFeatures);
  FRIEND_TEST_ALL_PREFIXES(ExtensionAPITest, TypesHaveNamespace);
  friend struct DefaultSingletonTraits<ExtensionAPI>;

  void InitDefaultConfiguration();

  bool default_configuration_initialized_;

  
  
  Feature* GetFeatureDependency(const std::string& dependency_name);

  
  void LoadSchema(const std::string& name, const base::StringPiece& schema);

  
  
  typedef std::map<std::string, int> UnloadedSchemaMap;
  UnloadedSchemaMap unloaded_schemas_;

  
  typedef std::map<std::string, linked_ptr<const base::DictionaryValue> >
        SchemaMap;
  SchemaMap schemas_;

  
  typedef std::map<std::string, FeatureProvider*> FeatureProviderMap;
  FeatureProviderMap dependency_providers_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionAPI);
};

} 

#endif  
