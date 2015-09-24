// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TABS_TAB_STRIP_SELECTION_MODEL_H_
#define CHROME_BROWSER_TABS_TAB_STRIP_SELECTION_MODEL_H_
#pragma once

#include <vector>

#include "base/basictypes.h"

class TabStripSelectionModel {
 public:
  typedef std::vector<int> SelectedIndices;

  
  static const int kUnselectedIndex;

  TabStripSelectionModel();
  ~TabStripSelectionModel();

  
  void set_anchor(int anchor) { anchor_ = anchor; }
  int anchor() const { return anchor_; }

  
  void set_active(int active) { active_ = active; }
  int active() const { return active_; }

  
  bool empty() const { return selected_indices_.empty(); }

  
  size_t size() const { return selected_indices_.size(); }

  
  
  
  
  void IncrementFrom(int index);

  
  
  
  
  void DecrementFrom(int index);

  
  void SetSelectedIndex(int index);

  
  bool IsSelected(int index) const;

  
  
  void AddIndexToSelection(int index);

  
  
  void RemoveIndexFromSelection(int index);

  
  
  void SetSelectionFromAnchorTo(int index);

  
  
  void AddSelectionFromAnchorTo(int index);

  
  
  
  
  
  
  void Move(int from, int to);

  
  
  void Clear();

  
  
  const SelectedIndices& selected_indices() const { return selected_indices_; }

  
  void Copy(const TabStripSelectionModel& source);

 private:
  SelectedIndices selected_indices_;

  int active_;

  int anchor_;

  DISALLOW_COPY_AND_ASSIGN(TabStripSelectionModel);
};

#endif  
