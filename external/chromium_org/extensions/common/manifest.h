// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_MANIFEST_H_
#define EXTENSIONS_COMMON_MANIFEST_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/values.h"

namespace extensions {
struct InstallWarning;

class Manifest {
 public:
  
  
  
  
  enum Location {
    INVALID_LOCATION,
    INTERNAL,           
    EXTERNAL_PREF,      
    EXTERNAL_REGISTRY,  
                        
    UNPACKED,           
                        
    COMPONENT,          
                        
                        
    EXTERNAL_PREF_DOWNLOAD,    
                               
    EXTERNAL_POLICY_DOWNLOAD,  
                               
    COMMAND_LINE,       
    EXTERNAL_POLICY,    
                        
                        
    EXTERNAL_COMPONENT, 
                        
                        

    NUM_LOCATIONS
  };

  
  
  enum Type {
    TYPE_UNKNOWN = 0,
    TYPE_EXTENSION,
    TYPE_THEME,
    TYPE_USER_SCRIPT,
    TYPE_HOSTED_APP,
    
    
    TYPE_LEGACY_PACKAGED_APP,
    TYPE_PLATFORM_APP,
    TYPE_SHARED_MODULE
  };

  
  
  
  static Location GetHigherPriorityLocation(Location loc1, Location loc2);

  
  static inline bool IsExternalLocation(Location location) {
    return location == EXTERNAL_PREF ||
           location == EXTERNAL_REGISTRY ||
           location == EXTERNAL_PREF_DOWNLOAD ||
           location == EXTERNAL_POLICY ||
           location == EXTERNAL_POLICY_DOWNLOAD ||
           location == EXTERNAL_COMPONENT;
  }

  
  static inline bool IsUnpackedLocation(Location location) {
    return location == UNPACKED || location == COMMAND_LINE;
  }

  
  static inline bool IsAutoUpdateableLocation(Location location) {
    
    return location == INTERNAL ||
           IsExternalLocation(location);
  }

  
  
  static inline bool IsPolicyLocation(Location location) {
    return location == EXTERNAL_POLICY ||
           location == EXTERNAL_POLICY_DOWNLOAD ||
           location == EXTERNAL_COMPONENT;
  }

  
  
  static inline bool ShouldAlwaysAllowFileAccess(Location location) {
    return IsUnpackedLocation(location);
  }

  Manifest(Location location, scoped_ptr<base::DictionaryValue> value);
  virtual ~Manifest();

  const std::string& extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& id) { extension_id_ = id; }

  Location location() const { return location_; }

  
  
  
  bool ValidateManifest(std::string* error,
                        std::vector<InstallWarning>* warnings) const;

  
  
  
  
  int GetManifestVersion() const;

  
  Type type() const { return type_; }

  bool is_theme() const { return type_ == TYPE_THEME; }
  bool is_app() const {
    return is_legacy_packaged_app() || is_hosted_app() || is_platform_app();
  }
  bool is_platform_app() const { return type_ == TYPE_PLATFORM_APP; }
  bool is_hosted_app() const { return type_ == TYPE_HOSTED_APP; }
  bool is_legacy_packaged_app() const {
    return type_ == TYPE_LEGACY_PACKAGED_APP;
  }
  bool is_extension() const { return type_ == TYPE_EXTENSION; }
  bool is_shared_module() const { return type_ == TYPE_SHARED_MODULE; }

  
  
  bool HasKey(const std::string& key) const;
  bool HasPath(const std::string& path) const;
  bool Get(const std::string& path, const base::Value** out_value) const;
  bool GetBoolean(const std::string& path, bool* out_value) const;
  bool GetInteger(const std::string& path, int* out_value) const;
  bool GetString(const std::string& path, std::string* out_value) const;
  bool GetString(const std::string& path, string16* out_value) const;
  bool GetDictionary(const std::string& path,
                     const base::DictionaryValue** out_value) const;
  bool GetList(const std::string& path,
               const base::ListValue** out_value) const;

  
  
  Manifest* DeepCopy() const;

  
  bool Equals(const Manifest* other) const;

  
  
  const base::DictionaryValue* value() const { return value_.get(); }

 private:
  
  bool CanAccessPath(const std::string& path) const;
  bool CanAccessKey(const std::string& key) const;

  
  
  
  
  std::string extension_id_;

  
  Location location_;

  
  scoped_ptr<base::DictionaryValue> value_;

  Type type_;

  DISALLOW_COPY_AND_ASSIGN(Manifest);
};

}  

#endif  
