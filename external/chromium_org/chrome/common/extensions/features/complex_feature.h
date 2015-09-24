// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_FEATURES_COMPLEX_FEATURE_H_
#define CHROME_COMMON_EXTENSIONS_FEATURES_COMPLEX_FEATURE_H_

#include <string>

#include "base/memory/scoped_vector.h"
#include "extensions/common/extension.h"
#include "extensions/common/features/feature.h"
#include "extensions/common/manifest.h"

namespace extensions {

class ComplexFeature : public Feature {
 public:
  typedef ScopedVector<Feature> FeatureList;

  explicit ComplexFeature(scoped_ptr<FeatureList> features);
  virtual ~ComplexFeature();

  
  virtual Availability IsAvailableToManifest(const std::string& extension_id,
                                             Manifest::Type type,
                                             Location location,
                                             int manifest_version,
                                             Platform platform) const OVERRIDE;

  virtual Availability IsAvailableToContext(const Extension* extension,
                                            Context context,
                                            const GURL& url,
                                            Platform platform) const OVERRIDE;

  virtual bool IsIdInWhitelist(const std::string& extension_id) const OVERRIDE;

 protected:
  
  virtual std::string GetAvailabilityMessage(
      AvailabilityResult result,
      Manifest::Type type,
      const GURL& url,
      Context context) const OVERRIDE;

  virtual std::set<Context>* GetContexts() OVERRIDE;

  virtual bool IsInternal() const OVERRIDE;

 private:
  FeatureList features_;

  DISALLOW_COPY_AND_ASSIGN(ComplexFeature);
};

}  

#endif  
