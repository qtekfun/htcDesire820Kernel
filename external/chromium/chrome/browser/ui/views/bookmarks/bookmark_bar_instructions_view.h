// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_INSTRUCTIONS_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_INSTRUCTIONS_VIEW_H_
#pragma once

#include "ui/base/accessibility/accessible_view_state.h"
#include "views/controls/link.h"
#include "views/view.h"

namespace views {
class Label;
class Link;
}

class BookmarkBarInstructionsView : public views::View,
                                    public views::LinkController {
 public:
  
  
  class Delegate {
   public:
    virtual void ShowImportDialog() = 0;

   protected:
    virtual ~Delegate() {}
  };

  explicit BookmarkBarInstructionsView(Delegate* delegate);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnThemeChanged() OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

  
  virtual void LinkActivated(views::Link* source, int event_flags) OVERRIDE;

 private:
  void UpdateColors();

  Delegate* delegate_;

  views::Label* instructions_;
  views::Link* import_link_;

  
  
  int baseline_;

  
  
  bool updated_colors_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkBarInstructionsView);
};

#endif  
