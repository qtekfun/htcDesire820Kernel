// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_MODEL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_MODEL_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/search_engines/search_host_to_urls_map.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/browser/webdata/web_data_service.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class Extension;
class PrefService;
class Profile;
class PrefSetObserver;
class SearchHostToURLsMap;
class SearchTermsData;
class TemplateURLModelObserver;
class TemplateURLRef;

namespace history {
struct URLVisitedDetails;
}


class TemplateURLModel : public WebDataServiceConsumer,
                         public NotificationObserver {
 public:
  typedef std::map<std::string, std::string> QueryTerms;
  typedef std::vector<const TemplateURL*> TemplateURLVector;

  
  
  struct Initializer {
    const char* const keyword;
    const char* const url;
    const char* const content;
  };

  explicit TemplateURLModel(Profile* profile);
  
  TemplateURLModel(const Initializer* initializers, const int count);
  virtual ~TemplateURLModel();

  
  
  
  
  
  static string16 GenerateKeyword(const GURL& url, bool autodetected);

  
  
  static string16 CleanUserInputKeyword(const string16& keyword);

  
  
  static GURL GenerateSearchURL(const TemplateURL* t_url);

  
  
  
  static GURL GenerateSearchURLUsingTermsData(
      const TemplateURL* t_url,
      const SearchTermsData& search_terms_data);

  
  
  
  
  
  
  
  bool CanReplaceKeyword(const string16& keyword,
                         const GURL& url,
                         const TemplateURL** template_url_to_replace);

  
  
  
  void FindMatchingKeywords(const string16& prefix,
                            bool support_replacement_only,
                            std::vector<string16>* matches) const;

  
  
  
  
  const TemplateURL* GetTemplateURLForKeyword(const string16& keyword) const;

  
  
  const TemplateURL* GetTemplateURLForHost(const std::string& host) const;

  
  
  void Add(TemplateURL* template_url);

  
  
  void Remove(const TemplateURL* template_url);

  
  
  void RemoveAutoGeneratedBetween(base::Time created_after,
                                  base::Time created_before);

  
  
  void RemoveAutoGeneratedSince(base::Time created_after);

  
  
  
  void RegisterExtensionKeyword(const Extension* extension);

  
  
  void UnregisterExtensionKeyword(const Extension* extension);

  
  
  
  const TemplateURL* GetTemplateURLForExtension(
      const Extension* extension) const;

  
  
  TemplateURLVector GetTemplateURLs() const;

  
  
  void IncrementUsageCount(const TemplateURL* url);

  
  
  void ResetTemplateURL(const TemplateURL* url,
                        const string16& title,
                        const string16& keyword,
                        const std::string& search_url);

  
  bool CanMakeDefault(const TemplateURL* url);

  
  
  
  void SetDefaultSearchProvider(const TemplateURL* url);

  
  
  
  
  const TemplateURL* GetDefaultSearchProvider();

  
  bool is_default_search_managed() const { return is_default_search_managed_; }

  
  
  void AddObserver(TemplateURLModelObserver* observer);
  void RemoveObserver(TemplateURLModelObserver* observer);

  
  
  
  
  void Load();

  
  bool loaded() { return loaded_; }

  
  
  
  virtual void OnWebDataServiceRequestDone(WebDataService::Handle h,
                                           const WDTypedResult* result);

  
  
  
  string16 GetKeywordShortName(const string16& keyword,
                               bool* is_extension_keyword);

  
  
  
  
  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  Profile* profile() const { return profile_; }

  void SetSearchEngineDialogSlot(int slot) {
    search_engine_dialog_chosen_slot_ = slot;
  }

  int GetSearchEngineDialogSlot() const {
    return search_engine_dialog_chosen_slot_;
  }

  
  static void RegisterUserPrefs(PrefService* prefs);

 protected:
  
  
  
  
  virtual void SetKeywordSearchTermsForURL(const TemplateURL* t_url,
                                           const GURL& url,
                                           const string16& term);

 private:
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest, BuildQueryTerms);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest, TestManagedDefaultSearch);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest,
                           UpdateKeywordSearchTermsForURL);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest,
                           DontUpdateKeywordSearchForNonReplaceable);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest, ChangeGoogleBaseValue);
  FRIEND_TEST_ALL_PREFIXES(TemplateURLModelTest, MergeDeletesUnusedProviders);
  friend class TemplateURLModelTestUtil;

  typedef std::map<string16, const TemplateURL*> KeywordToTemplateMap;

  
  
  class LessWithPrefix;

  void Init(const Initializer* initializers, int num_initializers);

  void RemoveFromMaps(const TemplateURL* template_url);

  
  
  
  void RemoveFromKeywordMapByPointer(const TemplateURL* template_url);

  void AddToMaps(const TemplateURL* template_url);

  
  
  void SetTemplateURLs(const std::vector<TemplateURL*>& urls);

  
  void ChangeToLoadedState();

  
  
  void NotifyLoaded();

  
  
  void SaveDefaultSearchProviderToPrefs(const TemplateURL* url);

  
  
  
  
  
  
  bool LoadDefaultSearchProviderFromPrefs(
      scoped_ptr<TemplateURL>* default_provider,
      bool* is_managed);

  
  
  
  bool CanReplaceKeywordForHost(const std::string& host,
                                const TemplateURL** to_replace);

  
  
  
  
  bool CanReplace(const TemplateURL* t_url);

  
  
  
  void UpdateNoNotify(const TemplateURL* existing_turl,
                      const TemplateURL& new_values);

  
  PrefService* GetPrefs();

  
  
  
  void UpdateKeywordSearchTermsForURL(
      const history::URLVisitedDetails& details);

  
  void AddTabToSearchVisit(const TemplateURL& t_url);

  
  
  
  
  static bool BuildQueryTerms(
      const GURL& url,
      std::map<std::string,std::string>* query_terms);

  
  
  
  void GoogleBaseURLChanged();

  
  
  void UpdateDefaultSearch();

  
  
  
  const TemplateURL* FindNewDefaultSearchProvider();

  
  
  void SetDefaultSearchProviderNoNotify(const TemplateURL* url);

  
  
  
  void AddNoNotify(TemplateURL* template_url);

  
  
  
  void RemoveNoNotify(const TemplateURL* template_url);

  
  
  void NotifyObservers();

  NotificationRegistrar registrar_;

  
  KeywordToTemplateMap keyword_to_template_map_;

  TemplateURLVector template_urls_;

  ObserverList<TemplateURLModelObserver> model_observers_;

  
  SearchHostToURLsMap provider_map_;

  
  
  
  Profile* profile_;

  
  bool loaded_;

  
  bool load_failed_;

  
  WebDataService::Handle load_handle_;

  
  scoped_refptr<WebDataService> service_;

  
  
  std::vector<history::URLVisitedDetails> visits_to_add_;

  
  
  const TemplateURL* default_search_provider_;

  
  
  int search_engine_dialog_chosen_slot_;

  
  
  scoped_ptr<TemplateURL> initial_default_search_provider_;

  
  bool is_default_search_managed_;

  
  
  scoped_ptr<PrefSetObserver> default_search_prefs_;

  
  
  TemplateURLID next_id_;

  
  std::vector<std::string> pending_extension_ids_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLModel);
};

#endif  