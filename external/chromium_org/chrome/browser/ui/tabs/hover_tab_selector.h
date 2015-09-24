// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_HOVER_TAB_SELECTOR_H_
#define CHROME_BROWSER_UI_TABS_HOVER_TAB_SELECTOR_H_

#include "base/memory/weak_ptr.h"

class TabStripModel;

class HoverTabSelector {
 public:
  explicit HoverTabSelector(TabStripModel* tab_strip_model);
  ~HoverTabSelector();

  
  
  
  
  void StartTabTransition(int index);

  
  void CancelTabTransition();

 private:
  
  void PerformTabTransition();

  
  TabStripModel* tab_strip_model_;

  
  int tab_transition_tab_index_;

  
  base::WeakPtrFactory<HoverTabSelector> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(HoverTabSelector);
};

#endif  

