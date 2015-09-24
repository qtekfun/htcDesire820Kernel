// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_LIMITER_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_REQUEST_LIMITER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class DownloadRequestInfoBarDelegate;
class NavigationController;
class TabContents;

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

  
  class Callback {
   public:
    virtual void ContinueDownload() = 0;
    virtual void CancelDownload() = 0;

   protected:
    virtual ~Callback() {}
  };

  
  
  
  
  class TabDownloadState : public NotificationObserver {
   public:
    
    
    
    
    
    
    TabDownloadState(DownloadRequestLimiter* host,
                     NavigationController* controller,
                     NavigationController* originating_controller);
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

    
    
    void OnUserGesture();

    
    
    
    void PromptUserForDownload(TabContents* tab,
                               DownloadRequestLimiter::Callback* callback);

    
    bool is_showing_prompt() const { return (infobar_ != NULL); }

    
    NavigationController* controller() const { return controller_; }

    
    
    virtual void Cancel();
    virtual void Accept();

   protected:
    
    TabDownloadState()
        : host_(NULL),
          controller_(NULL),
          status_(DownloadRequestLimiter::ALLOW_ONE_DOWNLOAD),
          download_count_(0),
          infobar_(NULL) {
    }

   private:
    
    virtual void Observe(NotificationType type,
                         const NotificationSource& source,
                         const NotificationDetails& details);

    
    
    void NotifyCallbacks(bool allow);

    DownloadRequestLimiter* host_;

    NavigationController* controller_;

    
    std::string initial_page_host_;

    DownloadRequestLimiter::DownloadStatus status_;

    size_t download_count_;

    
    
    
    
    std::vector<DownloadRequestLimiter::Callback*> callbacks_;

    
    NotificationRegistrar registrar_;

    
    DownloadRequestInfoBarDelegate* infobar_;

    DISALLOW_COPY_AND_ASSIGN(TabDownloadState);
  };

  DownloadRequestLimiter();

  
  
  DownloadStatus GetDownloadStatus(TabContents* tab);

  
  
  
  
  
  void CanDownloadOnIOThread(int render_process_host_id,
                             int render_view_id,
                             int request_id,
                             Callback* callback);

  
  
  
  void OnUserGesture(TabContents* tab);

 private:
  friend class base::RefCountedThreadSafe<DownloadRequestLimiter>;
  friend class DownloadRequestLimiterTest;
  friend class TabDownloadState;

  ~DownloadRequestLimiter();

  
  class TestingDelegate {
   public:
    virtual bool ShouldAllowDownload() = 0;

   protected:
    virtual ~TestingDelegate() {}
  };
  static void SetTestingDelegate(TestingDelegate* delegate);

  
  
  
  
  
  
  
  TabDownloadState* GetDownloadState(
      NavigationController* controller,
      NavigationController* originating_controller,
      bool create);

  
  
  void CanDownload(int render_process_host_id,
                   int render_view_id,
                   int request_id,
                   Callback* callback);

  
  
  void CanDownloadImpl(TabContents* originating_tab,
                       int request_id,
                       Callback* callback);

  
  
  void ScheduleNotification(Callback* callback, bool allow);

  
  void NotifyCallback(Callback* callback, bool allow);

  
  
  
  void Remove(TabDownloadState* state);

  
  
  
  
  typedef std::map<NavigationController*, TabDownloadState*> StateMap;
  StateMap state_map_;

  static TestingDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(DownloadRequestLimiter);
};

#endif  
