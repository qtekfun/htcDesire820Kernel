// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_REPOST_FORM_WARNING_GTK_H_
#define CHROME_BROWSER_UI_GTK_REPOST_FORM_WARNING_GTK_H_
#pragma once

#include <gtk/gtk.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/gtk/constrained_window_gtk.h"
#include "ui/base/gtk/gtk_signal.h"

class RepostFormWarningController;

class RepostFormWarningGtk : public ConstrainedDialogDelegate {
 public:
  RepostFormWarningGtk(GtkWindow* parent, TabContents* tab_contents);

  
  virtual GtkWidget* GetWidgetRoot();
  virtual GtkWidget* GetFocusWidget();
  virtual void DeleteDelegate();

 private:
  virtual ~RepostFormWarningGtk();

  
  CHROMEGTK_CALLBACK_0(RepostFormWarningGtk, void, OnRefresh);
  CHROMEGTK_CALLBACK_0(RepostFormWarningGtk, void, OnCancel);

  scoped_ptr<RepostFormWarningController> controller_;

  GtkWidget* dialog_;
  GtkWidget* ok_;
  GtkWidget* cancel_;

  DISALLOW_COPY_AND_ASSIGN(RepostFormWarningGtk);
};

#endif  
