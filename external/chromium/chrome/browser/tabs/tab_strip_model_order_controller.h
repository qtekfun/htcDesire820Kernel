// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_STRIP_MODEL_ORDER_CONTROLLER_H_
#define CHROME_BROWSER_TABS_TAB_STRIP_MODEL_ORDER_CONTROLLER_H_
#pragma once

#include "chrome/browser/tabs/tab_strip_model.h"
#include "content/common/page_transition_types.h"

class TabContentsWrapper;

class TabStripModelOrderController : public TabStripModelObserver {
 public:
  explicit TabStripModelOrderController(TabStripModel* tabstrip);
  virtual ~TabStripModelOrderController();

  
  void set_insertion_policy(TabStripModel::InsertionPolicy policy) {
    insertion_policy_ = policy;
  }
  TabStripModel::InsertionPolicy insertion_policy() const {
    return insertion_policy_;
  }

  
  
  int DetermineInsertionIndex(TabContentsWrapper* new_contents,
                              PageTransition::Type transition,
                              bool foreground);

  
  int DetermineInsertionIndexForAppending();

  
  int DetermineNewSelectedIndex(int removed_index) const;

  
  virtual void TabSelectedAt(TabContentsWrapper* old_contents,
                             TabContentsWrapper* new_contents,
                             int index,
                             bool user_gesture);

 private:
  
  
  
  int GetValidIndex(int index, int removing_index) const;

  TabStripModel* tabstrip_;

  TabStripModel::InsertionPolicy insertion_policy_;

  DISALLOW_COPY_AND_ASSIGN(TabStripModelOrderController);
};

#endif  
