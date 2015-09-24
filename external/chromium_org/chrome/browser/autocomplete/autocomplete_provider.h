// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_PROVIDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/common/metrics/proto/omnibox_event.pb.h"

class AutocompleteInput;
class AutocompleteProviderListener;
class GURL;
class Profile;

typedef std::vector<metrics::OmniboxEventProto_ProviderInfo> ProvidersInfo;

class AutocompleteProvider
    : public base::RefCountedThreadSafe<AutocompleteProvider> {
 public:
  
  enum Type {
    TYPE_BOOKMARK         = 1 << 0,
    TYPE_BUILTIN          = 1 << 1,
    TYPE_CONTACT          = 1 << 2,
    TYPE_EXTENSION_APP    = 1 << 3,
    TYPE_HISTORY_QUICK    = 1 << 4,
    TYPE_HISTORY_URL      = 1 << 5,
    TYPE_KEYWORD          = 1 << 6,
    TYPE_SEARCH           = 1 << 7,
    TYPE_SHORTCUTS        = 1 << 8,
    TYPE_ZERO_SUGGEST     = 1 << 9,
  };

  AutocompleteProvider(AutocompleteProviderListener* listener,
                       Profile* profile,
                       Type type);

  
  static const char* TypeToString(Type type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Start(const AutocompleteInput& input, bool minimal_changes) = 0;

  
  
  
  
  virtual void Stop(bool clear_cached_results);

  
  
  
  metrics::OmniboxEventProto_ProviderType AsOmniboxEventProviderType() const;

  
  
  
  
  
  virtual void DeleteMatch(const AutocompleteMatch& match);

  
  
  
  
  virtual void AddProviderInfo(ProvidersInfo* provider_info) const;

  
  
  
  virtual void ResetSession();

  
  
  
  base::string16 StringForURLDisplay(const GURL& url,
                                     bool check_accept_lang,
                                     bool trim_http) const;

  
  const ACMatches& matches() const { return matches_; }

  
  bool done() const { return done_; }

  
  Type type() const { return type_; }

  
  const char* GetName() const;

#ifdef UNIT_TEST
  void set_listener(AutocompleteProviderListener* listener) {
    listener_ = listener;
  }
#endif
  
  
  
  
  static const size_t kMaxMatches;

 protected:
  friend class base::RefCountedThreadSafe<AutocompleteProvider>;

  virtual ~AutocompleteProvider();

  
  
  void UpdateStarredStateOfMatches();

  
  
  
  
  
  
  
  
  
  
  
  
  static bool FixupUserInput(AutocompleteInput* input);

  
  
  
  
  static size_t TrimHttpPrefix(base::string16* url);

  
  
  Profile* profile_;

  AutocompleteProviderListener* listener_;
  ACMatches matches_;
  bool done_;

  Type type_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutocompleteProvider);
};

typedef std::vector<AutocompleteProvider*> ACProviders;

#endif  
