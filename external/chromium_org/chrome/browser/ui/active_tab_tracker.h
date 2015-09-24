// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ACTIVE_TAB_TRACKER_H_
#define CHROME_BROWSER_UI_ACTIVE_TAB_TRACKER_H_

#include "base/logging.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/idle.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/native_focus_tracker.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

class Browser;

namespace content {
class WebContents;
}

class ActiveTabTracker : public chrome::BrowserListObserver,
                         public TabStripModelObserver,
                         public content::NotificationObserver,
                         public NativeFocusTrackerHost {
 public:
  ActiveTabTracker();
  virtual ~ActiveTabTracker();

  
  bool is_valid() const { return native_focus_tracker_.get() != NULL; }

  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabStripEmpty() OVERRIDE;

  
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void SetBrowser(Browser* browser) OVERRIDE;

 private:
  
  void SetWebContents(content::WebContents* web_contents);

  
  void SetIdleState(IdleState idle_state);

  
  
  void QueryIdleState();

  
  
  
  GURL GetURLFromWebContents() const;

  
  void CommitActiveTime();

  scoped_ptr<NativeFocusTracker> native_focus_tracker_;

  
  Browser* browser_;

  
  content::WebContents* web_contents_;

  
  
  IdleState idle_state_;

  
  base::TimeTicks active_time_;

  
  base::Timer timer_;

  
  GURL url_;

  content::NotificationRegistrar registrar_;

  
  base::WeakPtrFactory<ActiveTabTracker> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ActiveTabTracker);
};

#endif  
