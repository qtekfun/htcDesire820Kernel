// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_H_
#define UI_VIEWS_CONTROLS_TABBED_PANE_TABBED_PANE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/views/view.h"

namespace views {

class Tab;
class TabbedPaneListener;
class TabStrip;

class VIEWS_EXPORT TabbedPane : public View {
 public:
  
  static const char kViewClassName[];

  TabbedPane();
  virtual ~TabbedPane();

  TabbedPaneListener* listener() const { return listener_; }
  void set_listener(TabbedPaneListener* listener) { listener_ = listener; }

  int selected_tab_index() const { return selected_tab_index_; }

  
  int GetTabCount();

  
  View* GetSelectedTab();

  
  
  
  void AddTab(const string16& title, View* contents);

  
  
  
  void AddTabAtIndex(int index, const string16& title, View* contents);

  
  void SelectTabAt(int index);

  
  void SelectTab(Tab* tab);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 private:
   friend class TabStrip;

   
   Tab* GetTabAt(int index);

  
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  TabbedPaneListener* listener_;

  
  
  TabStrip* tab_strip_;
  View* contents_;

  
  int selected_tab_index_;

  DISALLOW_COPY_AND_ASSIGN(TabbedPane);
};

}  

#endif  
