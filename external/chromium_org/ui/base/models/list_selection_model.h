// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_LIST_SELECTION_MODEL_H_
#define UI_BASE_MODELS_LIST_SELECTION_MODEL_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ListSelectionModel {
 public:
  typedef std::vector<int> SelectedIndices;

  
  static const int kUnselectedIndex;

  ListSelectionModel();
  ~ListSelectionModel();

  
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

  
  void Copy(const ListSelectionModel& source);

  
  bool Equals(const ListSelectionModel& rhs) const;

 private:
  SelectedIndices selected_indices_;

  int active_;

  int anchor_;

  DISALLOW_COPY_AND_ASSIGN(ListSelectionModel);
};

}  

#endif  
