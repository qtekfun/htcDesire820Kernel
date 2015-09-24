// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRERENDER_PRERENDER_CONTENTS_H_
#define CHROME_BROWSER_PRERENDER_PRERENDER_CONTENTS_H_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/values.h"
#include "chrome/browser/prerender/prerender_final_status.h"
#include "chrome/browser/prerender/prerender_origin.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/referrer.h"
#include "ui/gfx/size.h"

class Profile;

namespace base {
class ProcessMetrics;
}

namespace content {
struct FaviconURL;
class RenderViewHost;
class SessionStorageNamespace;
class WebContents;
}

namespace history {
struct HistoryAddPageArgs;
}

namespace prerender {

class PrerenderHandle;
class PrerenderManager;

class PrerenderContents : public content::NotificationObserver,
                          public content::WebContentsObserver {
 public:
  
  
  class Factory {
   public:
    Factory() {}
    virtual ~Factory() {}

    
    
    virtual PrerenderContents* CreatePrerenderContents(
        PrerenderManager* prerender_manager,
        Profile* profile,
        const GURL& url,
        const content::Referrer& referrer,
        Origin origin,
        uint8 experiment_id) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Factory);
  };

  class Observer {
   public:
    
    virtual void OnPrerenderStart(PrerenderContents* contents) = 0;

    
    virtual void OnPrerenderStopLoading(PrerenderContents* contents);

    
    virtual void OnPrerenderStop(PrerenderContents* contents) = 0;

    
    virtual void OnPrerenderCreatedMatchCompleteReplacement(
        PrerenderContents* contents, PrerenderContents* replacement);

   protected:
    Observer();
    virtual ~Observer() = 0;
  };

  
  struct PendingPrerenderInfo {
   public:
    PendingPrerenderInfo(
        base::WeakPtr<PrerenderHandle> weak_prerender_handle,
        Origin origin,
        const GURL& url,
        const content::Referrer& referrer,
        const gfx::Size& size);

    ~PendingPrerenderInfo();

    base::WeakPtr<PrerenderHandle> weak_prerender_handle;
    Origin origin;
    GURL url;
    content::Referrer referrer;
    gfx::Size size;
  };

  
  
  
  
  
  enum MatchCompleteStatus {
    
    
    MATCH_COMPLETE_DEFAULT,
    
    
    
    MATCH_COMPLETE_REPLACED,
    
    
    
    MATCH_COMPLETE_REPLACEMENT,
    
    
    
    MATCH_COMPLETE_REPLACEMENT_PENDING,
  };

  virtual ~PrerenderContents();

  
  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  PrerenderContents* CreateMatchCompleteReplacement();

  bool Init();

  static Factory* CreateFactory();

  
  
  
  
  
  
  
  
  
  virtual void StartPrerendering(
      int creator_child_id,
      const gfx::Size& size,
      content::SessionStorageNamespace* session_storage_namespace);

  
  
  void DestroyWhenUsingTooManyResources();

  content::RenderViewHost* GetRenderViewHostMutable();
  const content::RenderViewHost* GetRenderViewHost() const;

  PrerenderManager* prerender_manager() { return prerender_manager_; }

  base::string16 title() const { return title_; }
  int32 page_id() const { return page_id_; }
  GURL icon_url() const { return icon_url_; }
  const GURL& prerender_url() const { return prerender_url_; }
  const content::Referrer& referrer() const { return referrer_; }
  bool has_stopped_loading() const { return has_stopped_loading_; }
  bool has_finished_loading() const { return has_finished_loading_; }
  bool prerendering_has_started() const { return prerendering_has_started_; }
  MatchCompleteStatus match_complete_status() const {
    return match_complete_status_;
  }
  void set_match_complete_status(MatchCompleteStatus status) {
    match_complete_status_ = status;
  }

  
  
  virtual bool GetChildId(int* child_id) const;

  
  
  virtual bool GetRouteId(int* route_id) const;

  
  
  
  void SetFinalStatus(FinalStatus final_status);
  FinalStatus final_status() const { return final_status_; }

  Origin origin() const { return origin_; }
  uint8 experiment_id() const { return experiment_id_; }
  int child_id() const { return child_id_; }

  base::TimeTicks load_start_time() const { return load_start_time_; }

  
  
