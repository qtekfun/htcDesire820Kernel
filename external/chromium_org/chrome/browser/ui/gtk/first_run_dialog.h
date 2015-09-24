// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FIRST_RUN_DIALOG_H_
#define CHROME_BROWSER_UI_GTK_FIRST_RUN_DIALOG_H_

typedef struct _GtkButton GtkButton;
typedef struct _GtkWidget GtkWidget;

#include "base/compiler_specific.h"
#include "chrome/browser/first_run/first_run.h"
#include "ui/base/gtk/gtk_signal.h"

class FirstRunDialog {
 public:
  
  
  static bool Show(Profile* profile);

 private:
  explicit FirstRunDialog(Profile* profile);
  virtual ~FirstRunDialog();

  CHROMEGTK_CALLBACK_1(FirstRunDialog, void, OnResponseDialog, int);
  CHROMEG_CALLBACK_0(FirstRunDialog, void, OnLearnMoreLinkClicked, GtkButton*);

  void ShowReportingDialog();

  
  
  
  void FirstRunDone();

  Profile* profile_;

  
  GtkWidget* dialog_;

  
  GtkWidget* report_crashes_;

  
  GtkWidget* make_default_;

  
  
  bool show_reporting_dialog_;

  
  int* response_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunDialog);
};

#endif  
