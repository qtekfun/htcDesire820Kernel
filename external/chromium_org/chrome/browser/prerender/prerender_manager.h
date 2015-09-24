// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_MANAGER_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_MANAGER_H_

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/media/media_capture_devices_dispatcher.h"
#include "chrome/browser/predictors/logged_in_predictor_table.h"
#include "chrome/browser/prerender/prerender_config.h"
#include "chrome/browser/prerender/prerender_contents.h"
#include "chrome/browser/prerender/prerender_events.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "chrome/browser/prerender/prerender_origin.h"
#include "chrome/browser/prerender/prerender_tracker.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/session_storage_namespace.h"
#include "content/public/browser/web_contents_observer.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_monster.h"
#include "url/gurl.h"

class Profile;
class InstantSearchPrerendererTest;
struct ChromeCookieDetails;

namespace base {
class DictionaryValue;
}

namespace chrome {
struct NavigateParams;
}

namespace content {
class WebContents;
}

namespace gfx {
class Size;
}

namespace net {
class URLRequestContextGetter;
}

#if defined(COMPILER_GCC)

namespace BASE_HASH_NAMESPACE {
template <>
struct hash<content::WebContents*> {
  std::size_t operator()(content::WebContents* value) const {
    return reinterpret_cast<std::size_t>(value);
  }
};

}  

#endif

