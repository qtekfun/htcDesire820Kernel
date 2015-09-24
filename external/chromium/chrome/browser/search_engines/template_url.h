// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_H_
#pragma once

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/time.h"
#include "chrome/browser/search_engines/search_engine_type.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "googleurl/src/gurl.h"

class PrefService;
class SearchTermsData;
class TemplateURL;
class WebDataService;
struct WDKeywordsResult;

class TemplateURLRef {
 public:
  
  
  
  
  enum AcceptedSuggestion {
    NO_SUGGESTION_CHOSEN = -1,
    NO_SUGGESTIONS_AVAILABLE = -2,
  };

  TemplateURLRef();

  TemplateURLRef(const std::string& url, int index_offset, int page_offset);

  ~TemplateURLRef();

  
  bool SupportsReplacement() const;

  
  bool SupportsReplacementUsingTermsData(
      const SearchTermsData& search_terms_data) const;

  
  
  
  
  
  
  
  std::string ReplaceSearchTerms(
      const TemplateURL& host,
      const string16& terms,
      int accepted_suggestion,
      const string16& original_query_for_suggestion) const;

  
  
  
  std::string ReplaceSearchTermsUsingTermsData(
      const TemplateURL& host,
      const string16& terms,
      int accepted_suggestion,
      const string16& original_query_for_suggestion,
      const SearchTermsData& search_terms_data) const;

  
  const std::string& url() const { return url_; }

  
  int index_offset() const { return index_offset_; }

  
  int page_offset() const { return page_offset_; }

  
  
  bool IsValid() const;

  
  bool IsValidUsingTermsData(const SearchTermsData& search_terms_data) const;

  
  
  string16 DisplayURL() const;

  
  
  static std::string DisplayURLToURLRef(const string16& display_url);

  
  
  const std::string& GetHost() const;
  const std::string& GetPath() const;

  
  
  const std::string& GetSearchTermKey() const;

  
  
  string16 SearchTermToString16(const TemplateURL& host,
                                const std::string& term) const;

  
  
  bool HasGoogleBaseURLs() const;

  
  static bool SameUrlRefs(const TemplateURLRef* ref1,
                          const TemplateURLRef* ref2);

  
  void CollectRLZMetrics() const;

 private:
  friend class SearchHostToURLsMapTest;
  friend class TemplateURL;
  friend class TemplateURLModelTestUtil;
  friend class TemplateURLTest;
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseParameterKnown);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseParameterUnknown);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLEmpty);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNoTemplateEnd);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNoKnownParameters);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLTwoParameters);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNestedParameter);

  
  enum ReplacementType {
    ENCODING,
    GOOGLE_ACCEPTED_SUGGESTION,
    GOOGLE_BASE_URL,
    GOOGLE_BASE_SUGGEST_URL,
    GOOGLE_ORIGINAL_QUERY_FOR_SUGGESTION,
    GOOGLE_RLZ,
    GOOGLE_UNESCAPED_SEARCH_TERMS,
    LANGUAGE,
    SEARCH_TERMS,
  };

  
  struct Replacement {
    Replacement(ReplacementType type, size_t index)
        : type(type), index(index) {}
    ReplacementType type;
    size_t index;
  };

  
  typedef std::vector<struct Replacement> Replacements;

  
  
  void InvalidateCachedValues() const;

  
  void Set(const std::string& url, int index_offset, int page_offset);

  
  
  
  
  
  
  
  
  
  bool ParseParameter(size_t start,
                      size_t end,
                      std::string* url,
                      Replacements* replacements) const;

  
  
  
  
  
  std::string ParseURL(const std::string& url,
                       Replacements* replacements,
                       bool* valid) const;

  
  
  
  void ParseIfNecessary() const;

  
  void ParseIfNecessaryUsingTermsData(
      const SearchTermsData& search_terms_data) const;

  
  void ParseHostAndSearchTermKey(
      const SearchTermsData& search_terms_data) const;

  
  
  static void SetGoogleBaseURL(std::string* google_base_url);

  
  
  std::string url_;

  
  int index_offset_;

  
  int page_offset_;

  
  mutable bool parsed_;

  
  mutable bool valid_;

  
  
  mutable std::string parsed_url_;

  
  mutable bool supports_replacements_;

  
  
  mutable Replacements replacements_;

  
  
  mutable std::string host_;
  mutable std::string path_;
  mutable std::string search_term_key_;
};

class TemplateURL {
 public:
  
  
  
  
  
  struct ImageRef {
    ImageRef(const std::string& type, int width, int height)
        : type(type), width(width), height(height) {
    }

    ImageRef(const std::string& type, int width, int height, const GURL& url)
      : type(type), width(width), height(height), url(url) {
    }

    
    
    std::string type;

    
    int width;
    int height;

    
    GURL url;
  };

  
  static GURL GenerateFaviconURL(const GURL& url);

  
  
  static bool SupportsReplacement(const TemplateURL* turl);

  
  static bool SupportsReplacementUsingTermsData(
      const TemplateURL* turl,
      const SearchTermsData& search_terms_data);

