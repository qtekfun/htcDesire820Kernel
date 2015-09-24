// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PATTERN_H_
#define CHROME_BROWSER_CONTENT_SETTINGS_CONTENT_SETTINGS_PATTERN_H_
#pragma once

#include <ostream>
#include <string>

class GURL;

class ContentSettingsPattern {
 public:
  
  static ContentSettingsPattern FromURL(const GURL& url);

  
  static ContentSettingsPattern FromURLNoWildcard(const GURL& url);

  ContentSettingsPattern() {}

  explicit ContentSettingsPattern(const std::string& pattern)
      : pattern_(pattern),
        scheme_("") {}

  
  
  
  
  
  
  bool IsValid() const;

  
  bool Matches(const GURL& url) const;

  
  const std::string& AsString() const { return pattern_; }

  bool operator==(const ContentSettingsPattern& other) const {
    return pattern_ == other.pattern_;
  }

  
  
  std::string CanonicalizePattern() const;

  std::string scheme() const {
    return scheme_;
  }

  
  static const int kContentSettingsPatternVersion;

  
  static const char* kDomainWildcard;

  
  static const size_t kDomainWildcardLength;

 private:
  
  
  
  ContentSettingsPattern(const std::string& host, const std::string& scheme)
      : pattern_(host),
        scheme_(scheme) {}

  std::string pattern_;

  
  
  
  std::string scheme_;
};

inline std::ostream& operator<<(
    std::ostream& out, const ContentSettingsPattern& pattern) {
  return out << pattern.AsString();
}

#endif  