namespace prerender {

class PrerenderCondition;
class PrerenderHandle;
class PrerenderHistograms;
class PrerenderHistory;
class PrerenderLocalPredictor;

class PrerenderManager : public base::SupportsWeakPtr<PrerenderManager>,
                         public base::NonThreadSafe,
                         public content::NotificationObserver,
                         public BrowserContextKeyedService,
                         public MediaCaptureDevicesDispatcher::Observer {
 public:
  
  enum PrerenderManagerMode {
    PRERENDER_MODE_DISABLED = 0,
    PRERENDER_MODE_ENABLED = 1,
    PRERENDER_MODE_EXPERIMENT_CONTROL_GROUP = 2,
    PRERENDER_MODE_EXPERIMENT_PRERENDER_GROUP = 3,
    
    PRERENDER_MODE_EXPERIMENT_NO_USE_GROUP = 5,
    PRERENDER_MODE_EXPERIMENT_MULTI_PRERENDER_GROUP = 6,
    PRERENDER_MODE_EXPERIMENT_15MIN_TTL_GROUP = 7,
    PRERENDER_MODE_MAX
  };

  
  
  enum ClearFlags {
    CLEAR_PRERENDER_CONTENTS = 0x1 << 0,
    CLEAR_PRERENDER_HISTORY = 0x1 << 1,
    CLEAR_MAX = 0x1 << 2
  };

  typedef predictors::LoggedInPredictorTable::LoggedInStateMap LoggedInStateMap;

  
  static const uint8 kNoExperiment = 0;

  
  PrerenderManager(Profile* profile, PrerenderTracker* prerender_tracker);

  virtual ~PrerenderManager();

  
  virtual void Shutdown() OVERRIDE;

  

  
  
  
  
  
  
  PrerenderHandle* AddPrerenderFromLinkRelPrerender(
      int process_id,
      int route_id,
      const GURL& url,
      const content::Referrer& referrer,
      const gfx::Size& size);

  
  
  
  
  
  
  PrerenderHandle* AddPrerenderFromOmnibox(
      const GURL& url,
      content::SessionStorageNamespace* session_storage_namespace,
      const gfx::Size& size);

  PrerenderHandle* AddPrerenderFromLocalPredictor(
      const GURL& url,
      content::SessionStorageNamespace* session_storage_namespace,
      const gfx::Size& size);

  PrerenderHandle* AddPrerenderFromExternalRequest(
      const GURL& url,
      const content::Referrer& referrer,
      content::SessionStorageNamespace* session_storage_namespace,
      const gfx::Size& size);

  
  
  
  
  PrerenderHandle* AddPrerenderForInstant(
      const GURL& url,
      content::SessionStorageNamespace* session_storage_namespace,
      const gfx::Size& size);

  
  
  virtual void DestroyPrerenderForRenderView(int process_id,
                                             int view_id,
                                             FinalStatus final_status);

  
  void CancelAllPrerenders();

  
  
  
  
  bool MaybeUsePrerenderedPage(const GURL& url,
                               chrome::NavigateParams* params);

  
  
  virtual void MoveEntryToPendingDelete(PrerenderContents* entry,
                                        FinalStatus final_status);

  
  
  
  
  
  
  static void RecordPerceivedPageLoadTime(
      base::TimeDelta perceived_page_load_time,
      double fraction_plt_elapsed_at_swap_in,
      content::WebContents* web_contents,
      const GURL& url);

  
  void RecordFractionPixelsFinalAtSwapin(
      content::WebContents* web_contents,
      double fraction);

  
  
  
  
  void set_enabled(bool enabled);

  static PrerenderManagerMode GetMode();
  static void SetMode(PrerenderManagerMode mode);
  static const char* GetModeString();
  static bool IsPrerenderingPossible();
  static bool ActuallyPrerendering();
  static bool IsControlGroup(uint8 experiment_id);
  static bool IsNoUseGroup();

  
  
  
  
  bool IsWebContentsPrerendering(const content::WebContents* web_contents,
                                 Origin* origin) const;

  
  
  bool HasPrerenderedUrl(GURL url, content::WebContents* web_contents) const;

  
  
  PrerenderContents* GetPrerenderContents(
      const content::WebContents* web_contents) const;

  
  const std::vector<content::WebContents*> GetAllPrerenderingContents() const;

  
  
  void MarkWebContentsAsPrerendered(content::WebContents* web_contents,
                                    Origin origin);
  void MarkWebContentsAsWouldBePrerendered(content::WebContents* web_contents,
                                           Origin origin);
  void MarkWebContentsAsNotPrerendered(content::WebContents* web_contents);

  
  
  
  
  bool IsWebContentsPrerendered(content::WebContents* web_contents,
                                Origin* origin) const;
  bool WouldWebContentsBePrerendered(content::WebContents* web_contents,
                                     Origin* origin) const;

  
  bool HasRecentlyBeenNavigatedTo(Origin origin, const GURL& url);

  
  static bool IsValidHttpMethod(const std::string& method);

  
  static bool DoesURLHaveValidScheme(const GURL& url);

  
  
  static bool DoesSubresourceURLHaveValidScheme(const GURL& url);

  
  
  
  base::DictionaryValue* GetAsValue() const;

  
  
  
  
  
  
  
  
  
  
  
  void ClearData(int clear_flags);

  
  
  
  void RecordFinalStatusWithMatchCompleteStatus(
      Origin origin,
      uint8 experiment_id,
      PrerenderContents::MatchCompleteStatus mc_status,
      FinalStatus final_status) const;

  
  void RecordCookieStatus(Origin origin,
                          uint8 experiment_id,
                          int cookie_status) const;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnCreatingAudioStream(int render_process_id,
                                     int render_view_id) OVERRIDE;

  const Config& config() const { return config_; }
  Config& mutable_config() { return config_; }

  PrerenderTracker* prerender_tracker() { return prerender_tracker_; }

  
  void AddCondition(const PrerenderCondition* condition);

  
  
  void RecordNavigation(const GURL& url);

  
  
  void RecordLikelyLoginOnURL(const GURL& url);

  
  
  void CheckIfLikelyLoggedInOnURL(const GURL& url,
                                  bool* lookup_result,
                                  bool* database_was_present,
                                  const base::Closure& result_cb);

  void OnHistoryServiceDidQueryURL(Origin origin,
                                   uint8 experiment_id,
                                   CancelableRequestProvider::Handle handle,
                                   bool success,
                                   const history::URLRow* url_row,
                                   history::VisitVector* visits);

  Profile* profile() const { return profile_; }

  
  
  
  virtual base::Time GetCurrentTime() const;
  virtual base::TimeTicks GetCurrentTimeTicks() const;

  scoped_refptr<predictors::LoggedInPredictorTable>
  logged_in_predictor_table() {
    return logged_in_predictor_table_;
  }

  PrerenderLocalPredictor* local_predictor() {
    return local_predictor_.get();
  }

  
  
  
  static void RecordCookieEvent(int process_id,
                                int render_view_id,
                                const GURL& url,
                                const GURL& frame_url,
                                PrerenderContents::CookieEvent event,
                                const net::CookieList* cookie_list);

 protected:
  class PendingSwap;
  class PrerenderData : public base::SupportsWeakPtr<PrerenderData> {
   public:
    struct OrderByExpiryTime;

    PrerenderData(PrerenderManager* manager,
                  PrerenderContents* contents,
                  base::TimeTicks expiry_time);

    ~PrerenderData();

    
    
    void MakeIntoMatchCompleteReplacement();

    
    void OnHandleCreated(PrerenderHandle* prerender_handle);

    
    
    
    
    void OnHandleNavigatedAway(PrerenderHandle* prerender_handle);

    
    
    
    void OnHandleCanceled(PrerenderHandle* prerender_handle);

    PrerenderContents* contents() { return contents_.get(); }

    PrerenderContents* ReleaseContents();

    int handle_count() const { return handle_count_; }

    base::TimeTicks expiry_time() const { return expiry_time_; }
    void set_expiry_time(base::TimeTicks expiry_time) {
      expiry_time_ = expiry_time;
    }

    void ClearPendingSwap();

    PendingSwap* pending_swap() { return pending_swap_.get(); }
    void set_pending_swap(PendingSwap* pending_swap) {
      pending_swap_.reset(pending_swap);
    }

   private:
    PrerenderManager* manager_;
    scoped_ptr<PrerenderContents> contents_;

    
    
    
    
    
    int handle_count_;

    
    
    base::TimeTicks expiry_time_;

    
    
    scoped_ptr<PendingSwap> pending_swap_;

    DISALLOW_COPY_AND_ASSIGN(PrerenderData);
  };

  
  
  
  
  
  class PendingSwap : public content::WebContentsObserver {
   public:
    PendingSwap(PrerenderManager* manager,
                content::WebContents* target_contents,
                PrerenderData* prerender_data,
                const GURL& url,
                bool should_replace_current_entry);
    virtual ~PendingSwap();

    void set_swap_successful(bool swap_successful) {
      swap_successful_ = swap_successful;
    }

    void BeginSwap();

    
    virtual void ProvisionalChangeToMainFrameUrl(
        const GURL& url,
        content::RenderViewHost* render_view_host) OVERRIDE;
    virtual void DidCommitProvisionalLoadForFrame(
        int64 frame_id,
        const base::string16& frame_unique_name,
        bool is_main_frame,
        const GURL& validated_url,
        content::PageTransition transition_type,
        content::RenderViewHost* render_view_host) OVERRIDE;
    virtual void RenderViewCreated(
        content::RenderViewHost* render_view_host) OVERRIDE;
    virtual void DidFailProvisionalLoad(
        int64 frame_id,
        const base::string16& frame_unique_name,
        bool is_main_frame,
        const GURL& validated_url,
        int error_code,
        const base::string16& error_description,
        content::RenderViewHost* render_view_host) OVERRIDE;
    virtual void WebContentsDestroyed(content::WebContents* web_contents)
        OVERRIDE;

   private:
    void RecordEvent(PrerenderEvent event) const;

    void OnMergeCompleted(content::SessionStorageNamespace::MergeResult result);
    void OnMergeTimeout();

    
    PrerenderManager* manager_;
    content::WebContents* target_contents_;
    PrerenderData* prerender_data_;
    GURL url_;
    bool should_replace_current_entry_;

    base::TimeTicks start_time_;
    std::vector<PrerenderTracker::ChildRouteIdPair> rvh_ids_;
    base::OneShotTimer<PendingSwap> merge_timeout_;
    bool swap_successful_;

    base::WeakPtrFactory<PendingSwap> weak_factory_;
  };

  void SetPrerenderContentsFactory(
      PrerenderContents::Factory* prerender_contents_factory);

  
  
  void StartPendingPrerenders(
      int process_id,
      ScopedVector<PrerenderContents::PendingPrerenderInfo>* pending_prerenders,
      content::SessionStorageNamespace* session_storage_namespace);

  
  
  
  
  
  void SourceNavigatedAway(PrerenderData* prerender_data);

 private:
  friend class ::InstantSearchPrerendererTest;
  friend class PrerenderBrowserTest;
  friend class PrerenderContents;
  friend class PrerenderHandle;
  friend class UnitTestPrerenderManager;

  class OnCloseWebContentsDeleter;
  struct NavigationRecord;

  
  
  
  struct PrerenderedWebContentsData {
    explicit PrerenderedWebContentsData(Origin origin);

    Origin origin;
  };

  
  
  
  
  struct WouldBePrerenderedWebContentsData {
    
    
    
    
    
    
    enum State {
      WAITING_FOR_PROVISIONAL_LOAD,
      SEEN_PROVISIONAL_LOAD,
    };

    explicit WouldBePrerenderedWebContentsData(Origin origin);

    Origin origin;
    State state;
  };

  
  static const int kMinTimeBetweenPrerendersMs = 500;

  
  static const int kNavigationRecordWindowMs = 5000;

  void OnCancelPrerenderHandle(PrerenderData* prerender_data);

  
  
  
  
  
  PrerenderHandle* AddPrerender(
      Origin origin,
      int child_id,
      const GURL& url,
      const content::Referrer& referrer,
      const gfx::Size& size,
      content::SessionStorageNamespace* session_storage_namespace);

  void StartSchedulingPeriodicCleanups();
  void StopSchedulingPeriodicCleanups();

  void EvictOldestPrerendersIfNecessary();

  
  
  
  
  void PeriodicCleanup();

  
  
  
  void PostCleanupTask();

  base::TimeTicks GetExpiryTimeForNewPrerender(Origin origin) const;
  base::TimeTicks GetExpiryTimeForNavigatedAwayPrerender() const;

  void DeleteOldEntries();
  virtual PrerenderContents* CreatePrerenderContents(
      const GURL& url,
      const content::Referrer& referrer,
      Origin origin,
      uint8 experiment_id);

  
  
  
  void SortActivePrerenders();

  
  
  PrerenderData* FindPrerenderData(
      const GURL& url,
      const content::SessionStorageNamespace* session_storage_namespace);

  
  
  
  PrerenderData* FindPrerenderDataForChildAndRoute(int child_id, int route_id);

  
  
  ScopedVector<PrerenderData>::iterator
      FindIteratorForPrerenderContents(PrerenderContents* prerender_contents);

  bool DoesRateLimitAllowPrerender(Origin origin) const;

  
  
  
  void DeleteOldWebContents();

  
  void CleanUpOldNavigations();

  
  
  void ScheduleDeleteOldWebContents(content::WebContents* tab,
                                    OnCloseWebContentsDeleter* deleter);

  
  void AddToHistory(PrerenderContents* contents);

  
  
  base::Value* GetActivePrerendersAsValue() const;

  
  
  
  void DestroyAllContents(FinalStatus final_status);

  
  
  
  void DestroyAndMarkMatchCompleteAsUsed(PrerenderContents* prerender_contents,
                                         FinalStatus final_status);

  
  
  
  void RecordFinalStatus(Origin origin,
                         uint8 experiment_id,
                         FinalStatus final_status) const;

  
  
  bool IsEnabled() const;

  void CookieChanged(ChromeCookieDetails* details);
  void CookieChangedAnyCookiesLeftLookupResult(const std::string& domain_key,
                                               bool cookies_exist);
  void LoggedInPredictorDataReceived(scoped_ptr<LoggedInStateMap> new_map);

  void RecordEvent(PrerenderContents* contents, PrerenderEvent event) const;

  
  
  
  
  content::WebContents* SwapInternal(const GURL& url,
                                     content::WebContents* web_contents,
                                     PrerenderData* prerender_data,
                                     bool should_replace_current_entry);

  
  Config config_;

  
  
  
  bool enabled_;

  
  Profile* profile_;

  PrerenderTracker* prerender_tracker_;

  
  ScopedVector<PrerenderData> active_prerenders_;

  
  ScopedVector<PrerenderData> to_delete_prerenders_;

  
  
  std::list<NavigationRecord> navigations_;

  
  
  
  
  base::hash_map<content::WebContents*, PrerenderedWebContentsData>
      prerendered_web_contents_data_;

  
  
  
  
  
  
  
  
  
  base::hash_map<content::WebContents*, WouldBePrerenderedWebContentsData>
      would_be_prerendered_map_;

  scoped_ptr<PrerenderContents::Factory> prerender_contents_factory_;

  static PrerenderManagerMode mode_;

  
  
  static int prerenders_per_session_count_;

  
  
  base::RepeatingTimer<PrerenderManager> repeating_timer_;

  
  base::TimeTicks last_prerender_start_time_;

  std::list<content::WebContents*> old_web_contents_list_;

  ScopedVector<OnCloseWebContentsDeleter> on_close_web_contents_deleters_;

  scoped_ptr<PrerenderHistory> prerender_history_;

  std::list<const PrerenderCondition*> prerender_conditions_;

  scoped_ptr<PrerenderHistograms> histograms_;

  scoped_ptr<PrerenderLocalPredictor> local_predictor_;

  scoped_refptr<predictors::LoggedInPredictorTable> logged_in_predictor_table_;

  
  
  
  
  
  
  scoped_ptr<LoggedInStateMap> logged_in_state_;

  content::NotificationRegistrar notification_registrar_;

  CancelableRequestConsumer query_url_consumer_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderManager);
};

PrerenderManager* FindPrerenderManagerUsingRenderProcessId(
    int render_process_id);

}  

#endif  
