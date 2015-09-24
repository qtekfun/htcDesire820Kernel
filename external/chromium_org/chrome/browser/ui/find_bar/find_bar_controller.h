// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_FIND_BAR_FIND_BAR_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class FindBar;

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

class FindBarController : public content::NotificationObserver {
 public:
  
  
  enum SelectionAction {
    kKeepSelectionOnPage,     
                              
    kClearSelectionOnPage,    
    kActivateSelectionOnPage  
  };

  
  
  enum ResultAction {
    kClearResultsInFindBox,  
    kKeepResultsInFindBox,   
  };

  
  explicit FindBarController(FindBar* find_bar);

  virtual ~FindBarController();

  
  void Show();

  
  
  
  void EndFindSession(SelectionAction selection_action,
                      ResultAction results_action);

  
  content::WebContents* web_contents() const { return web_contents_; }

  
  
  
  void ChangeWebContents(content::WebContents* contents);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  FindBar* find_bar() const { return find_bar_.get(); }

  
  
  static gfx::Rect GetLocationForFindbarView(
      gfx::Rect view_location,
      const gfx::Rect& dialog_bounds,
      const gfx::Rect& avoid_overlapping_rect);

 private:
  
  
  
  void UpdateFindBarForCurrentResult();

  
  
  
  
  
  void MaybeSetPrepopulateText();

  content::NotificationRegistrar registrar_;

  scoped_ptr<FindBar> find_bar_;

  
  content::WebContents* web_contents_;

  
  
  int last_reported_matchcount_;

  DISALLOW_COPY_AND_ASSIGN(FindBarController);
};

#endif  
