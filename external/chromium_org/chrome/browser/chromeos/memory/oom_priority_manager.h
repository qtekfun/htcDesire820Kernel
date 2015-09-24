// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_MEMORY_OOM_PRIORITY_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_MEMORY_OOM_PRIORITY_MANAGER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chromeos/memory/low_memory_listener_delegate.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class GURL;

namespace chromeos {

class LowMemoryListener;

class OomPriorityManager : public content::NotificationObserver,
                           public LowMemoryListenerDelegate {
 public:
  OomPriorityManager();
  virtual ~OomPriorityManager();

  
  int discard_count() const { return discard_count_; }

  
  bool recent_tab_discard() const { return recent_tab_discard_; }

  void Start();
  void Stop();

  
  
  std::vector<base::string16> GetTabTitles();

  
  
  
  bool DiscardTab();

  
  
  
  void LogMemoryAndDiscardTab();

 private:
  friend class OomMemoryDetails;
  FRIEND_TEST_ALL_PREFIXES(OomPriorityManagerTest, Comparator);
  FRIEND_TEST_ALL_PREFIXES(OomPriorityManagerTest, IsReloadableUI);

  struct TabStats {
    TabStats();
    ~TabStats();
    bool is_app;  
    bool is_reloadable_ui;  
    bool is_playing_audio;
    bool is_pinned;
    bool is_selected;  
    bool is_discarded;
    base::TimeTicks last_selected;
    base::ProcessHandle renderer_handle;
    base::string16 title;
    int64 tab_contents_id;  
  };
  typedef std::vector<TabStats> TabStatsList;

  
  
  static bool IsReloadableUI(const GURL& url);

  
  bool DiscardTabById(int64 target_web_contents_id);

  
  
  
  void RecordDiscardStatistics();

  
  
  void RecordRecentTabDiscard();

  
  void PurgeBrowserMemory();

  
  int GetTabCount() const;

  TabStatsList GetTabStatsOnUIThread();

  
  void AdjustOomPriorities();

  
  void AdjustOomPrioritiesOnFileThread(TabStatsList stats_list);

  
  void OnFocusTabScoreAdjustmentTimeout();

  
  void AdjustFocusedTabScoreOnFileThread();

  static bool CompareTabStats(TabStats first, TabStats second);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnMemoryLow() OVERRIDE;

  base::RepeatingTimer<OomPriorityManager> timer_;
  base::OneShotTimer<OomPriorityManager> focus_tab_score_adjust_timer_;
  base::RepeatingTimer<OomPriorityManager> recent_tab_discard_timer_;
  content::NotificationRegistrar registrar_;

  
  base::Lock pid_to_oom_score_lock_;
  
  typedef base::hash_map<base::ProcessHandle, int> ProcessScoreMap;
  ProcessScoreMap pid_to_oom_score_;
  base::ProcessHandle focused_tab_pid_;

  
  scoped_ptr<LowMemoryListener> low_memory_listener_;

  
  base::TimeTicks start_time_;

  
  
  base::TimeTicks last_discard_time_;

  
  
  base::TimeTicks last_adjust_time_;

  
  int discard_count_;

  
  
  bool recent_tab_discard_;

  DISALLOW_COPY_AND_ASSIGN(OomPriorityManager);
};

}  

#endif  
