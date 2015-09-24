// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "base/timer.h"
#include "googleurl/src/gurl.h"
#include "googleurl/src/url_parse.h"


class AutocompleteController;
class AutocompleteControllerDelegate;
class AutocompleteInput;
struct AutocompleteMatch;
class AutocompleteProvider;
class AutocompleteResult;
class HistoryContentsProvider;
class Profile;
class SearchProvider;
class TemplateURL;

typedef std::vector<AutocompleteMatch> ACMatches;
typedef std::vector<AutocompleteProvider*> ACProviders;


class AutocompleteInput {
 public:
  
  
  
  
  enum Type {
    INVALID,        
    UNKNOWN,        
    REQUESTED_URL,  
                    
    URL,            
    QUERY,          
    FORCED_QUERY,   
  };

  
  
  
  enum MatchesRequested {
    
    
    
    
    BEST_MATCH,

    
    SYNCHRONOUS_MATCHES,

    
    ALL_MATCHES,
  };

  AutocompleteInput();
  AutocompleteInput(const string16& text,
                    const string16& desired_tld,
                    bool prevent_inline_autocomplete,
                    bool prefer_keyword,
                    bool allow_exact_keyword_match,
                    MatchesRequested matches_requested);
  ~AutocompleteInput();

  
  static void RemoveForcedQueryStringIfNecessary(Type type, string16* text);

  
  static std::string TypeToString(Type type);

  
  
  
  
  
  static Type Parse(const string16& text,
                    const string16& desired_tld,
                    url_parse::Parsed* parts,
                    string16* scheme,
                    GURL* canonicalized_url);

  
  
  
  
  static void ParseForEmphasizeComponents(const string16& text,
                                          const string16& desired_tld,
                                          url_parse::Component* scheme,
                                          url_parse::Component* host);

  
  
  
  
  
  
  static string16 FormattedStringWithEquivalentMeaning(
      const GURL& url,
      const string16& formatted_url);

  
  const string16& text() const { return text_; }

  
  
  
  void set_text(const string16& text) { text_ = text; }

  
  
  const string16& original_text() const { return original_text_; }

  
  
  
  
  const string16& desired_tld() const { return desired_tld_; }

  
  Type type() const { return type_; }

  
  const url_parse::Parsed& parts() const { return parts_; }

  
  
  const string16& scheme() const { return scheme_; }

  
  const GURL& canonicalized_url() const { return canonicalized_url_; }

  
  bool prevent_inline_autocomplete() const {
    return prevent_inline_autocomplete_;
  }

  
  
  
  bool initial_prevent_inline_autocomplete() const {
    return initial_prevent_inline_autocomplete_;
  }

  
  
  bool prefer_keyword() const { return prefer_keyword_; }

  
  
  
  bool allow_exact_keyword_match() const { return allow_exact_keyword_match_; }

  
  MatchesRequested matches_requested() const { return matches_requested_; }

  
  bool Equals(const AutocompleteInput& other) const;

  
  void Clear();

 private:
  string16 text_;
  string16 original_text_;
  string16 desired_tld_;
  Type type_;
  url_parse::Parsed parts_;
  string16 scheme_;
  GURL canonicalized_url_;
  bool initial_prevent_inline_autocomplete_;
  bool prevent_inline_autocomplete_;
  bool prefer_keyword_;
  bool allow_exact_keyword_match_;
  MatchesRequested matches_requested_;
};


class AutocompleteProvider
    : public base::RefCountedThreadSafe<AutocompleteProvider> {
 public:
  class ACProviderListener {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnProviderUpdate(bool updated_matches) = 0;

   protected:
    virtual ~ACProviderListener();
  };

  AutocompleteProvider(ACProviderListener* listener,
                       Profile* profile,
                       const char* name);

  
  
  
  void SetProfile(Profile* profile);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) = 0;

  
  
  
  virtual void Stop();

  
  const ACMatches& matches() const { return matches_; }

  
  bool done() const { return done_; }

  
  const char* name() const { return name_; }

  
  
  
  
  
  virtual void DeleteMatch(const AutocompleteMatch& match);

  
  
  
  
  static const size_t kMaxMatches;

 protected:
  friend class base::RefCountedThreadSafe<AutocompleteProvider>;

  virtual ~AutocompleteProvider();

  
  static bool HasHTTPScheme(const string16& input);

  
  
  void UpdateStarredStateOfMatches();

  
  
  
  string16 StringForURLDisplay(const GURL& url,
                               bool check_accept_lang,
                               bool trim_http) const;

  
  
  Profile* profile_;

  ACProviderListener* listener_;
  ACMatches matches_;
  bool done_;

  
  const char* name_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutocompleteProvider);
};

