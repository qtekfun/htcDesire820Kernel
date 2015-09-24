// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_METADATA_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_METADATA_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/version.h"
#include "url/gurl.h"

namespace content {
struct WebPluginInfo;
}

class PluginMetadata {
 public:
  
  enum SecurityStatus {
    SECURITY_STATUS_UP_TO_DATE,
    SECURITY_STATUS_OUT_OF_DATE,
    SECURITY_STATUS_REQUIRES_AUTHORIZATION,
  };

  
  
  static const char kAdobeReaderGroupName[];
  static const char kJavaGroupName[];
  static const char kQuickTimeGroupName[];
  static const char kShockwaveGroupName[];
  static const char kRealPlayerGroupName[];
  static const char kSilverlightGroupName[];
  static const char kWindowsMediaPlayerGroupName[];

  PluginMetadata(const std::string& identifier,
                 const base::string16& name,
                 bool url_for_display,
                 const GURL& plugin_url,
                 const GURL& help_url,
                 const base::string16& group_name_matcher,
                 const std::string& language);
  ~PluginMetadata();

  
  const std::string& identifier() const { return identifier_; }

  
  const base::string16& name() const { return name_; }

  
  
  
  
  bool url_for_display() const { return url_for_display_; }
  const GURL& plugin_url() const { return plugin_url_; }

  
  const GURL& help_url() const { return help_url_; }

  const std::string& language() const { return language_; }

  bool HasMimeType(const std::string& mime_type) const;
  void AddMimeType(const std::string& mime_type);
  void AddMatchingMimeType(const std::string& mime_type);

  
  void AddVersion(const Version& version, SecurityStatus status);

  
  
  
  bool MatchesPlugin(const content::WebPluginInfo& plugin);

  
  
  static bool ParseSecurityStatus(const std::string& status_str,
                                  SecurityStatus* status);

  
  
  SecurityStatus GetSecurityStatus(const content::WebPluginInfo& plugin) const;

  scoped_ptr<PluginMetadata> Clone() const;

 private:
  struct VersionComparator {
    bool operator() (const Version& lhs, const Version& rhs) const;
  };

  std::string identifier_;
  base::string16 name_;
  base::string16 group_name_matcher_;
  bool url_for_display_;
  GURL plugin_url_;
  GURL help_url_;
  std::string language_;
  std::map<Version, SecurityStatus, VersionComparator> versions_;
  std::vector<std::string> all_mime_types_;
  std::vector<std::string> matching_mime_types_;

  DISALLOW_COPY_AND_ASSIGN(PluginMetadata);
};

#endif  
