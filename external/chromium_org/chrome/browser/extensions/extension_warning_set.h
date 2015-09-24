// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_SET_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_SET_H_

#include <set>
#include <string>
#include <vector>

#include "url/gurl.h"


namespace base {
class FilePath;
}

class ExtensionSet;

namespace extensions {

class ExtensionWarning {
 public:
  enum WarningType {
    
    
    kInvalid = 0,
    
    kNetworkDelay,
    
    
    kNetworkConflict,
    
    
    kRedirectConflict,
    
    
    kRepeatedCacheFlushes,
    
    
    kDownloadFilenameConflict,
    kMaxWarningType
  };

  
  
  ExtensionWarning(const ExtensionWarning& other);
  ~ExtensionWarning();
  ExtensionWarning& operator=(const ExtensionWarning& other);

  
  static ExtensionWarning CreateNetworkDelayWarning(
      const std::string& extension_id);
  static ExtensionWarning CreateNetworkConflictWarning(
      const std::string& extension_id);
  static ExtensionWarning CreateRedirectConflictWarning(
      const std::string& extension_id,
      const std::string& winning_extension_id,
      const GURL& attempted_redirect_url,
      const GURL& winning_redirect_url);
  static ExtensionWarning CreateRequestHeaderConflictWarning(
      const std::string& extension_id,
      const std::string& winning_extension_id,
      const std::string& conflicting_header);
  static ExtensionWarning CreateResponseHeaderConflictWarning(
      const std::string& extension_id,
      const std::string& winning_extension_id,
      const std::string& conflicting_header);
  static ExtensionWarning CreateCredentialsConflictWarning(
      const std::string& extension_id,
      const std::string& winning_extension_id);
  static ExtensionWarning CreateRepeatedCacheFlushesWarning(
      const std::string& extension_id);
  static ExtensionWarning CreateDownloadFilenameConflictWarning(
      const std::string& losing_extension_id,
      const std::string& winning_extension_id,
      const base::FilePath& losing_filename,
      const base::FilePath& winning_filename);

  
  WarningType warning_type() const { return type_; }

  
  const std::string& extension_id() const { return extension_id_; }

  
  std::string GetLocalizedMessage(const ExtensionSet* extensions) const;

 private:
  
  
  
  
  ExtensionWarning(WarningType type,
                   const std::string& extension_id,
                   int message_id,
                   const std::vector<std::string>& message_parameters);

  WarningType type_;
  std::string extension_id_;
  
  int message_id_;
  
  std::vector<std::string> message_parameters_;
};

bool operator<(const ExtensionWarning& a, const ExtensionWarning& b);

typedef std::set<ExtensionWarning> ExtensionWarningSet;

}  

#endif  
