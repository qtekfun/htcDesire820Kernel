// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_TAB_HELPER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_TAB_HELPER_H_
#pragma once

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/tab_contents/tab_contents_observer.h"

class TabContents;
class AutomationTabHelper;

namespace IPC {
class Message;
}

class TabEventObserver {
 public:
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  virtual void OnFirstPendingLoad(TabContents* tab_contents) { }

  
  
  
  
  virtual void OnNoMorePendingLoads(TabContents* tab_contents) { }

 protected:
  TabEventObserver();
  virtual ~TabEventObserver();

  
  
  void StartObserving(AutomationTabHelper* tab_helper);

  
  
  
  void StopObserving(AutomationTabHelper* tab_helper);

 private:
  friend class AutomationTabHelperTest;
  typedef std::vector<base::WeakPtr<AutomationTabHelper> > EventSourceVector;

  
  
  EventSourceVector event_sources_;

  DISALLOW_COPY_AND_ASSIGN(TabEventObserver);
};

class AutomationTabHelper
    : public TabContentsObserver,
      public base::SupportsWeakPtr<AutomationTabHelper> {
 public:
  explicit AutomationTabHelper(TabContents* tab_contents);
  virtual ~AutomationTabHelper();

  void AddObserver(TabEventObserver* observer);
  void RemoveObserver(TabEventObserver* observer);

  
  
  bool has_pending_loads() const;

 private:
  friend class AutomationTabHelperTest;

  
  virtual void DidStartLoading();
  virtual void DidStopLoading();
  virtual void RenderViewGone();
  virtual void TabContentsDestroyed(TabContents* tab_contents);
  virtual bool OnMessageReceived(const IPC::Message& message);

  void OnWillPerformClientRedirect(int64 frame_id, double delay_seconds);
  void OnDidCompleteOrCancelClientRedirect(int64 frame_id);
  void OnTabOrRenderViewDestroyed(TabContents* tab_contents);

  
  
  bool is_loading_;

  
  
  std::set<int64> pending_client_redirects_;

  
  ObserverList<TabEventObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(AutomationTabHelper);
};

#endif  
