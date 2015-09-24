// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FAST_UNLOAD_CONTROLLER_H_
#define CHROME_BROWSER_UI_FAST_UNLOAD_CONTROLLER_H_

#include <set>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_piece.h"
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
class FastUnloadController : public content::NotificationObserver,
                             public TabStripModelObserver {
 public:
  explicit FastUnloadController(Browser* browser);
  virtual ~FastUnloadController();

  
  
  
  
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

  
  bool HasCompletedUnloadProcessing() const;

 private:
  
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

  
  
  
  bool DetachWebContents(content::WebContents* contents);

  
  void ProcessPendingTabs();

  
  
  void CancelWindowClose();

  
  
  
  
  void ClearUnloadState(content::WebContents* contents);

  
  void PostTaskForProcessPendingTabs();

  
  void LogUnloadStep(const base::StringPiece& step_name,
                     content::WebContents* contents) const;

  bool is_calling_before_unload_handlers() {
    return !on_close_confirmed_.is_null();
  }

  Browser* browser_;

  content::NotificationRegistrar registrar_;

  typedef std::set<content::WebContents*> WebContentsSet;

  
  
  WebContentsSet tabs_needing_before_unload_;

  
  
  content::WebContents* tab_needing_before_unload_ack_;

  
  
  WebContentsSet tabs_needing_unload_;

  
  
  WebContentsSet tabs_needing_unload_ack_;

  
  
  
  
  bool is_attempting_to_close_browser_;

  
  
  
  base::Callback<void(bool)> on_close_confirmed_;

  
  class DetachedWebContentsDelegate;
  scoped_ptr<DetachedWebContentsDelegate> detached_delegate_;

  base::WeakPtrFactory<FastUnloadController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(FastUnloadController);
};

}  

#endif  
