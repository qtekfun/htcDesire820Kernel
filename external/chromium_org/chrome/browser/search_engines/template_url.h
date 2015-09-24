// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_H_

#include <string>
#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"
#include "url/url_parse.h"

class Profile;
class SearchTermsData;
class TemplateURL;



class TemplateURLRef {
 public:
  
  
  
  
  enum AcceptedSuggestion {
    NO_SUGGESTION_CHOSEN = -1,
    NO_SUGGESTIONS_AVAILABLE = -2,
  };

  
  
  
  enum Type {
    SEARCH,
    SUGGEST,
    INSTANT,
    IMAGE,
    NEW_TAB,
    INDEXED
  };

  
  
  
  
  
  typedef std::pair<std::string, std::string> PostContent;

  
  
  
  struct SearchTermsArgs {
    explicit SearchTermsArgs(const base::string16& search_terms);
    ~SearchTermsArgs();

    
    base::string16 search_terms;

    
    base::string16 original_query;

    
    
    
    
    
    
    std::string assisted_query_stats;

    
    int accepted_suggestion;

    
    
    size_t cursor_position;

    
    
    int omnibox_start_margin;

    
    
    std::string current_page_url;

    
    AutocompleteInput::PageClassification page_classification;

    
    bool bookmark_bar_pinned;

    
    std::string suggest_query_params;

    
    
    
    
    
    
    
    bool append_extra_query_params;

    
    
    std::string image_thumbnail_content;

    
    
    GURL image_url;

    
    gfx::Size image_original_size;

    
    
    
    
    bool force_instant_results;
  };

  TemplateURLRef(TemplateURL* owner, Type type);
  TemplateURLRef(TemplateURL* owner, size_t index_in_owner);
  ~TemplateURLRef();

  
  std::string GetURL() const;

  
  
  std::string GetPostParamsString() const;

  
  bool SupportsReplacement() const;

  
  bool SupportsReplacementUsingTermsData(
      const SearchTermsData& search_terms_data) const;

  
  
  
  
  
  
  
  
  std::string ReplaceSearchTerms(
      const SearchTermsArgs& search_terms_args,
      PostContent* post_content) const;
  
  
  
  std::string ReplaceSearchTerms(
      const SearchTermsArgs& search_terms_args) const {
    return ReplaceSearchTerms(search_terms_args, NULL);
  }

  
  
  
  std::string ReplaceSearchTermsUsingTermsData(
      const SearchTermsArgs& search_terms_args,
      const SearchTermsData& search_terms_data,
      PostContent* post_content) const;

  
  
  bool IsValid() const;

  
  bool IsValidUsingTermsData(const SearchTermsData& search_terms_data) const;

  
  
  base::string16 DisplayURL() const;

  
  
  static std::string DisplayURLToURLRef(const base::string16& display_url);

  
  
  const std::string& GetHost() const;
  const std::string& GetPath() const;

  
  
  const std::string& GetSearchTermKey() const;

  
  base::string16 SearchTermToString16(const std::string& term) const;

  
  
  bool HasGoogleBaseURLs() const;

  
  
  
  
  
  
  
  
  bool ExtractSearchTermsFromURL(
      const GURL& url,
      base::string16* search_terms,
      const SearchTermsData& search_terms_data,
      url_parse::Parsed::ComponentType* search_term_component,
      url_parse::Component* search_terms_position) const;

  
  bool UsesPOSTMethodUsingTermsData(
      const SearchTermsData* search_terms_data) const;
  bool UsesPOSTMethod() const {
    return UsesPOSTMethodUsingTermsData(NULL);
  }

