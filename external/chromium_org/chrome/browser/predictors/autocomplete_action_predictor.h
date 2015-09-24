// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PREDICTORS_AUTOCOMPLETE_ACTION_PREDICTOR_H_
#define CHROME_BROWSER_PREDICTORS_AUTOCOMPLETE_ACTION_PREDICTOR_H_

#include <map>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/predictors/autocomplete_action_predictor_table.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

struct AutocompleteMatch;
class AutocompleteResult;
class HistoryService;
struct OmniboxLog;
class PredictorsHandler;
class Profile;

namespace content {
class SessionStorageNamespace;
}

namespace gfx {
class Size;
}

namespace history {
class URLDatabase;
}

namespace prerender {
class PrerenderHandle;
}

namespace predictors {

class AutocompleteActionPredictor
    : public BrowserContextKeyedService,
      public content::NotificationObserver,
      public base::SupportsWeakPtr<AutocompleteActionPredictor> {
 public:
  enum Action {
    ACTION_PRERENDER = 0,
    ACTION_PRECONNECT,
    ACTION_NONE,
    LAST_PREDICT_ACTION = ACTION_NONE
  };

  explicit AutocompleteActionPredictor(Profile* profile);
  virtual ~AutocompleteActionPredictor();

  
  
  
  void RegisterTransitionalMatches(const base::string16& user_text,
                                   const AutocompleteResult& result);

  
  
  void ClearTransitionalMatches();

  
  
  
  
  
  
  Action RecommendAction(const base::string16& user_text,
                         const AutocompleteMatch& match) const;

  
  
  
  
  void StartPrerendering(
      const GURL& url,
      const content::SessionStorageNamespaceMap& session_storage_namespace_map,
      const gfx::Size& size);

  
  
  static bool IsPreconnectable(const AutocompleteMatch& match);

 private:
  friend class AutocompleteActionPredictorTest;
  friend class ::PredictorsHandler;

  struct TransitionalMatch {
    TransitionalMatch();
    ~TransitionalMatch();

    base::string16 user_text;
    std::vector<GURL> urls;

    bool operator==(const base::string16& other_user_text) const {
      return user_text == other_user_text;
    }
  };

  struct DBCacheKey {
    base::string16 user_text;
    GURL url;

    bool operator<(const DBCacheKey& rhs) const {
      return (user_text != rhs.user_text) ?
          (user_text < rhs.user_text) :  (url < rhs.url);
    }

    bool operator==(const DBCacheKey& rhs) const {
      return (user_text == rhs.user_text) && (url == rhs.url);
    }
  };

  struct DBCacheValue {
    int number_of_hits;
    int number_of_misses;
  };

  typedef std::map<DBCacheKey, DBCacheValue> DBCacheMap;
  typedef std::map<DBCacheKey, AutocompleteActionPredictorTable::Row::Id>
      DBIdCacheMap;

  static const int kMaximumDaysToKeepEntry;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void CreateLocalCachesFromDatabase();

  
  void DeleteAllRows();

  
  void DeleteRowsWithURLs(const history::URLRows& rows);

  
  void OnOmniboxOpenedUrl(const OmniboxLog& log);

  
  void AddAndUpdateRows(
    const AutocompleteActionPredictorTable::Rows& rows_to_add,
    const AutocompleteActionPredictorTable::Rows& rows_to_update);

  
  
  
  void CreateCaches(
      std::vector<AutocompleteActionPredictorTable::Row>* row_buffer);

  
  
  bool TryDeleteOldEntries(HistoryService* service);

  
  
  void DeleteOldEntries(history::URLDatabase* url_db);

  
  
  void DeleteOldIdsFromCaches(
      history::URLDatabase* url_db,
      std::vector<AutocompleteActionPredictorTable::Row::Id>* id_list);

  
  
  void CopyFromMainProfile();

  
  void FinishInitialization();

  
  
  
  
  double CalculateConfidence(const base::string16& user_text,
                             const AutocompleteMatch& match,
                             bool* is_in_db) const;

  
  double CalculateConfidenceForDbEntry(DBCacheMap::const_iterator iter) const;

  Profile* profile_;

  
  AutocompleteActionPredictor* main_profile_predictor_;

  
  
  
  AutocompleteActionPredictor* incognito_predictor_;

  
  scoped_refptr<AutocompleteActionPredictorTable> table_;

  content::NotificationRegistrar notification_registrar_;

  
  std::vector<TransitionalMatch> transitional_matches_;

  scoped_ptr<prerender::PrerenderHandle> prerender_handle_;

  
  
  mutable std::vector<std::pair<GURL, double> > tracked_urls_;

  
  
  DBCacheMap db_cache_;
  DBIdCacheMap db_id_cache_;

  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteActionPredictor);
};

}  

#endif  
