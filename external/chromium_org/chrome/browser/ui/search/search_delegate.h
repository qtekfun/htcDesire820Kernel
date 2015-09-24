// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_SEARCH_DELEGATE_H_
#define CHROME_BROWSER_UI_SEARCH_SEARCH_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/search/search_model_observer.h"

namespace content {
class WebContents;
}

class SearchModel;

class SearchDelegate : public SearchModelObserver {
 public:
  explicit SearchDelegate(SearchModel* browser_search_model);
  virtual ~SearchDelegate();

  
  virtual void ModelChanged(const SearchModel::State& old_state,
                            const SearchModel::State& new_state) OVERRIDE;

  
  
  void OnTabActivated(content::WebContents* web_contents);

  
  
  void OnTabDeactivated(content::WebContents* web_contents);

  
  
  void OnTabDetached(content::WebContents* web_contents);

 private:
  
  void StopObservingTab(content::WebContents* web_contents);

  
  
  SearchModel* browser_model_;

  
  
  SearchModel* tab_model_;

  DISALLOW_COPY_AND_ASSIGN(SearchDelegate);
};

#endif  