 private:
  friend class TemplateURL;
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, SetPrepopulatedAndParse);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseParameterKnown);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseParameterUnknown);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLEmpty);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNoTemplateEnd);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNoKnownParameters);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLTwoParameters);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ParseURLNestedParameter);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, URLRefTestImageURLWithPOST);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ReflectsBookmarkBarPinned);

  
  enum ReplacementType {
    ENCODING,
    GOOGLE_ASSISTED_QUERY_STATS,
    GOOGLE_BASE_URL,
    GOOGLE_BASE_SUGGEST_URL,
    GOOGLE_BOOKMARK_BAR_PINNED,
    GOOGLE_CURRENT_PAGE_URL,
    GOOGLE_CURSOR_POSITION,
    GOOGLE_IMAGE_ORIGINAL_HEIGHT,
    GOOGLE_IMAGE_ORIGINAL_WIDTH,
    GOOGLE_IMAGE_SEARCH_SOURCE,
    GOOGLE_IMAGE_THUMBNAIL,
    GOOGLE_IMAGE_URL,
    GOOGLE_FORCE_INSTANT_RESULTS,
    GOOGLE_INSTANT_EXTENDED_ENABLED,
    GOOGLE_NTP_IS_THEMED,
    GOOGLE_OMNIBOX_START_MARGIN,
    GOOGLE_ORIGINAL_QUERY_FOR_SUGGESTION,
    GOOGLE_PAGE_CLASSIFICATION,
    GOOGLE_RLZ,
    GOOGLE_SEARCH_CLIENT,
    GOOGLE_SEARCH_FIELDTRIAL_GROUP,
    GOOGLE_SUGGEST_CLIENT,
    GOOGLE_SUGGEST_REQUEST_ID,
    GOOGLE_UNESCAPED_SEARCH_TERMS,
    LANGUAGE,
    SEARCH_TERMS,
  };

  
  struct Replacement {
    Replacement(ReplacementType type, size_t index)
        : type(type), index(index), is_post_param(false) {}
    ReplacementType type;
    size_t index;
    
    
    
    bool is_post_param;
  };

  
  typedef std::vector<struct Replacement> Replacements;
  
  typedef std::pair<std::string, std::string> PostParam;
  typedef std::vector<PostParam> PostParams;

  
  
  void InvalidateCachedValues() const;

  
  
  
  
  
  
  
  
  
  bool ParseParameter(size_t start,
                      size_t end,
                      std::string* url,
                      Replacements* replacements) const;

  
  
  
  
  
  
  
  
  std::string ParseURL(const std::string& url,
                       Replacements* replacements,
                       PostParams* post_params,
                       bool* valid) const;

  
  
  
  void ParseIfNecessary() const;

  
  void ParseIfNecessaryUsingTermsData(
      const SearchTermsData& search_terms_data) const;

  
  void ParseHostAndSearchTermKey(
      const SearchTermsData& search_terms_data) const;

  
  
  
  bool EncodeFormData(const PostParams& post_params,
                      PostContent* post_content) const;

  
  
  
  
  void HandleReplacement(const std::string& name,
                         const std::string& value,
                         const Replacement& replacement,
                         std::string* url) const;

  
  
  
  std::string HandleReplacements(
      const SearchTermsArgs& search_terms_args,
      const SearchTermsData& search_terms_data,
      PostContent* post_content) const;

  
  TemplateURL* const owner_;

  
  const Type type_;

  
  
  const size_t index_in_owner_;

  
  mutable bool parsed_;

  
  mutable bool valid_;

  
  
  mutable std::string parsed_url_;

  
  mutable bool supports_replacements_;

  
  
  mutable Replacements replacements_;

  
  
  mutable std::string host_;
  mutable std::string path_;
  mutable std::string search_term_key_;
  mutable url_parse::Parsed::ComponentType search_term_key_location_;

  mutable PostParams post_params_;

  
  bool prepopulated_;

  
  
  bool showing_search_terms_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLRef);
};



struct TemplateURLData {
  TemplateURLData();
  ~TemplateURLData();

  
  
  
  base::string16 short_name;

  
  void SetKeyword(const base::string16& keyword);
  const base::string16& keyword() const { return keyword_; }

  
  
  void SetURL(const std::string& url);
  const std::string& url() const { return url_; }

  
  std::string suggestions_url;
  std::string instant_url;
  std::string image_url;
  std::string new_tab_url;

  
  
  std::string search_url_post_params;
  std::string suggestions_url_post_params;
  std::string instant_url_post_params;
  std::string image_url_post_params;

  
  GURL favicon_url;

  
  GURL originating_url;

  
  
  
  
  bool show_in_default_list;

  
  
  
  
  
  bool safe_for_autoreplace;

  
  
  std::vector<std::string> input_encodings;

  
  
  TemplateURLID id;

  
  
  
  
  base::Time date_created;

  
  
  
  base::Time last_modified;

  
  
  bool created_by_policy;

  
  
  
  
  
  int usage_count;

  
  int prepopulate_id;

  
  
  std::string sync_guid;

  
  
  std::vector<std::string> alternate_urls;

  
  
  std::string search_terms_replacement_key;

 private:
  
  
  base::string16 keyword_;
  std::string url_;
};



struct AssociatedExtensionInfo {
  std::string extension_id;

  
  bool wants_to_be_default_engine;

  
  
  base::Time install_time;
};



class TemplateURL {
 public:
  enum Type {
    
    NORMAL,
    
    NORMAL_CONTROLLED_BY_EXTENSION,
    
    OMNIBOX_API_EXTENSION,
  };
  
  
  TemplateURL(Profile* profile, const TemplateURLData& data);
  ~TemplateURL();

  
  static GURL GenerateFaviconURL(const GURL& url);

  Profile* profile() { return profile_; }
  const TemplateURLData& data() const { return data_; }

  const base::string16& short_name() const { return data_.short_name; }
  
  
  base::string16 AdjustedShortNameForLocaleDirection() const;

