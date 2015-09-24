// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_REMOVER_H_
#define CHROME_BROWSER_BROWSING_DATA_REMOVER_H_
#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/synchronization/waitable_event_watcher.h"
#include "base/time.h"
#include "content/browser/appcache/chrome_appcache_service.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_registrar.h"

class ExtensionSpecialStoragePolicy;
class IOThread;
class PluginDataRemover;
class Profile;

namespace disk_cache {
class Backend;
}

namespace net {
class URLRequestContextGetter;
}

namespace webkit_database {
class DatabaseTracker;
}


class BrowsingDataRemover : public NotificationObserver,
                            public base::WaitableEventWatcher::Delegate {
 public:
  
  enum TimePeriod {
    LAST_HOUR = 0,
    LAST_DAY,
    LAST_WEEK,
    FOUR_WEEKS,
    EVERYTHING
  };

  

  
  static const int REMOVE_HISTORY = 1 << 0;
  static const int REMOVE_DOWNLOADS = 1 << 1;
  static const int REMOVE_COOKIES = 1 << 2;
  static const int REMOVE_PASSWORDS = 1 << 3;
  static const int REMOVE_FORM_DATA = 1 << 4;
  static const int REMOVE_CACHE = 1 << 5;
  static const int REMOVE_LSO_DATA = 1 << 6;

  
  
  class Observer {
   public:
    virtual void OnBrowsingDataRemoverDone() = 0;

   protected:
    virtual ~Observer() {}
  };

  
  
  BrowsingDataRemover(Profile* profile, base::Time delete_begin,
                      base::Time delete_end);

  
  
  BrowsingDataRemover(Profile* profile, TimePeriod time_period,
                      base::Time delete_end);

  
  void Remove(int remove_mask);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  void OnHistoryDeletionDone();

  static bool is_removing() { return removing_; }

 private:
  enum CacheState {
    STATE_NONE,
    STATE_CREATE_MAIN,
    STATE_CREATE_MEDIA,
    STATE_DELETE_MAIN,
    STATE_DELETE_MEDIA,
    STATE_DONE
  };

  
  
  
  friend class DeleteTask<BrowsingDataRemover>;
  ~BrowsingDataRemover();

  
  
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  virtual void OnWaitableEventSignaled(base::WaitableEvent* waitable_event);

  
  
  void NotifyAndDeleteIfDone();

  
  
  void ClearedNetworkHistory();

  
  
  void ClearNetworkingHistory(IOThread* io_thread);

  
  void ClearedCache();

  
  void ClearCacheOnIOThread();

  
  void DoClearCache(int rv);

  
  
  void OnClearedDatabases(int rv);

  
  void ClearDatabasesOnFILEThread();

  
  
  void OnClearedAppCache();

  
  void ClearAppCacheOnIOThread();

  
  void OnGotAppCacheInfo(int rv);
  void OnAppCacheDeleted(int rv);
  ChromeAppCacheService* GetAppCacheService();

  
  base::Time CalculateBeginDeleteTime(TimePeriod time_period);

  
  bool all_done() {
    return registrar_.IsEmpty() && !waiting_for_clear_cache_ &&
           !waiting_for_clear_history_ &&
           !waiting_for_clear_networking_history_ &&
           !waiting_for_clear_databases_ && !waiting_for_clear_appcache_ &&
           !waiting_for_clear_lso_data_;
  }

  NotificationRegistrar registrar_;

  
  Profile* profile_;

  
  scoped_refptr<ExtensionSpecialStoragePolicy> special_storage_policy_;

  
  const base::Time delete_begin_;

  
  const base::Time delete_end_;

  
  static bool removing_;

  
  scoped_refptr<webkit_database::DatabaseTracker> database_tracker_;

  net::CompletionCallbackImpl<BrowsingDataRemover> database_cleared_callback_;
  net::CompletionCallbackImpl<BrowsingDataRemover> cache_callback_;

  
  net::CompletionCallbackImpl<BrowsingDataRemover> appcache_got_info_callback_;
  net::CompletionCallbackImpl<BrowsingDataRemover> appcache_deleted_callback_;
  scoped_refptr<appcache::AppCacheInfoCollection> appcache_info_;
  int appcaches_to_be_deleted_count_;

  
  CacheState next_cache_state_;
  disk_cache::Backend* cache_;

  
  scoped_refptr<net::URLRequestContextGetter> main_context_getter_;
  scoped_refptr<net::URLRequestContextGetter> media_context_getter_;

  
  scoped_refptr<PluginDataRemover> plugin_data_remover_;
  base::WaitableEventWatcher watcher_;

  
  bool waiting_for_clear_databases_;
  bool waiting_for_clear_history_;
  bool waiting_for_clear_networking_history_;
  bool waiting_for_clear_cache_;
  bool waiting_for_clear_appcache_;
  bool waiting_for_clear_lso_data_;

  ObserverList<Observer> observer_list_;

  
  CancelableRequestConsumer request_consumer_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataRemover);
};

#endif  
