// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_FEATURES_FEATURE_PROVIDER_H_
#define EXTENSIONS_COMMON_FEATURES_FEATURE_PROVIDER_H_

#include <string>
#include <vector>

namespace extensions {

class Feature;

class FeatureProvider {
 public:
  FeatureProvider() {}
  virtual ~FeatureProvider() {}

  
  virtual Feature* GetFeature(const std::string& name) = 0;

  
  virtual Feature* GetParent(Feature* feature) = 0;

  
  virtual const std::vector<std::string>& GetAllFeatureNames() = 0;

  
  static FeatureProvider* GetByName(const std::string& name);

  
  static FeatureProvider* GetAPIFeatures();
  static FeatureProvider* GetManifestFeatures();
  static FeatureProvider* GetPermissionFeatures();
};

}  

#endif  
