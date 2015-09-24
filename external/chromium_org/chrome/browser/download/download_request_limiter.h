// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_LIMITER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_LIMITER_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/content_settings.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"

class HostContentSettingsMap;
class DownloadRequestInfoBarDelegate;

namespace content {
class NavigationController;
class WebContents;
}

class DownloadRequestLimiter
    : public base::RefCountedThreadSafe<DownloadRequestLimiter> {
 public:
  
  enum DownloadStatus {
    ALLOW_ONE_DOWNLOAD,
    PROMPT_BEFORE_DOWNLOAD,
    ALLOW_ALL_DOWNLOADS,
    DOWNLOADS_NOT_ALLOWED
  };

  
  static const size_t kMaxDownloadsAtOnce = 50;

  
  
  typedef base::Callback<void(bool )> Callback;

  
  
  
  
  class TabDownloadState : public content::NotificationObserver,
                           public content::WebContentsObserver {
   public:
    
    
    
    
    
    
    TabDownloadState(DownloadRequestLimiter* host,
                     content::WebContents* web_contents,
                     content::WebContents* originating_web_contents);
    virtual ~TabDownloadState();

    
    void set_download_status(DownloadRequestLimiter::DownloadStatus status) {
      status_ = status;
    }
    DownloadRequestLimiter::DownloadStatus download_status() const {
      return status_;
    }

    
    void increment_download_count() {
      download_count_++;
    }
    size_t download_count() const {
      return download_count_;
    }

    
    content::WebContents* web_contents() {
      return content::WebContentsObserver::web_contents();
    }

    
    virtual void AboutToNavigateRenderView(
        content::RenderViewHost* render_view_host) OVERRIDE;
    
    
    virtual void DidGetUserGesture() OVERRIDE;
    virtual void WebContentsDestroyed(
        content::WebContents* web_contents) OVERRIDE;

    
    
    
    void PromptUserForDownload(
        const DownloadRequestLimiter::Callback& callback);

    
    
    virtual void Cancel();
    virtual void CancelOnce();
    virtual void Accept();

   protected:
    
    TabDownloadState();

   private:
    
    
    
    bool is_showing_prompt() const { return factory_.HasWeakPtrs(); }

    
    virtual void Observe(int type,
                         const content::NotificationSource& source,
                         const content::NotificationDetails& details) OVERRIDE;

    
    void SetContentSetting(ContentSetting setting);

    
    
    void NotifyCallbacks(bool allow);

    content::WebContents* web_contents_;

    DownloadRequestLimiter* host_;

    
    std::string initial_page_host_;

    DownloadRequestLimiter::DownloadStatus status_;

    size_t download_count_;

    
    
    
    
    std::vector<DownloadRequestLimiter::Callback> callbacks_;

    
    content::NotificationRegistrar registrar_;

    
    
    
    
    base::WeakPtrFactory<DownloadRequestLimiter::TabDownloadState> factory_;

    DISALLOW_COPY_AND_ASSIGN(TabDownloadState);
  };

  static void SetContentSettingsForTesting(HostContentSettingsMap* settings);

  DownloadRequestLimiter();

  
  
  DownloadStatus GetDownloadStatus(content::WebContents* tab);

  
  
  void CanDownloadOnIOThread(int render_process_host_id,
                             int render_view_id,
                             int request_id,
                             const std::string& request_method,
                             const Callback& callback);

 private:
  FRIEND_TEST_ALL_PREFIXES(DownloadTest, DownloadResourceThrottleCancels);
  friend class base::RefCountedThreadSafe<DownloadRequestLimiter>;
  friend class DownloadRequestLimiterTest;
  friend class TabDownloadState;

  ~DownloadRequestLimiter();

  
  
  
  
  
  
  
  TabDownloadState* GetDownloadState(
      content::WebContents* web_contents,
      content::WebContents* originating_web_contents,
      bool create);

  
  
  void CanDownload(int render_process_host_id,
                   int render_view_id,
                   int request_id,
                   const std::string& request_method,
                   const Callback& callback);

  
  
  void CanDownloadImpl(content::WebContents* originating_contents,
                       int request_id,
                       const std::string& request_method,
                       const Callback& callback);

  
  void OnCanDownloadDecided(int render_process_host_id,
                            int render_view_id,
                            int request_id,
                            const std::string& request_method,
                            const Callback& orig_callback,
                            bool allow);

  
  
  void ScheduleNotification(const Callback& callback, bool allow);

  
  
  
  void Remove(TabDownloadState* state, content::WebContents* contents);

  static HostContentSettingsMap* content_settings_;
  static HostContentSettingsMap* GetContentSettings(
      content::WebContents* contents);

  
  
  
  
  typedef std::map<content::WebContents*, TabDownloadState*> StateMap;
  StateMap state_map_;

  
  
  base::WeakPtrFactory<DownloadRequestLimiter> factory_;

  DISALLOW_COPY_AND_ASSIGN(DownloadRequestLimiter);
};

#endif  
