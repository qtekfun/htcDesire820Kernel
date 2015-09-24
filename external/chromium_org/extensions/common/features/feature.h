// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_FEATURES_FEATURE_H_
#define EXTENSIONS_COMMON_FEATURES_FEATURE_H_

#include <set>
#include <string>

#include "base/values.h"
#include "extensions/common/manifest.h"

class GURL;

namespace extensions {

class Extension;

class Feature {
 public:
  
  enum Context {
    UNSPECIFIED_CONTEXT,

    
    BLESSED_EXTENSION_CONTEXT,

    
    UNBLESSED_EXTENSION_CONTEXT,

    
    CONTENT_SCRIPT_CONTEXT,

    
    WEB_PAGE_CONTEXT,

    
    
    
    BLESSED_WEB_PAGE_CONTEXT,
  };

  
  enum Location {
    UNSPECIFIED_LOCATION,
    COMPONENT_LOCATION
  };

  
  enum Platform {
    UNSPECIFIED_PLATFORM,
    CHROMEOS_PLATFORM,
    LINUX_PLATFORM,
    MACOSX_PLATFORM,
    WIN_PLATFORM
  };

  
  
  enum AvailabilityResult {
    IS_AVAILABLE,
    NOT_FOUND_IN_WHITELIST,
    INVALID_URL,
    INVALID_TYPE,
    INVALID_CONTEXT,
    INVALID_LOCATION,
    INVALID_PLATFORM,
    INVALID_MIN_MANIFEST_VERSION,
    INVALID_MAX_MANIFEST_VERSION,
    NOT_PRESENT,
    UNSUPPORTED_CHANNEL,
  };

  
  
  class Availability {
   public:
    AvailabilityResult result() const { return result_; }
    bool is_available() const { return result_ == IS_AVAILABLE; }
    const std::string& message() const { return message_; }

   private:
    friend class SimpleFeature;
    friend class Feature;

    
    Availability(AvailabilityResult result, const std::string& message)
        : result_(result), message_(message) { }

    const AvailabilityResult result_;
    const std::string message_;
  };

  Feature();
  virtual ~Feature();

  
  static Availability CreateAvailability(AvailabilityResult result,
                                         const std::string& message);

  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }
  const std::set<std::string>& dependencies() { return dependencies_; }
  bool no_parent() const { return no_parent_; }

  
  static Platform GetCurrentPlatform();

  
  static Location ConvertLocation(Manifest::Location extension_location);

  virtual std::set<Context>* GetContexts() = 0;

  
  virtual bool IsInternal() const = 0;

  
  
  Availability IsAvailableToManifest(const std::string& extension_id,
                                     Manifest::Type type,
                                     Location location,
                                     int manifest_version) const {
    return IsAvailableToManifest(extension_id, type, location, manifest_version,
                                 GetCurrentPlatform());
  }
  virtual Availability IsAvailableToManifest(const std::string& extension_id,
                                             Manifest::Type type,
                                             Location location,
                                             int manifest_version,
                                             Platform platform) const = 0;

  
  
  Availability IsAvailableToContext(const Extension* extension,
                                    Context context,
                                    const GURL& url) const {
    return IsAvailableToContext(extension, context, url, GetCurrentPlatform());
  }
  virtual Availability IsAvailableToContext(const Extension* extension,
                                            Context context,
                                            const GURL& url,
                                            Platform platform) const = 0;

  virtual std::string GetAvailabilityMessage(AvailabilityResult result,
                                             Manifest::Type type,
                                             const GURL& url,
                                             Context context) const = 0;

  virtual bool IsIdInWhitelist(const std::string& extension_id) const = 0;

 protected:
  std::string name_;
  std::set<std::string> dependencies_;
  bool no_parent_;
};

}  

#endif  