  bool Matches(
      const GURL& url,
      const content::SessionStorageNamespace* session_storage_namespace) const;

  
  virtual void RenderFrameCreated(
      content::RenderFrameHost* render_frame_host) OVERRIDE;
  virtual void RenderFrameDeleted(
      content::RenderFrameHost* render_frame_host) OVERRIDE;
  virtual void DidStopLoading(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidStartProvisionalLoadForFrame(
      int64 frame_id,
      int64 parent_frame_id,
      bool is_main_frame,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFinishLoad(
      int64 frame_id,
      const GURL& validated_url,
      bool is_main_frame,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;
  virtual void DidGetRedirectForResourceRequest(
      const content::ResourceRedirectDetails& details) OVERRIDE;
  virtual void DidUpdateFaviconURL(int32 page_id,
      const std::vector<content::FaviconURL>& urls) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  virtual void RenderProcessGone(base::TerminationStatus status) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  virtual bool CheckURL(const GURL& url);

  
  
  bool AddAliasURL(const GURL& url);

  
  content::WebContents* prerender_contents() const {
    return prerender_contents_.get();
  }

  content::WebContents* ReleasePrerenderContents();

  
  
  void Destroy(FinalStatus reason);

  
  
  
  void DidNavigate(const history::HistoryAddPageArgs& add_page_args);

  
  
  void CommitHistory(content::WebContents* tab);

  base::Value* GetAsValue() const;

  
  
  
  bool IsCrossSiteNavigationPending() const;

  
  
  virtual void AddPendingPrerender(
      scoped_ptr<PendingPrerenderInfo> pending_prerender_info);

  
  
  void PrepareForUse();

  content::SessionStorageNamespace* GetSessionStorageNamespace() const;

  
  enum CookieEvent {
    COOKIE_EVENT_SEND = 0,
    COOKIE_EVENT_CHANGE = 1,
    COOKIE_EVENT_MAX
  };

  
  
  
  void RecordCookieEvent(CookieEvent event,
                         bool main_frame_http_request,
                         base::Time earliest_create_date);

  static const int kNumCookieStatuses;

 protected:
  PrerenderContents(PrerenderManager* prerender_manager,
                    Profile* profile,
                    const GURL& url,
                    const content::Referrer& referrer,
                    Origin origin,
                    uint8 experiment_id);

  
  
  void NotifyPrerenderStart();
  void NotifyPrerenderStopLoading();
  void NotifyPrerenderStop();
  void NotifyPrerenderCreatedMatchCompleteReplacement(
      PrerenderContents* replacement);

  
  
  virtual void OnRenderViewHostCreated(
      content::RenderViewHost* new_render_view_host);

  content::NotificationRegistrar& notification_registrar() {
    return notification_registrar_;
  }

  size_t pending_prerender_count() const;

  bool prerendering_has_been_cancelled() const {
    return prerendering_has_been_cancelled_;
  }

  virtual content::WebContents* CreateWebContents(
      content::SessionStorageNamespace* session_storage_namespace);

  bool prerendering_has_started_;

  
  
  
  base::TimeTicks load_start_time_;

  
  scoped_ptr<content::WebContents> prerender_contents_;

  
  
  
  int64 session_storage_namespace_id_;

  
  
  base::Time start_time_;

 private:
  class WebContentsDelegateImpl;

  
  friend class PrerenderContentsFactoryImpl;

  
  base::ProcessMetrics* MaybeGetProcessMetrics();

  
  void OnCancelPrerenderForPrinting();

  ObserverList<Observer> observer_list_;

  
  PrerenderManager* prerender_manager_;

  
  GURL prerender_url_;

  
  content::Referrer referrer_;

  
  Profile* profile_;

  
  
  
  
  
  base::string16 title_;
  int32 page_id_;
  GURL url_;
  GURL icon_url_;
  content::NotificationRegistrar notification_registrar_;

  
  
  
  std::vector<GURL> alias_urls_;

  bool has_stopped_loading_;

  
  bool has_finished_loading_;

  
  
  FinalStatus final_status_;

  
  
  
  MatchCompleteStatus match_complete_status_;

  
  
  bool prerendering_has_been_cancelled_;

  
  
  scoped_ptr<base::ProcessMetrics> process_metrics_;

  scoped_ptr<WebContentsDelegateImpl> web_contents_delegate_;

  
  int child_id_;
  int route_id_;

  
  Origin origin_;

  
  uint8 experiment_id_;

  
  
  ScopedVector<PendingPrerenderInfo> pending_prerenders_;

  
  int creator_child_id_;

  
  gfx::Size size_;

  typedef std::vector<history::HistoryAddPageArgs> AddPageVector;

  
  AddPageVector add_page_vector_;

  
  scoped_refptr<content::SessionStorageNamespace>
      alias_session_storage_namespace;

  
  std::set<content::RenderFrameHost*> render_frame_hosts_;

  
  
  int cookie_status_;

  DISALLOW_COPY_AND_ASSIGN(PrerenderContents);
};

}  

#endif  
