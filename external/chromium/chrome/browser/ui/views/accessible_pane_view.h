// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_ACCESSIBLE_PANE_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_ACCESSIBLE_PANE_VIEW_H_
#pragma once

#include "base/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "views/focus/focus_manager.h"
#include "views/view.h"

namespace views {
class FocusSearch;
}

class AccessiblePaneView : public views::View,
                           public views::FocusChangeListener,
                           public views::FocusTraversable {
 public:
  AccessiblePaneView();
  virtual ~AccessiblePaneView();

  
  
  
  
  
  virtual bool SetPaneFocus(int view_storage_id, View* initial_focus);

  
  
  
  
  virtual bool SetPaneFocusAndFocusDefault(int view_storage_id);

  
  virtual FocusTraversable* GetPaneFocusTraversable() OVERRIDE;
  virtual bool AcceleratorPressed(const views::Accelerator& accelerator)
      OVERRIDE;
  virtual void SetVisible(bool flag) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void FocusWillChange(View* focused_before,
                               View* focused_now) OVERRIDE;

  
  virtual views::FocusSearch* GetFocusSearch() OVERRIDE;
  virtual FocusTraversable* GetFocusTraversableParent() OVERRIDE;
  virtual View* GetFocusTraversableParentView() OVERRIDE;

 protected:
  
  
  virtual views::View* GetDefaultFocusableChild();

  
  virtual void RemovePaneFocus();

  
  virtual void LocationBarSelectAll();

  void RestoreLastFocusedView();

  View* GetFirstFocusableChild();
  View* GetLastFocusableChild();

  bool pane_has_focus_;

  ScopedRunnableMethodFactory<AccessiblePaneView> method_factory_;

  
  
  views::FocusManager* focus_manager_;

  
  
  
  scoped_ptr<views::FocusSearch> focus_search_;

  
  views::Accelerator home_key_;
  views::Accelerator end_key_;
  views::Accelerator escape_key_;
  views::Accelerator left_key_;
  views::Accelerator right_key_;

  
  int last_focused_view_storage_id_;

  DISALLOW_COPY_AND_ASSIGN(AccessiblePaneView);
};

#endif  
