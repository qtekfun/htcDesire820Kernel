// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_FIRST_RUN_DIALOG_H_
#define CHROME_BROWSER_UI_GTK_FIRST_RUN_DIALOG_H_
#pragma once

typedef struct _GtkButton GtkButton;
typedef struct _GtkWidget GtkWidget;

#include "chrome/browser/first_run/first_run.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "ui/base/gtk/gtk_signal.h"

class TemplateURL;
class TemplateURLModel;

class FirstRunDialog : public TemplateURLModelObserver {
 public:
  
  static bool Show(Profile* profile, bool randomize_search_engine_order);

  virtual void OnTemplateURLModelChanged();

 private:
  FirstRunDialog(Profile* profile,
                 bool show_reporting_dialog,
                 bool show_search_engines_dialog,
                 int* response);
  virtual ~FirstRunDialog();

  CHROMEGTK_CALLBACK_1(FirstRunDialog, void, OnResponseDialog, int);
  CHROMEGTK_CALLBACK_0(FirstRunDialog, void, OnSearchEngineButtonClicked);
  CHROMEGTK_CALLBACK_0(FirstRunDialog, void, OnSearchEngineWindowDestroy);
  CHROMEG_CALLBACK_0(FirstRunDialog, void, OnLearnMoreLinkClicked, GtkButton*);

  void ShowSearchEngineWindow();
  void ShowReportingDialog();

  
  
  
  void FirstRunDone();

  
  
  GtkWidget* search_engine_window_;

  
  GtkWidget* dialog_;

  
  GtkWidget* search_engine_hbox_;

  
  GtkWidget* report_crashes_;

  
  GtkWidget* make_default_;

  
  Profile* profile_;

  
  TemplateURLModel* search_engines_model_;

  
  
  TemplateURL* chosen_search_engine_;

  
  
  bool show_reporting_dialog_;

  
  int* response_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunDialog);
};

#endif  
