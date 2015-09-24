// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_CLIENT_VIEW_H_
#define UI_VIEWS_WINDOW_CLIENT_VIEW_H_

#include "ui/views/view.h"

namespace views {

class DialogClientView;
class Widget;

class VIEWS_EXPORT ClientView : public View {
 public:
  
  static const char kViewClassName[];

  
  
  
  
  ClientView(Widget* widget, View* contents_view);
  virtual ~ClientView() {}

  
  virtual DialogClientView* AsDialogClientView();
  virtual const DialogClientView* AsDialogClientView() const;

  
  
  
  
  virtual bool CanClose();

  
  virtual void WidgetClosing();

  
  
  
  
  
  
  
  
  
  virtual int NonClientHitTest(const gfx::Point& point);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 protected:
  
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;

  
  View* contents_view() const { return contents_view_; }
  void set_contents_view(View* contents_view) {
    contents_view_ = contents_view;
  }

 private:
  
  View* contents_view_;
};

}  

#endif  
