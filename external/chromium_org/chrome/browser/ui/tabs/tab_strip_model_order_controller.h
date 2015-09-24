// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_ORDER_CONTROLLER_H_
#define CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_ORDER_CONTROLLER_H_

#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/common/page_transition_types.h"

class TabStripModelOrderController : public TabStripModelObserver {
 public:
  explicit TabStripModelOrderController(TabStripModel* tabstrip);
  virtual ~TabStripModelOrderController();

  
  
  int DetermineInsertionIndex(content::PageTransition transition,
                              bool foreground);

  
  int DetermineNewSelectedIndex(int removed_index) const;

  
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;

 private:
  
  
  
  int GetValidIndex(int index, int removing_index) const;

  TabStripModel* tabstrip_;

  DISALLOW_COPY_AND_ASSIGN(TabStripModelOrderController);
};

#endif  