typedef AutocompleteProvider::ACProviderListener ACProviderListener;


class AutocompleteResult {
 public:
  typedef ACMatches::const_iterator const_iterator;
  typedef ACMatches::iterator iterator;

  
  
  struct Selection {
    Selection()
        : provider_affinity(NULL),
          is_history_what_you_typed_match(false) {
    }

    
    void Clear();

    
    bool empty() const {
      return destination_url.is_empty() && !provider_affinity &&
          !is_history_what_you_typed_match;
    }

    
    GURL destination_url;

    
    
    const AutocompleteProvider* provider_affinity;

    
    
    
    
    bool is_history_what_you_typed_match;
  };

  AutocompleteResult();
  ~AutocompleteResult();

  
  void CopyFrom(const AutocompleteResult& rhs);

  
  
  void CopyOldMatches(const AutocompleteInput& input,
                      const AutocompleteResult& old_matches);

  
  
  
  void AddMatch(const AutocompleteMatch& match);

  
  void AppendMatches(const ACMatches& matches);

  
  
  
  void SortAndCull(const AutocompleteInput& input);

  
  bool HasCopiedMatches() const;

  
  size_t size() const;
  bool empty() const;
  const_iterator begin() const;
  iterator begin();
  const_iterator end() const;
  iterator end();

  
  const AutocompleteMatch& match_at(size_t index) const;

  
  
  const_iterator default_match() const { return default_match_; }

  GURL alternate_nav_url() const { return alternate_nav_url_; }

  
  void Reset();

  void Swap(AutocompleteResult* other);

#ifndef NDEBUG
  
  void Validate() const;
#endif

  
  static const size_t kMaxMatches;

 private:
  typedef std::map<AutocompleteProvider*, ACMatches> ProviderToMatches;

  
  void BuildProviderToMatches(ProviderToMatches* provider_to_matches) const;

  
  
  static bool HasMatchByDestination(const AutocompleteMatch& match,
                                    const ACMatches& matches);

  
  
  void MergeMatchesByProvider(const ACMatches& old_matches,
                              const ACMatches& new_matches);

  ACMatches matches_;

  const_iterator default_match_;

  
  
  
  
  
  
  
  GURL alternate_nav_url_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteResult);
};


class AutocompleteController : public ACProviderListener {
 public:
  
  static const int kNoItemSelected;

  
  
  
  
  AutocompleteController(Profile* profile,
                         AutocompleteControllerDelegate* delegate);
#ifdef UNIT_TEST
  explicit AutocompleteController(const ACProviders& providers)
      : delegate_(NULL),
        providers_(providers),
        search_provider_(NULL),
        done_(true),
        in_start_(false) {
  }
#endif
  ~AutocompleteController();

  
  
  void SetProfile(Profile* profile);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  void Start(const string16& text,
             const string16& desired_tld,
             bool prevent_inline_autocomplete,
             bool prefer_keyword,
             bool allow_exact_keyword_match,
             AutocompleteInput::MatchesRequested matches_requested);

  
  
  
  
  
  void Stop(bool clear_result);

  
  
  
  void DeleteMatch(const AutocompleteMatch& match);

  
  
  void ExpireCopiedEntries();

  SearchProvider* search_provider() const { return search_provider_; }

  
  const AutocompleteInput& input() const { return input_; }
  const AutocompleteResult& result() const { return result_; }
  bool done() const { return done_; }

  
  virtual void OnProviderUpdate(bool updated_matches);

 private:
  
  
  
  void UpdateResult(bool is_synchronous_pass);

  
  
  void NotifyChanged(bool notify_default_match);

  
  void CheckIfDone();

  
  void StartExpireTimer();

  AutocompleteControllerDelegate* delegate_;

  
  ACProviders providers_;

  SearchProvider* search_provider_;

  
  AutocompleteInput input_;

  
  AutocompleteResult result_;

  
  
  base::OneShotTimer<AutocompleteController> expire_timer_;

  
  bool done_;

  
  
  bool in_start_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteController);
};


struct AutocompleteLog {
  AutocompleteLog(const string16& text,
                  AutocompleteInput::Type input_type,
                  size_t selected_index,
                  size_t inline_autocompleted_length,
                  const AutocompleteResult& result)
      : text(text),
        input_type(input_type),
        selected_index(selected_index),
        inline_autocompleted_length(inline_autocompleted_length),
        result(result) {
  }
  
  string16 text;
  
  AutocompleteInput::Type input_type;
  
  size_t selected_index;
  
  size_t inline_autocompleted_length;
  
  const AutocompleteResult& result;
};

#endif  