  TemplateURL();
  ~TemplateURL();

  
  
  
  void set_short_name(const string16& short_name) {
    short_name_ = short_name;
  }
  string16 short_name() const { return short_name_; }

  
  
  string16 AdjustedShortNameForLocaleDirection() const;

  
  void set_description(const string16& description) {
    description_ = description;
  }
  string16 description() const { return description_; }

  
  
  
  
  void SetSuggestionsURL(const std::string& suggestions_url,
                         int index_offset,
                         int page_offset);
  const TemplateURLRef* suggestions_url() const {
    return suggestions_url_.url().empty() ? NULL : &suggestions_url_;
  }

  
  
  
  void SetURL(const std::string& url, int index_offset, int page_offset);
  
  
  const TemplateURLRef* url() const {
    return url_.url().empty() ? NULL : &url_;
  }

  
  
  
  void SetInstantURL(const std::string& url, int index_offset, int page_offset);
  
  
  const TemplateURLRef* instant_url() const {
    return instant_url_.url().empty() ? NULL : &instant_url_;
  }

  
  void set_originating_url(const GURL& url) {
    originating_url_ = url;
  }
  const GURL& originating_url() const { return originating_url_; }

  
  void set_keyword(const string16& keyword);
  string16 keyword() const;

  
  
  
  
  void set_autogenerate_keyword(bool autogenerate_keyword) {
    autogenerate_keyword_ = autogenerate_keyword;
    if (autogenerate_keyword_) {
      keyword_.clear();
      keyword_generated_ = false;
    }
  }
  bool autogenerate_keyword() const {
    return autogenerate_keyword_;
  }

  
  
  
  void EnsureKeyword() const;

  
  
  
  
  
  void set_show_in_default_list(bool show_in_default_list) {
    show_in_default_list_ = show_in_default_list;
  }
  bool show_in_default_list() const { return show_in_default_list_; }

  
  
  bool ShowInDefaultList() const;

  
  
  
  
  
  void set_safe_for_autoreplace(bool safe_for_autoreplace) {
    safe_for_autoreplace_ = safe_for_autoreplace;
  }
  bool safe_for_autoreplace() const { return safe_for_autoreplace_; }

  
  void add_image_ref(const ImageRef& ref) { image_refs_.push_back(ref); }
  const std::vector<ImageRef>& image_refs() const { return image_refs_; }

  
  
  
  
  
  void SetFaviconURL(const GURL& url);
  GURL GetFaviconURL() const;

  
  void add_language(const string16& language) {
    languages_.push_back(language);
  }
  std::vector<string16> languages() const { return languages_; }

  
  
  
  
  void set_date_created(base::Time time) { date_created_ = time; }
  base::Time date_created() const { return date_created_; }

  
  
  void set_created_by_policy(bool created_by_policy) {
     created_by_policy_ = created_by_policy;
  }
  bool created_by_policy() const { return created_by_policy_; }

  
  
  
  
  
  void set_usage_count(int count) { usage_count_ = count; }
  int usage_count() const { return usage_count_; }

  
  
  void set_input_encodings(const std::vector<std::string>& encodings) {
    input_encodings_ = encodings;
  }
  void add_input_encoding(const std::string& encoding) {
    input_encodings_.push_back(encoding);
  }
  const std::vector<std::string>& input_encodings() const {
    return input_encodings_;
  }

  void set_search_engine_type(SearchEngineType search_engine_type) {
    search_engine_type_ = search_engine_type;
  }
  SearchEngineType search_engine_type() const {
    return search_engine_type_;
  }

  void set_logo_id(int logo_id) { logo_id_ = logo_id; }
  int logo_id() const { return logo_id_; }

  
  
  TemplateURLID id() const { return id_; }

  
  void set_prepopulate_id(int id) { prepopulate_id_ = id; }
  int prepopulate_id() const { return prepopulate_id_; }

  std::string GetExtensionId() const;
  bool IsExtensionKeyword() const;

 private:
  friend void MergeEnginesFromPrepopulateData(
      PrefService* prefs,
      WebDataService* service,
      std::vector<TemplateURL*>* template_urls,
      const TemplateURL** default_search_provider);
  friend class KeywordTable;
  friend class KeywordTableTest;
  friend class SearchHostToURLsMap;
  friend class TemplateURLModel;

  
  void InvalidateCachedValues() const;

  
  void set_id(TemplateURLID id) { id_ = id; }

  string16 short_name_;
  string16 description_;
  TemplateURLRef suggestions_url_;
  TemplateURLRef url_;
  TemplateURLRef instant_url_;
  GURL originating_url_;
  mutable string16 keyword_;
  bool autogenerate_keyword_;  
                               
  mutable bool keyword_generated_;  
                                    
                                    
  bool show_in_default_list_;
  bool safe_for_autoreplace_;
  std::vector<ImageRef> image_refs_;
  std::vector<string16> languages_;
  
  std::vector<std::string> input_encodings_;
  TemplateURLID id_;
  base::Time date_created_;
  bool created_by_policy_;
  int usage_count_;
  SearchEngineType search_engine_type_;
  int logo_id_;
  int prepopulate_id_;

  
};

#endif  
