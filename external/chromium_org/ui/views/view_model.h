// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEW_MODEL_H_
#define UI_VIEWS_VIEW_MODEL_H_

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "ui/gfx/rect.h"
#include "ui/views/views_export.h"

namespace views {

class View;

class VIEWS_EXPORT ViewModel {
 public:
  ViewModel();
  ~ViewModel();

  
  
  void Add(View* view, int index);

  
  
  void Remove(int index);

  
  
  void Move(int index, int target_index);

  
  
  
  
  void MoveViewOnly(int index, int target_index);

  
  int view_size() const { return static_cast<int>(entries_.size()); }

  
  void Clear();

  
  View* view_at(int index) const {
    check_index(index);
    return entries_[index].view;
  }

  void set_ideal_bounds(int index, const gfx::Rect& bounds) {
    check_index(index);
    entries_[index].ideal_bounds = bounds;
  }

  const gfx::Rect& ideal_bounds(int index) const {
    check_index(index);
    return entries_[index].ideal_bounds;
  }

  
  
  int GetIndexOfView(const View* view) const;

 private:
  struct Entry {
    Entry() : view(NULL) {}

    View* view;
    gfx::Rect ideal_bounds;
  };
  typedef std::vector<Entry> Entries;

#if !defined(NDEBUG)
  void check_index(int index) const {
    DCHECK_LT(index, static_cast<int>(entries_.size()));
    DCHECK_GE(index, 0);
  }
#else
  void check_index(int index) const {}
#endif

  Entries entries_;

  DISALLOW_COPY_AND_ASSIGN(ViewModel);
};

}  

#endif  
