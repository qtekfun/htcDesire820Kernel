// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_FOCUS_FOCUS_SEARCH_H_
#define UI_VIEWS_FOCUS_FOCUS_SEARCH_H_

#include "ui/views/view.h"

namespace views {

class FocusTraversable;

class VIEWS_EXPORT FocusSearch {
 public:
  
  
  
  
  
  enum Direction {
    UP = 0,
    DOWN
  };

  
  
  
  
  
  
  
  
  
  
  
  FocusSearch(View* root, bool cycle, bool accessibility_mode);
  virtual ~FocusSearch() {}

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual View* FindNextFocusableView(View* starting_view,
                                      bool reverse,
                                      Direction direction,
                                      bool check_starting_view,
                                      FocusTraversable** focus_traversable,
                                      View** focus_traversable_view);

 protected:
  
  
  
  virtual View* GetParent(View* v);

  
  
  virtual bool Contains(View* root, const View* v);

  View* root() const { return root_; }

 private:
  
  
  bool IsViewFocusableCandidate(View* v, int skip_group_id);

  
  
  bool IsFocusable(View* v);

  
  
  
  View* FindSelectedViewForGroup(View* view);

  
  
  
  
  
  View* FindNextFocusableViewImpl(View* starting_view,
                                  bool check_starting_view,
                                  bool can_go_up,
                                  bool can_go_down,
                                  int skip_group_id,
                                  FocusTraversable** focus_traversable,
                                  View** focus_traversable_view);

  
  View* FindPreviousFocusableViewImpl(View* starting_view,
                                      bool check_starting_view,
                                      bool can_go_up,
                                      bool can_go_down,
                                      int skip_group_id,
                                      FocusTraversable** focus_traversable,
                                      View** focus_traversable_view);

  View* root_;
  bool cycle_;
  bool accessibility_mode_;

  DISALLOW_COPY_AND_ASSIGN(FocusSearch);
};

}  

#endif  
