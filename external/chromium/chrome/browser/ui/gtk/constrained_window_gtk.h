// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_CONSTRAINED_WINDOW_GTK_H_
#define CHROME_BROWSER_UI_GTK_CONSTRAINED_WINDOW_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/basictypes.h"
#include "base/task.h"
#include "chrome/browser/ui/gtk/owned_widget_gtk.h"
#include "content/browser/tab_contents/constrained_window.h"
#include "ui/base/gtk/gtk_signal.h"

class TabContents;
typedef struct _GdkColor GdkColor;
#if defined(TOUCH_UI)
class TabContentsViewTouch;
#else
class TabContentsViewGtk;
#endif

class ConstrainedWindowGtkDelegate {
 public:
  
  virtual GtkWidget* GetWidgetRoot() = 0;

  
  virtual GtkWidget* GetFocusWidget() = 0;

  
  
  virtual void DeleteDelegate() = 0;

  virtual bool GetBackgroundColor(GdkColor* color);

  
  virtual bool ShouldHaveBorderPadding() const;

 protected:
  virtual ~ConstrainedWindowGtkDelegate();
};

class ConstrainedWindowGtk : public ConstrainedWindow {
 public:
#if defined(TOUCH_UI)
   typedef TabContentsViewTouch TabContentsViewType;
#else
   typedef TabContentsViewGtk TabContentsViewType;
#endif

  virtual ~ConstrainedWindowGtk();

  
  virtual void ShowConstrainedWindow();
  virtual void CloseConstrainedWindow();
  virtual void FocusConstrainedWindow();

  
  TabContents* owner() const { return owner_; }

  
  GtkWidget* widget() { return border_.get(); }

  
  TabContentsViewType* ContainingView();

 private:
  friend class ConstrainedWindow;

  ConstrainedWindowGtk(TabContents* owner,
                       ConstrainedWindowGtkDelegate* delegate);

  
  CHROMEGTK_CALLBACK_1(ConstrainedWindowGtk, gboolean, OnKeyPress,
                       GdkEventKey*);
  CHROMEGTK_CALLBACK_1(ConstrainedWindowGtk, void, OnHierarchyChanged,
                       GtkWidget*);

  
  TabContents* owner_;

  
  OwnedWidgetGtk border_;

  
  ConstrainedWindowGtkDelegate* delegate_;

  
  bool visible_;

  ScopedRunnableMethodFactory<ConstrainedWindowGtk> factory_;

  DISALLOW_COPY_AND_ASSIGN(ConstrainedWindowGtk);
};

#endif  
