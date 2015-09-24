// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_FEATURES_SIMPLE_FEATURE_H_
#define CHROME_COMMON_EXTENSIONS_FEATURES_SIMPLE_FEATURE_H_

#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/values.h"
#include "chrome/common/chrome_version_info.h"
#include "extensions/common/extension.h"
#include "extensions/common/features/feature.h"
#include "extensions/common/manifest.h"

namespace extensions {

class ComplexFeature;

class SimpleFeature : public Feature {
 public:
  SimpleFeature();
  SimpleFeature(const SimpleFeature& other);
  virtual ~SimpleFeature();

  std::set<std::string>* whitelist() { return &whitelist_; }
  std::set<Manifest::Type>* extension_types() { return &extension_types_; }

  
  
  
  
  virtual std::string Parse(const base::DictionaryValue* value);

  
  bool Equals(const SimpleFeature& other) const;

  Location location() const { return location_; }
  void set_location(Location location) { location_ = location; }

  std::set<Platform>* platforms() { return &platforms_; }

  int min_manifest_version() const { return min_manifest_version_; }
  void set_min_manifest_version(int min_manifest_version) {
    min_manifest_version_ = min_manifest_version;
  }

  int max_manifest_version() const { return max_manifest_version_; }
  void set_max_manifest_version(int max_manifest_version) {
    max_manifest_version_ = max_manifest_version;
  }

  Availability IsAvailableToContext(const Extension* extension,
                                    Context context) const {
    return IsAvailableToContext(extension, context, GURL());
  }
  Availability IsAvailableToContext(const Extension* extension,
                                    Context context,
                                    Platform platform) const {
    return IsAvailableToContext(extension, context, GURL(), platform);
  }
  Availability IsAvailableToContext(const Extension* extension,
                                    Context context,
                                    const GURL& url) const {
    return IsAvailableToContext(extension, context, url, GetCurrentPlatform());
  }

  
  virtual Availability IsAvailableToManifest(const std::string& extension_id,
                                             Manifest::Type type,
                                             Location location,
                                             int manifest_version,
                                             Platform platform) const OVERRIDE;

  virtual Availability IsAvailableToContext(const Extension* extension,
                                            Context context,
                                            const GURL& url,
                                            Platform platform) const OVERRIDE;

  virtual std::string GetAvailabilityMessage(AvailabilityResult result,
                                             Manifest::Type type,
                                             const GURL& url,
                                             Context context) const OVERRIDE;

  virtual std::set<Context>* GetContexts() OVERRIDE;

  virtual bool IsInternal() const OVERRIDE;

  virtual bool IsIdInWhitelist(const std::string& extension_id) const OVERRIDE;
  static bool IsIdInWhitelist(const std::string& extension_id,
                              const std::set<std::string>& whitelist);

 protected:
  Availability CreateAvailability(AvailabilityResult result) const;
  Availability CreateAvailability(AvailabilityResult result,
                                  Manifest::Type type) const;
  Availability CreateAvailability(AvailabilityResult result,
                                  const GURL& url) const;
  Availability CreateAvailability(AvailabilityResult result,
                                  Context context) const;

 private:
  
  
  
  
  std::set<std::string> whitelist_;
  std::set<Manifest::Type> extension_types_;
  std::set<Context> contexts_;
  URLPatternSet matches_;
  Location location_;  
  std::set<Platform> platforms_;
  int min_manifest_version_;
  int max_manifest_version_;
  chrome::VersionInfo::Channel channel_;
  bool has_parent_;
  bool channel_has_been_set_;

  FRIEND_TEST_ALL_PREFIXES(ExtensionSimpleFeatureTest, Context);
};

}  

#endif  
