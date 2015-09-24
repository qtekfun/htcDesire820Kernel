// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_UNLOAD_CONTROLLER_H_
#define CHROME_BROWSER_UI_UNLOAD_CONTROLLER_H_

#include <set>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Browser;
class TabStripModel;

namespace content {
class NotificationSource;
class NotifictaionDetails;
class WebContents;
}

namespace chrome {

class UnloadController : public content::NotificationObserver,
                         public TabStripModelObserver {
 public:
  explicit UnloadController(Browser* browser);
  virtual ~UnloadController();

  
  
  
  
  bool CanCloseContents(content::WebContents* contents);

  
  static bool ShouldRunUnloadEventsHelper(content::WebContents* contents);

  
  
  static bool RunUnloadEventsHelper(content::WebContents* contents);

  
  
  
  
  
  
  
  bool BeforeUnloadFired(content::WebContents* contents, bool proceed);

  bool is_attempting_to_close_browser() const {
    return is_attempting_to_close_browser_;
  }

  
  
  bool ShouldCloseWindow();

  
  
  bool CallBeforeUnloadHandlers(
      const base::Callback<void(bool)>& on_close_confirmed);

  
  
  void ResetBeforeUnloadHandlers();

  
  
  
  
  
  
  
  bool TabsNeedBeforeUnloadFired();

 private:
  typedef std::set<content::WebContents*> UnloadListenerSet;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void TabInsertedAt(content::WebContents* contents,
                             int index,
                             bool foreground) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabStripEmpty() OVERRIDE;

  void TabAttachedImpl(content::WebContents* contents);
  void TabDetachedImpl(content::WebContents* contents);

  
  void ProcessPendingTabs();

  
  bool HasCompletedUnloadProcessing() const;

  
  
  void CancelWindowClose();

  
  
  bool RemoveFromSet(UnloadListenerSet* set,
                     content::WebContents* web_contents);

  
  
  
  
  
  
  
  
  
  void ClearUnloadState(content::WebContents* web_contents, bool process_now);

  bool is_calling_before_unload_handlers() {
    return !on_close_confirmed_.is_null();
  }

  Browser* browser_;

  content::NotificationRegistrar registrar_;

  
  
  UnloadListenerSet tabs_needing_before_unload_fired_;

  
  
  UnloadListenerSet tabs_needing_unload_fired_;

  
  
  
  
  bool is_attempting_to_close_browser_;

  
  
  
  base::Callback<void(bool)> on_close_confirmed_;

  base::WeakPtrFactory<UnloadController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(UnloadController);
};

}  

#endif  
