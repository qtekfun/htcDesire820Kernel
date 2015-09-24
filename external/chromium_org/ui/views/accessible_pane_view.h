// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ACCESSIBLE_PANE_VIEW_H_
#define UI_VIEWS_ACCESSIBLE_PANE_VIEW_H_

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/view.h"

namespace views {
class FocusSearch;

class VIEWS_EXPORT AccessiblePaneView : public View,
                                        public FocusChangeListener,
                                        public FocusTraversable {
 public:
  AccessiblePaneView();
  virtual ~AccessiblePaneView();

  
  
  
  
  
  virtual bool SetPaneFocus(View* initial_focus);

  
  
  
  
  virtual bool SetPaneFocusAndFocusDefault();

  
  virtual FocusTraversable* GetPaneFocusTraversable() OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator)
      OVERRIDE;
  virtual void SetVisible(bool flag) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void RequestFocus() OVERRIDE;

  
  virtual void OnWillChangeFocus(View* focused_before,
                                 View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before,
                                View* focused_now) OVERRIDE;

  
  virtual FocusSearch* GetFocusSearch() OVERRIDE;
  virtual FocusTraversable* GetFocusTraversableParent() OVERRIDE;
  virtual View* GetFocusTraversableParentView() OVERRIDE;

  
  const ui::Accelerator& home_key() const { return home_key_; }
  const ui::Accelerator& end_key() const { return end_key_; }
  const ui::Accelerator& escape_key() const { return escape_key_; }
  const ui::Accelerator& left_key() const { return left_key_; }
  const ui::Accelerator& right_key() const { return right_key_; }

 protected:
  
  
  virtual View* GetDefaultFocusableChild();

  
  
  virtual View* GetParentForFocusSearch(View* v);

  
  
  
  virtual bool ContainsForFocusSearch(View* root, const View* v);

  
  virtual void RemovePaneFocus();

  View* GetFirstFocusableChild();
  View* GetLastFocusableChild();

  FocusManager* focus_manager() const { return focus_manager_; }

  
  
  
  void set_allow_deactivate_on_esc(bool allow) {
    allow_deactivate_on_esc_ = allow;
  }

 private:
  bool pane_has_focus_;

  
  
  bool allow_deactivate_on_esc_;

  base::WeakPtrFactory<AccessiblePaneView> method_factory_;

  
  
  FocusManager* focus_manager_;

  
  
  
  scoped_ptr<FocusSearch> focus_search_;

  
  ui::Accelerator home_key_;
  ui::Accelerator end_key_;
  ui::Accelerator escape_key_;
  ui::Accelerator left_key_;
  ui::Accelerator right_key_;

  
  int last_focused_view_storage_id_;

  friend class AccessiblePaneViewFocusSearch;

  DISALLOW_COPY_AND_ASSIGN(AccessiblePaneView);
};

}  

#endif  
