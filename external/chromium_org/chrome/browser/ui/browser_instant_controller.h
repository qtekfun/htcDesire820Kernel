// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_INSTANT_CONTROLLER_H_
#define CHROME_BROWSER_UI_BROWSER_INSTANT_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/search/instant_service_observer.h"
#include "chrome/browser/ui/search/instant_controller.h"
#include "chrome/browser/ui/search/instant_unload_handler.h"
#include "chrome/browser/ui/search/search_model_observer.h"

class Browser;
struct InstantSuggestion;
class Profile;

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

class BrowserInstantController : public SearchModelObserver,
                                 public InstantServiceObserver {
 public:
  explicit BrowserInstantController(Browser* browser);
  virtual ~BrowserInstantController();

  
  
  
  
  
  
  
  bool MaybeSwapInInstantNTPContents(
      const GURL& url,
      content::WebContents* source_contents,
      content::WebContents** target_contents);

  
  
  bool OpenInstant(WindowOpenDisposition disposition, const GURL& url);

  
  Profile* profile() const;

  
  
  InstantController* instant() { return &instant_; }

  
  content::WebContents* GetActiveWebContents() const;

  
  void ActiveTabChanged();

  
  void TabDeactivated(content::WebContents* contents);

  
  void SetOmniboxBounds(const gfx::Rect& bounds);

 private:
  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) OVERRIDE;

  
  virtual void DefaultSearchProviderChanged() OVERRIDE;
  virtual void GoogleURLUpdated() OVERRIDE;

  
  
  void ReloadTabsInInstantProcess();

  
  
  void ReplaceWebContentsAt(int index,
                            scoped_ptr<content::WebContents> new_contents);

  Browser* const browser_;

  InstantController instant_;
  InstantUnloadHandler instant_unload_handler_;

  DISALLOW_COPY_AND_ASSIGN(BrowserInstantController);
};

#endif  