  const base::string16& keyword() const { return data_.keyword(); }

  const std::string& url() const { return data_.url(); }
  const std::string& suggestions_url() const { return data_.suggestions_url; }
  const std::string& instant_url() const { return data_.instant_url; }
  const std::string& image_url() const { return data_.image_url; }
  const std::string& new_tab_url() const { return data_.new_tab_url; }
  const std::string& search_url_post_params() const {
    return data_.search_url_post_params;
  }
  const std::string& suggestions_url_post_params() const {
    return data_.suggestions_url_post_params;
  }
  const std::string& instant_url_post_params() const {
    return data_.instant_url_post_params;
  }
  const std::string& image_url_post_params() const {
    return data_.image_url_post_params;
  }
  const std::vector<std::string>& alternate_urls() const {
    return data_.alternate_urls;
  }
  const GURL& favicon_url() const { return data_.favicon_url; }

  const GURL& originating_url() const { return data_.originating_url; }

  bool show_in_default_list() const { return data_.show_in_default_list; }
  
  
  bool ShowInDefaultList() const;

  bool safe_for_autoreplace() const { return data_.safe_for_autoreplace; }

  const std::vector<std::string>& input_encodings() const {
    return data_.input_encodings;
  }

  TemplateURLID id() const { return data_.id; }

  base::Time date_created() const { return data_.date_created; }
  base::Time last_modified() const { return data_.last_modified; }

  bool created_by_policy() const { return data_.created_by_policy; }

  int usage_count() const { return data_.usage_count; }

  int prepopulate_id() const { return data_.prepopulate_id; }

  const std::string& sync_guid() const { return data_.sync_guid; }

  
  const std::string& search_terms_replacement_key() const {
    return data_.search_terms_replacement_key;
  }

  const TemplateURLRef& url_ref() const { return url_ref_; }
  const TemplateURLRef& suggestions_url_ref() const {
    return suggestions_url_ref_;
  }
  const TemplateURLRef& instant_url_ref() const { return instant_url_ref_; }
  const TemplateURLRef& image_url_ref() const { return image_url_ref_; }
  const TemplateURLRef& new_tab_url_ref() const { return new_tab_url_ref_; }

  
  bool SupportsReplacement() const;

  
  bool SupportsReplacementUsingTermsData(
      const SearchTermsData& search_terms_data) const;

  
  
  
  bool IsGoogleSearchURLWithReplaceableKeyword() const;

  
  
  bool HasSameKeywordAs(const TemplateURL& other) const;

  Type GetType() const;

  
  
  
  std::string GetExtensionId() const;

  
  
  size_t URLCount() const;

  
  
  
  
  const std::string& GetURL(size_t index) const;

  
  
  
  
  
  
  
  
  
  
  bool ExtractSearchTermsFromURL(const GURL& url, base::string16* search_terms);

  
  
  bool ExtractSearchTermsFromURLUsingTermsData(
      const GURL& url,
      base::string16* search_terms,
      const SearchTermsData& search_terms_data);

  
  
  
  bool IsSearchURL(const GURL& url);

  
  bool IsSearchURLUsingTermsData(
      const GURL& url,
      const SearchTermsData& search_terms_data);

  
  
  
  
  
  
  bool HasSearchTermsReplacementKey(const GURL& url) const;

  
  
  
  
  
  bool ReplaceSearchTermsInURL(
      const GURL& url,
      const TemplateURLRef::SearchTermsArgs& search_terms_args,
      GURL* result);

  
  
  
  
  
  void EncodeSearchTerms(
      const TemplateURLRef::SearchTermsArgs& search_terms_args,
      bool is_in_query,
      std::string* input_encoding,
      base::string16* encoded_terms,
      base::string16* encoded_original_query) const;

 private:
  friend class TemplateURLService;
  FRIEND_TEST_ALL_PREFIXES(TemplateURLTest, ReflectsBookmarkBarPinned);

  void CopyFrom(const TemplateURL& other);

  void SetURL(const std::string& url);
  void SetPrepopulateId(int id);

  
  
  
  
  
  void ResetKeywordIfNecessary(bool force);

  
  
  
  
  bool FindSearchTermsInURL(
      const GURL& url,
      const SearchTermsData& search_terms_data,
      base::string16* search_terms,
      url_parse::Parsed::ComponentType* search_terms_component,
      url_parse::Component* search_terms_position);

  Profile* profile_;
  TemplateURLData data_;
  TemplateURLRef url_ref_;
  TemplateURLRef suggestions_url_ref_;
  TemplateURLRef instant_url_ref_;
  TemplateURLRef image_url_ref_;
  TemplateURLRef new_tab_url_ref_;
  scoped_ptr<AssociatedExtensionInfo> extension_info_;

  

  DISALLOW_COPY_AND_ASSIGN(TemplateURL);
};

#endif  
