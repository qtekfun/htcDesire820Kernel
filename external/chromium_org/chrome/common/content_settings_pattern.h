// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_COMMON_CONTENT_SETTINGS_PATTERN_H_
#define CHROME_COMMON_CONTENT_SETTINGS_PATTERN_H_

#include <ostream>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"

class GURL;
class Pickle;
class PickleIterator;

namespace content_settings {
class PatternParser;
}

namespace IPC {
class Message;
}

class ContentSettingsPattern {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum Relation {
    DISJOINT_ORDER_POST = -2,
    SUCCESSOR = -1,
    IDENTITY = 0,
    PREDECESSOR = 1,
    DISJOINT_ORDER_PRE = 2,
  };

  struct PatternParts {
    PatternParts();
    ~PatternParts();

    
    
    std::string scheme;

    
    bool is_scheme_wildcard;

    
    
    
    
    
    std::string host;

    
    bool has_domain_wildcard;

    
    
    std::string port;

    
    bool is_port_wildcard;

    
    
    
    std::string path;

    
    bool is_path_wildcard;
  };

  class BuilderInterface {
   public:
    virtual ~BuilderInterface() {}

    virtual BuilderInterface* WithPort(const std::string& port) = 0;

    virtual BuilderInterface* WithPortWildcard() = 0;

    virtual BuilderInterface* WithHost(const std::string& host) = 0;

    virtual BuilderInterface* WithDomainWildcard() = 0;

    virtual BuilderInterface* WithScheme(const std::string& scheme) = 0;

    virtual BuilderInterface* WithSchemeWildcard() = 0;

    virtual BuilderInterface* WithPath(const std::string& path) = 0;

    virtual BuilderInterface* WithPathWildcard() = 0;

    virtual BuilderInterface* Invalid() = 0;

    
    
    virtual ContentSettingsPattern Build() = 0;
  };

  static BuilderInterface* CreateBuilder(bool use_legacy_validate);

  
  static const int kContentSettingsPatternVersion;

  
  static const char* kDomainWildcard;

  
  static const size_t kDomainWildcardLength;

  
  
  static ContentSettingsPattern Wildcard();

  
  
  static ContentSettingsPattern FromURL(const GURL& url);

  
  static ContentSettingsPattern FromURLNoWildcard(const GURL& url);

  
  
  
  
  
  
  
  
  
  static ContentSettingsPattern FromString(const std::string& pattern_spec);

  static ContentSettingsPattern LegacyFromString(
      const std::string& pattern_spec);

  
  
  ContentSettingsPattern();

  
  void WriteToMessage(IPC::Message* m) const;
  bool ReadFromMessage(const IPC::Message* m, PickleIterator* iter);

  
  bool IsValid() const { return is_valid_; }

  
  bool Matches(const GURL& url) const;

  
  bool MatchesAllHosts() const;

  
  const std::string ToString() const;

  
  
  Relation Compare(const ContentSettingsPattern& other) const;

  
  bool operator==(const ContentSettingsPattern& other) const;

  
  bool operator!=(const ContentSettingsPattern& other) const;

  
  bool operator<(const ContentSettingsPattern& other) const;

  
  bool operator>(const ContentSettingsPattern& other) const;

 private:
  friend class content_settings::PatternParser;
  friend class Builder;
  FRIEND_TEST_ALL_PREFIXES(ContentSettingsPatternParserTest, SerializePatterns);

  class Builder : public BuilderInterface {
    public:
     explicit Builder(bool use_legacy_validate);
     virtual ~Builder();

     
     virtual BuilderInterface* WithPort(const std::string& port) OVERRIDE;

     virtual BuilderInterface* WithPortWildcard() OVERRIDE;

     virtual BuilderInterface* WithHost(const std::string& host) OVERRIDE;

     virtual BuilderInterface* WithDomainWildcard() OVERRIDE;

     virtual BuilderInterface* WithScheme(const std::string& scheme) OVERRIDE;

     virtual BuilderInterface* WithSchemeWildcard() OVERRIDE;

     virtual BuilderInterface* WithPath(const std::string& path) OVERRIDE;

     virtual BuilderInterface* WithPathWildcard() OVERRIDE;

     virtual BuilderInterface* Invalid() OVERRIDE;

     virtual ContentSettingsPattern Build() OVERRIDE;

    private:
     
     
     
     static bool Canonicalize(PatternParts* parts);

     
     static bool Validate(const PatternParts& parts);

     static bool LegacyValidate(const PatternParts& parts);

     bool is_valid_;

     bool use_legacy_validate_;

     PatternParts parts_;

     DISALLOW_COPY_AND_ASSIGN(Builder);
  };

  static Relation CompareScheme(
      const ContentSettingsPattern::PatternParts& parts,
      const ContentSettingsPattern::PatternParts& other_parts);

  static Relation CompareHost(
      const ContentSettingsPattern::PatternParts& parts,
      const ContentSettingsPattern::PatternParts& other_parts);

  static Relation ComparePort(
      const ContentSettingsPattern::PatternParts& parts,
      const ContentSettingsPattern::PatternParts& other_parts);

  static bool Validate(const PatternParts& parts);

  ContentSettingsPattern(const PatternParts& parts, bool valid);

  PatternParts parts_;

  bool is_valid_;
};

inline std::ostream& operator<<(
    std::ostream& out, const ContentSettingsPattern& pattern) {
  return out << pattern.ToString();
}

#endif  
